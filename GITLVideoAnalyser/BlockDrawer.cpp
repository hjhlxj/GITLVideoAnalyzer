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
	pDC = new CDC;
	penWidth = 1;
	penStyle = PS_DASHDOTDOT;
	penColor = RGB(255, 255, 255);
	pen.CreatePen(penStyle, penWidth, penColor);
	LCUSIZE = 64;
}

sysuVideo::BlockDrawer::~BlockDrawer(void)
{
	if (pDC != NULL)
		delete pDC;
}

void sysuVideo::BlockDrawer::AddParams(void *)
{
	MessageBox(NULL, _T("Not implemented"), _T("Not implemented"), MB_OK);
}

void sysuVideo::BlockDrawer::Decorate(void *img, ...)
{
	va_list ap;
	int type;
	static TCHAR buf[250];
	CImage *pcimg = dynamic_cast<CImage *>((CImage *)img);
	va_start(ap, img);
	type = va_arg(ap, int);

	wsprintf(buf, _T("arg = %d"), type);
	//MessageBox(NULL, buf, _T("Argument validation"), MB_OK);
	drawBlockInfo();
	imgLayout.AlphaBlend(pcimg->GetDC(), 0, 0);
	pcimg->ReleaseDC();
}

void sysuVideo::BlockDrawer::drawBlockInfo()
{
	using namespace std;
	
	static CPen *oldPen;
	static stack<RECT> CUs;
	static RECT curCU;

	HDC hdc = imgLayout.GetDC();

	pDC->Attach(hdc);
	oldPen = pDC->SelectObject(&pen);
	
	while (!CUs.empty())
		CUs.pop();
	
	curLCU.top = curLCU.left = -LCUSIZE;
	curLCU.bottom = curLCU.right = 0;

	while (getNextLCU(&curCU))
	{
		CUs.push(curCU);	
		while (!CUs.empty())
		{
			curCU = CUs.top();
			drawCU(&curCU);
			CUs.pop();
			if (splitContinue())
				splitCU(curCU, CUs);
		}
	}

	DeleteObject(pDC->SelectObject(oldPen));
	pDC->Detach();
	imgLayout.ReleaseDC();
}

void sysuVideo::BlockDrawer::drawCU(RECT *cu)
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

	//Middle horizontal
	pDC->MoveTo((cu->left + cu->right) / 2, 0);
	pDC->LineTo((cu->left + cu->right) / 2, cu->bottom); 

	//Middle vertical
	pDC->MoveTo(cu->left, (cu->top + cu->bottom) / 2);
	pDC->LineTo(cu->right, (cu->top + cu->bottom) / 2);
}

BOOL sysuVideo::BlockDrawer::getNextLCU(RECT *lcu)
{	
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

	return TRUE;
}

BOOL sysuVideo::BlockDrawer::splitContinue()
{
	static int times = 0;

	srand(time(NULL));

	if (++times % 13 == 4)
	{
		//times = 0;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}