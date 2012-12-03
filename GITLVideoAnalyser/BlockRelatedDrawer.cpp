#include "stdafx.h"
#include "BlockRelatedDrawer.h"
#include "CUDrawer.h"
#include "PUDrawer.h"
#include "MVDrawer.h"
#include <cstdarg>
#include <stack>
#include <algorithm>

sysuVideo::BlockRelatedDrawer::BlockRelatedDrawer(CImage *ci)
{
	imgBase = ci;
	imgLayout.Create(ci->GetWidth(), ci->GetHeight(), 32, CImage::createAlphaChannel);
	
	drawers.push_back(new CUDrawer());
	(**(drawers.rbegin())).Init(_T("d:/master/rc/decoder_cupu.txt"));
	
	drawers.push_back(new PUDrawer());
	(**(drawers.rbegin())).Init(_T("d:/master/rc/decoder_cupu.txt"));
	
	drawers.push_back(new MVDrawer());
	(**(drawers.rbegin())).Init(_T("d:/master/rc/decoder_mv.txt"));

	bsmgr = new BlockSequenceManager(_T("d:/master/rc/decoder_cupu.txt"), ci);
	bsmgr->BuildIndex();
	
	for (StreamDirectedDrawer *sdd : drawers)
		activeDrawers.push_back(sdd);
}

sysuVideo::BlockRelatedDrawer::~BlockRelatedDrawer()
{
	std::for_each(drawers.begin(), drawers.end(), 
		[] (StreamDirectedDrawer *pd) -> void { delete pd; }); 
	imgLayout.Destroy();
	
	if (bsmgr != NULL)
		delete bsmgr;
}

void sysuVideo::BlockRelatedDrawer::Decorate(void *img, ...)
{
	va_list ap;
	int targetFrame;
	CImage *pcimg = dynamic_cast<CImage *>((CImage *)img);

	ASSERT(pcimg != NULL);

	va_start(ap, img);
	targetFrame = va_arg(ap, int);

	/*if (targetFrame != 9)
		return ;*/

	workingFrameCnt = targetFrame;

	drawBlockInfo();
	imgLayout.AlphaBlend(pcimg->GetDC(), 0, 0);
	pcimg->ReleaseDC();
}

void sysuVideo::BlockRelatedDrawer::AddParams(void *)
{
	MessageBox(NULL, _T("Memeber function AddParams Not implemented"), _T("Not implemented"), MB_OK);
}

void sysuVideo::BlockRelatedDrawer::drawBlockInfo()
{
	static ImgBlcok block;
	static CDC dc;
	CImage *ci = &imgLayout;
	unsigned long frm = workingFrameCnt;
	HDC hdc;
	CDC *pDC = &dc;
	
	memset(imgLayout.GetPitch() * (imgLayout.GetHeight() - 1) + (BYTE *)imgLayout.GetBits(), 0,
		imgLayout.GetWidth() * imgLayout.GetHeight() * 4); 

	bsmgr->Locale(workingFrameCnt);
	std::for_each(activeDrawers.begin(), activeDrawers.end(),
		[frm] (StreamDirectedDrawer *pd) -> void { pd->Locale(frm); });

	hdc = imgLayout.GetDC();
	pDC->Attach(hdc);
	while (bsmgr->GetNextBlock(&block))
	{
		std::for_each(activeDrawers.begin(), activeDrawers.end(), 
			[pDC] (StreamDirectedDrawer *pd) -> void { pd->Draw(&block, pDC); });
	}

	pDC->Detach();
	imgLayout.ReleaseDC();
}

//void splitCU(RECT& rect, std::stack<RECT>& s)
//{
//	static RECT subRect;
//
//	//Bottom right
//	subRect.top = (rect.top + rect.bottom) / 2;
//	subRect.left = (rect.right + rect.left) / 2;
//	subRect.right = rect.right;
//	subRect.bottom = rect.bottom;
//	s.push(subRect);
//
//	//Bottom left
//	subRect.right = subRect.left;
//	subRect.left = rect.left;
//	s.push(subRect);
//
//	//Top right
//	subRect.bottom = subRect.top;
//	subRect.top = rect.top;
//	subRect.left = subRect.right;
//	subRect.right = rect.right;
//	s.push(subRect);
//
//	//Top left
//	subRect.right = subRect.left;
//	subRect.left = rect.left;
//	s.push(subRect);
//}
//
//sysuVideo::BlockRelatedDrawer::BlockRelatedDrawer(CImage *ci)
//{
//	imgLayout.Create(ci->GetWidth(), ci->GetHeight(), 32, CImage::createAlphaChannel);
//	width = ci->GetWidth();
//	height = ci->GetHeight();
//	pDC = new CDC;
//	penWidth = 1;
//	penStyle = PS_DASHDOTDOT;
//	penColor = RGB(255, 255, 255);
//	pen.CreatePen(penStyle, penWidth, penColor);
//	LCUSIZE = 64;
//	splitFlags = new BYTE[(ci->GetWidth() / LCUSIZE + 1) * (ci->GetHeight() / LCUSIZE + 1) * 4]; 
//
//	fopen_s(&cuInfoStream, "D:\\master\\cu.txt", "r");
//}
//
//sysuVideo::BlockRelatedDrawer::~BlockRelatedDrawer(void)
//{
//	if (pDC != NULL)
//		delete pDC;
//
//	if (splitFlags != NULL)
//		delete [] splitFlags;
//
//	if (cuInfoStream != NULL)
//		fclose(cuInfoStream);
//}
//
//void sysuVideo::BlockRelatedDrawer::AddParams(void *)
//{
//	MessageBox(NULL, _T("Memeber function AddParams Not implemented"), _T("Not implemented"), MB_OK);
//}
//
//void sysuVideo::BlockRelatedDrawer::Decorate(void *img, ...)
//{
//	va_list ap;
//	int targetFrame;
//	static TCHAR buf[250];
//	CImage *pcimg = dynamic_cast<CImage *>((CImage *)img);
//
//	ASSERT(pcimg != NULL);
//
//	va_start(ap, img);
//	targetFrame = va_arg(ap, int);
//	workingFrameCnt = targetFrame;
//
//	wsprintf(buf, _T("arg = %d"), targetFrame);
//	//MessageBox(NULL, buf, _T("Argument validation"), MB_OK);
//	drawBlockInfo();
//	imgLayout.AlphaBlend(pcimg->GetDC(), 0, 0);
//	pcimg->ReleaseDC();
//}
//
//void sysuVideo::BlockRelatedDrawer::drawBlockInfo(void)
//{
//	using namespace std;
//
//	static CPen *oldPen;
//	static stack<RECT> CUs;
//	static RECT curCU;
//
//	static int x;
//
//	memset(imgLayout.GetPitch() * (imgLayout.GetHeight() - 1) + (BYTE *)imgLayout.GetBits(), 0,
//		imgLayout.GetWidth() * imgLayout.GetHeight() * 4);
//
//	HDC hdc = imgLayout.GetDC();
//
//	pDC->Attach(hdc);
//	oldPen = pDC->SelectObject(&pen);
//
//	while (!CUs.empty())
//		CUs.pop();
//
//	curLCU.left = -LCUSIZE;
//	curLCU.top = curLCU.right = 0;
//	curLCU.bottom = LCUSIZE;
//
//	frameReset = TRUE;
//
//	x = 0;
//
//	while (getNextLCU(&curCU))
//	{
//		CUs.push(curCU);
//		++x;
//		drawCU(&curCU);
//		while (!CUs.empty())
//		{
//			curCU = CUs.top();
//			CUs.pop();
//			if (splitContinue(&curCU))
//			{
//				drawCU(&curCU, TRUE);
//				splitCU(curCU, CUs);
//				x += 4;
//			}			
//		}
//	}
//
//	DeleteObject(pDC->SelectObject(oldPen));
//	pDC->Detach();
//	imgLayout.ReleaseDC();
//	/*WCHAR buf[100];
//	wsprintf(buf, _T("total cus: %d"), x);
//	MessageBox(NULL, buf, _T("CU count"), MB_OK);*/
//}
//
//void sysuVideo::BlockRelatedDrawer::drawCU(RECT *cu, BOOL splitCU)
//{
//	if (!splitCU)
//	{
//		//Left vertical
//		pDC->MoveTo(cu->left, cu->top);
//		pDC->LineTo(cu->left, cu->bottom);
//
//		//Right vertical
//		/*pDC->MoveTo(cu->right, cu->top);
//		pDC->LineTo(cu->right, cu->bottom);*/
//
//		//Top horizontal
//		pDC->MoveTo(cu->left, cu->top);
//		pDC->LineTo(cu->right, cu->top);
//
//		//Bottom horizontal
//		/*pDC->MoveTo(cu->left, cu->bottom);
//		pDC->LineTo(cu->right, cu->bottom);*/
//	}
//	else 
//	{
//		//Middle vertical
//		pDC->MoveTo((cu->left + cu->right) / 2, cu->top);
//		pDC->LineTo((cu->left + cu->right) / 2, cu->bottom); 
//
//		//Middle horizontal
//		pDC->MoveTo(cu->left, (cu->top + cu->bottom) / 2);
//		pDC->LineTo(cu->right, (cu->top + cu->bottom) / 2);
//	}
//}
//
//BOOL sysuVideo::BlockRelatedDrawer::getNextLCU(RECT *lcu)
//{	
//	/*static int i = 0;
//
//	if (i++ > 0) return FALSE;*/
//	curLCU.left += LCUSIZE;
//
//	if (curLCU.left >= imgLayout.GetWidth())
//	{
//		if (curLCU.bottom < imgLayout.GetHeight())
//		{
//			//Move to the head of next row
//			curLCU.left = 0;
//			curLCU.top = curLCU.bottom;
//			curLCU.right = LCUSIZE;
//			curLCU.bottom += LCUSIZE;
//
//			//adjust bottom
//			curLCU.bottom = min(curLCU.bottom, imgLayout.GetHeight());
//		}
//		else
//		{
//			return FALSE;	//Final block has been reached
//		}
//	}
//	else
//	{
//		//Horizontal movement
//		curLCU.right += LCUSIZE;
//	}
//
//	//Adjust right
//	curLCU.right = min(curLCU.right, imgLayout.GetWidth());
//	*lcu = curLCU;
//
//	localeCUInfo();	//Locale the stream to the current LCU split information line
//
//	return TRUE;
//}
//
//BOOL sysuVideo::BlockRelatedDrawer::splitContinue(RECT * cub)
//{
//	static int times = 0;
//
//	if (reachAtomicSize(cub))
//	{
//		MessageBox(NULL, _T("reachAtomicSize exec"), _T("oops!"), MB_OK);
//		return FALSE;
//	}
//	else if (sfcursor >= sflength)
//	{
//		return FALSE;
//	}
//	else
//	{
//		return splitFlags[sfcursor++] == 99;
//	}
//}
//
//void sysuVideo::BlockRelatedDrawer::localeCUInfo(void)
//{
//	static const int bufsize = 1024 * 3;
//	static char buf[bufsize];	//3K buffer
//	static int tmp;
//	static char *token, *nextToken;
//	static unsigned long fposition;
//
//	if (frmCnt != workingFrameCnt)
//	{
//		moveToCUInfoBeginLine();
//		fposition = ftell(cuInfoStream);
//	}
//
//	if (frameReset) {
//		fseek(cuInfoStream, fposition, SEEK_SET);
//		frameReset = FALSE;
//	}
//
//	fgets(buf, bufsize, cuInfoStream);
//	token = strtok_s(buf, " ", &nextToken);
//	sscanf_s(token, "<%lu,%lu>", &frmCnt, &CUCnt);
//	if (frmCnt != workingFrameCnt)
//		return;
//	sflength = 0;
//
//	token = strtok_s(NULL, " ", &nextToken);
//	while (token != NULL && *token != '\n')
//	{		
//		tmp = atoi(token);
//		splitFlags[sflength] = (BYTE)tmp;
//		++sflength;
//		token = strtok_s(NULL, " ", &nextToken);
//	}	
//
//	sfcursor = 0;
//}
//
//void sysuVideo::BlockRelatedDrawer::moveToCUInfoBeginLine(void)
//{
//	//Not implemented
//}
//
//BOOL sysuVideo::BlockRelatedDrawer::reachAtomicSize(RECT *cub)
//{
//	return (cub->right - cub->left) < (LCUSIZE / 4 / 4);
//}
//

