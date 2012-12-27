#include "stdafx.h"
#include "BlockRelatedDrawer.h"
#include "CUDrawer.h"
#include "PUDrawer.h"
#include "MVDrawer.h"
#include "DecisionModeDrawer.h"
#include "DiffDrawer.h"
#include <cstdarg>
#include <stack>
#include <algorithm>

BOOL sysuVideo::BlockDrawersFactory::bHasInstance = FALSE;
sysuVideo::BlockDrawersFactory* sysuVideo::BlockDrawersFactory::ins = NULL;

sysuVideo::BlockDrawersFactory* sysuVideo::BlockDrawersFactory::GetInstance()
{
	if (FALSE == bHasInstance)
	{
		ins = new BlockDrawersFactory();
		bHasInstance = TRUE;
	}
	return ins;
}

sysuVideo::DrawerBase* sysuVideo::BlockDrawersFactory::CreateDrawer(sysuVideo::DRAWERTYPE t)
{
	DrawerBase* pd = NULL;
	switch(t)
	{
	case DRAWERTYPE::CUDRAWER:
		pd = new CUDrawer();
		break;

	case DRAWERTYPE::PUDRAWER:
		pd = new PUDrawer();
		break;

	case DRAWERTYPE::MVDRAWER:
		pd = new MVDrawer();
		break;

	case DRAWERTYPE::MODEDECISIONDRAWER:
		pd = new DecisionModeDrawer();
		break;

	case DRAWERTYPE::DIFFDRAWER:
		pd = new DiffDrawer();
		break;

	default:
		break;
	}

	return pd;
}

//experiment only construct
sysuVideo::BlockRelatedDrawer::BlockRelatedDrawer(CImage *ci, LPWSTR bs, LPWSTR mv, 
												  LPWSTR md, LPWSTR diff, LPWSTR cmp)
{
	imgBase = ci;
	imgLayout.Create(ci->GetWidth(), ci->GetHeight(), 32, CImage::createAlphaChannel);
	
	drawers.push_back(new DecisionModeDrawer());
	(**(drawers.rbegin())).Init(md);

	drawers.push_back(new CUDrawer());
	//(**(drawers.rbegin())).Init(_T("d:/master/rc/decoder_cupu.txt"));
	(**(drawers.rbegin())).Init();

	drawers.push_back(new PUDrawer());
	//(**(drawers.rbegin())).Init(_T("d:/master/rc/decoder_cupu.txt"));
	(**(drawers.rbegin())).Init();
	
	drawers.push_back(new MVDrawer());
	(**(drawers.rbegin())).Init(mv);

	drawers.push_back(new DiffDrawer());
	(**(drawers.rbegin())).Init(diff, 0, cmp);

	bsmgr = new BlockSequenceManager(ci);
	//bsmgr->BuildIndex();
	
	for (DrawerBase *sdd : drawers)
		activeDrawers.push_back(sdd);
}

sysuVideo::BlockRelatedDrawer::BlockRelatedDrawer(CImage *ci)
{
	imgBase = ci;
	imgLayout.Create(ci->GetWidth(), ci->GetHeight(), 32, CImage::createAlphaChannel);
	
	drawers.push_back(new DecisionModeDrawer());
	//(**(drawers.rbegin())).Init(_T("d:/master/rc/decoder_pred.txt"));

	drawers.push_back(new CUDrawer());
	//(**(drawers.rbegin())).Init(_T("d:/master/rc/decoder_cupu.txt"));
	//(**(drawers.rbegin())).Init();

	drawers.push_back(new PUDrawer());
	//(**(drawers.rbegin())).Init(_T("d:/master/rc/decoder_cupu.txt"));
	//(**(drawers.rbegin())).Init();
	
	drawers.push_back(new MVDrawer());
	//(**(drawers.rbegin())).Init(_T("d:/master/rc/decoder_mv.txt"));

	drawers.push_back(new DiffDrawer());

	bsmgr = new BlockSequenceManager(/*_T("d:/master/rc/decoder_cupu.txt")*/ci);
	//bsmgr->BuildIndex();
	
	for (DrawerBase *sdd : drawers)
		activeDrawers.push_back(sdd);
}

sysuVideo::BlockRelatedDrawer::~BlockRelatedDrawer()
{
	std::for_each(drawers.begin(), drawers.end(), 
		[] (DrawerBase *pd) -> void { delete pd; }); 
	imgLayout.Destroy();
	
	if (bsmgr != NULL)
		delete bsmgr;
}

void* sysuVideo::BlockRelatedDrawer::DoForeachDrawer(sysuVideo::DRAWERTYPE type, void* (*op)(sysuVideo::DrawerBase *pDrawer))
{
	static void *result;

	std::for_each(activeDrawers.begin(), activeDrawers.end(),
		[type, op] (DrawerBase *pd) -> void 
	{
		if (type == pd->GetDrawerType())
			result = op(pd);
	});

	return result;
}

void sysuVideo::BlockRelatedDrawer::Decorate(void *img, int /*#num arg*/, ...)
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
	va_end(ap);
}

BOOL sysuVideo::BlockRelatedDrawer::ActivateDrawers(DRAWERTYPE type, BOOL activationCode)
{
	std::for_each(activeDrawers.begin(), activeDrawers.end(),
		[type, activationCode] (DrawerBase *db) -> void
		{ 
			if (type == db->GetDrawerType())
				db->Enable(activationCode); 
		}
	);

	return TRUE;
}

void sysuVideo::BlockRelatedDrawer::AddParams(sysuVideo::PDecoratorParams pParams)
{
	//MessageBox(NULL, _T("Memeber function AddParams Not implemented"), _T("Not implemented"), MB_OK);
	DrawerBase *pd;

	switch (pParams->op)
	{
	case DECORATOROPCODE::ADD:
		pd = BlockDrawersFactory::GetInstance()->CreateDrawer(pParams->dt);
		if (NULL != pd)
			activeDrawers.push_back(pd);
		activeDrawers.sort([] (const DrawerBase* a, const DrawerBase* b) 
			-> bool { return a->GetDrawerType() > b->GetDrawerType(); });
		break;

	case DECORATOROPCODE::REMOVE:
		std::remove_if(activeDrawers.begin(), activeDrawers.end(), [pParams] (DrawerBase *pdb)
			-> BOOL { return pdb->GetDrawerType() == pParams->dt; });
		break;

	case DECORATOROPCODE::CHANGESETTING:
		break;

	default:
		break;		
	}
}

void sysuVideo::BlockRelatedDrawer::drawBlockInfo()
{
	static ImgBlock block;
	static CDC dc;
	CImage *ci = &imgLayout;
	unsigned long frm = workingFrameCnt;
	HDC hdc;
	CDC *pDC = &dc;
	
	memset(imgLayout.GetPitch() * (imgLayout.GetHeight() - 1) + (BYTE *)imgLayout.GetBits(), 0,
		imgLayout.GetWidth() * imgLayout.GetHeight() * 4); 

	bsmgr->Locale(workingFrameCnt);
	std::for_each(activeDrawers.begin(), activeDrawers.end(),
		[frm] (DrawerBase *pd) -> void { pd->PreDrawingFrame(frm); });

	hdc = imgLayout.GetDC();
	pDC->Attach(hdc);
	while (bsmgr->GetNextBlock(&block))
	{
		std::for_each(activeDrawers.begin(), activeDrawers.end(), 
			[pDC] (DrawerBase *pd) -> void { pd->Draw(&block, pDC); });
	}

	pDC->Detach();
	imgLayout.ReleaseDC();
}

BOOL sysuVideo::BlockRelatedDrawer::IsReady()
{
	return bsmgr->IsReady();
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

