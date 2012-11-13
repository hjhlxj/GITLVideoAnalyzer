#include "stdafx.h"
#include "BlockDrawer.h"
#include <cstdarg>
#include <stack>

void splitCU(RECT& rect, std::stack<RECT>& s)
{
	static RECT subRect;

	//Bottom right
	subRect.top = (rect.top + rect.bottom) / 2;
	subRect.left = (rect.right + rect.left) / 2;
	subRect.right = rect.right;
	subRect.bottom = rect.bottom;
	s.push(subRect);

	//Bottom left
	subRect.right = subRect.left;
	subRect.left = rect.left;
	s.push(subRect);

	//Top right
	subRect.bottom = subRect.top;
	subRect.top = rect.top;
	subRect.left = subRect.right;
	subRect.right = rect.right;
	s.push(subRect);

	//Top left
	subRect.right = subRect.left;
	subRect.left = rect.left;
	s.push(subRect);
}

sysuVideo::BlockDrawer::BlockDrawer(CImage *ci)
{
	imgLayout.Create(ci->GetWidth(), ci->GetHeight(), 32, CImage::createAlphaChannel);
	width = ci->GetWidth();
	height = ci->GetHeight();
	pDC = new CDC;
	penWidth = 1;
	penStyle = PS_DASHDOTDOT;
	penColor = RGB(255, 255, 255);
	pen.CreatePen(penStyle, penWidth, penColor);
	LCUSIZE = 64;
	splitFlags = new BYTE[(ci->GetWidth() / LCUSIZE + 1) * (ci->GetHeight() / LCUSIZE + 1) * 4]; 

	fopen_s(&cuInfoStream, "D:\\master\\cu.txt", "r");
}

sysuVideo::BlockDrawer::~BlockDrawer(void)
{
	if (pDC != NULL)
		delete pDC;

	if (splitFlags != NULL)
		delete [] splitFlags;

	if (cuInfoStream != NULL)
		fclose(cuInfoStream);
}

void sysuVideo::BlockDrawer::AddParams(void *)
{
	MessageBox(NULL, _T("Memeber function AddParams Not implemented"), _T("Not implemented"), MB_OK);
}

void sysuVideo::BlockDrawer::Decorate(void *img, ...)
{
	va_list ap;
	int targetFrame;
	static TCHAR buf[250];
	CImage *pcimg = dynamic_cast<CImage *>((CImage *)img);

	ASSERT(pcimg != NULL);

	va_start(ap, img);
	targetFrame = va_arg(ap, int);
	workingFrameCnt = targetFrame;

	wsprintf(buf, _T("arg = %d"), targetFrame);
	//MessageBox(NULL, buf, _T("Argument validation"), MB_OK);
	drawBlockInfo();
	imgLayout.AlphaBlend(pcimg->GetDC(), 0, 0);
	pcimg->ReleaseDC();
}

void sysuVideo::BlockDrawer::drawBlockInfo(void)
{
	using namespace std;

	static CPen *oldPen;
	static stack<RECT> CUs;
	static RECT curCU;

	memset(imgLayout.GetPitch() * (imgLayout.GetHeight() - 1) + (BYTE *)imgLayout.GetBits(), 0,
		imgLayout.GetWidth() * imgLayout.GetHeight() * 4);

	HDC hdc = imgLayout.GetDC();

	pDC->Attach(hdc);
	oldPen = pDC->SelectObject(&pen);

	while (!CUs.empty())
		CUs.pop();

	curLCU.left = -LCUSIZE;
	curLCU.top = curLCU.right = 0;
	curLCU.bottom = LCUSIZE;

	frameReset = TRUE;

	while (getNextLCU(&curCU))
	{
		CUs.push(curCU);
		drawCU(&curCU);
		while (!CUs.empty())
		{
			curCU = CUs.top();
			CUs.pop();
			if (splitContinue(&curCU))
			{
				drawCU(&curCU, TRUE);
				splitCU(curCU, CUs);
			}			
		}
	}

	DeleteObject(pDC->SelectObject(oldPen));
	pDC->Detach();
	imgLayout.ReleaseDC();
}

void sysuVideo::BlockDrawer::drawCU(RECT *cu, BOOL splitCU)
{
	if (!splitCU)
	{
		//Left vertical
		pDC->MoveTo(cu->left, cu->top);
		pDC->LineTo(cu->left, cu->bottom);

		//Right vertical
		/*pDC->MoveTo(cu->right, cu->top);
		pDC->LineTo(cu->right, cu->bottom);*/

		//Top horizontal
		pDC->MoveTo(cu->left, cu->top);
		pDC->LineTo(cu->right, cu->top);

		//Bottom horizontal
		/*pDC->MoveTo(cu->left, cu->bottom);
	pDC->LineTo(cu->right, cu->bottom);*/
	}
	else 
	{
		//Middle vertical
		pDC->MoveTo((cu->left + cu->right) / 2, cu->top);
		pDC->LineTo((cu->left + cu->right) / 2, cu->bottom); 

		//Middle horizontal
		pDC->MoveTo(cu->left, (cu->top + cu->bottom) / 2);
		pDC->LineTo(cu->right, (cu->top + cu->bottom) / 2);
	}
}

BOOL sysuVideo::BlockDrawer::getNextLCU(RECT *lcu)
{	
	/*static int i = 0;

	if (i++ > 0) return FALSE;*/
	curLCU.left += LCUSIZE;

	if (curLCU.left >= imgLayout.GetWidth())
	{
		if (curLCU.bottom < imgLayout.GetHeight())
		{
			//Move to the head of next row
			curLCU.left = 0;
			curLCU.top = curLCU.bottom;
			curLCU.right = LCUSIZE;
			curLCU.bottom += LCUSIZE;

			//adjust bottom
			curLCU.bottom = min(curLCU.bottom, imgLayout.GetHeight());
		}
		else
		{
			return FALSE;	//Final block has been reached
		}
	}
	else
	{
		//Horizontal movement
		curLCU.right += LCUSIZE;
	}

	//Adjust right
	curLCU.right = min(curLCU.right, imgLayout.GetWidth());
	*lcu = curLCU;

	localeCUInfo();	//Locale the stream to the current LCU split information line

	return TRUE;
}

BOOL sysuVideo::BlockDrawer::splitContinue(RECT * cub)
{
	static int times = 0;

	if (reachAtomicSize(cub))
	{
		return FALSE;
	}
	else if (sfcursor >= sflength)
	{
		return FALSE;
	}
	else
	{
		return splitFlags[sfcursor++] != 0;
	}
}

void sysuVideo::BlockDrawer::localeCUInfo(void)
{
	static const int bufsize = 1024 * 3;
	static char buf[bufsize];	//3K buffer
	static int tmp;
	static char *token, *nextToken;
	static unsigned long fposition;

	if (frmCnt != workingFrameCnt)
	{
		moveToCUInfoBeginLine();
		fposition = ftell(cuInfoStream);
	}

	if (frameReset) {
		fseek(cuInfoStream, fposition, SEEK_SET);
		frameReset = FALSE;
	}

	fgets(buf, bufsize, cuInfoStream);
	token = strtok_s(buf, " ", &nextToken);
	sscanf_s(token, "<%lu,%lu>", &frmCnt, &CUCnt);
	sflength = 0;

	token = strtok_s(NULL, " ", &nextToken);
	while (token != NULL && *token != '\n')
	{		
		tmp = atoi(token);
		splitFlags[sflength] = (BYTE)tmp;
		++sflength;
		token = strtok_s(NULL, " ", &nextToken);
	}	

	sfcursor = 0;
}

void sysuVideo::BlockDrawer::moveToCUInfoBeginLine(void)
{
	//Not implemented
}

BOOL sysuVideo::BlockDrawer::reachAtomicSize(RECT *cub)
{
	return (cub->right - cub->left) < (LCUSIZE / 4 / 4);
}