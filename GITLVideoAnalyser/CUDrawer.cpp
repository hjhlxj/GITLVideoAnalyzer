#include "stdafx.h"
#include "CUDrawer.h"


sysuVideo::CUDrawer::CUDrawer(void)
{
	enable = FALSE;

	penWidth = 1;
	penStyle = PS_SOLID;
	penColor = RGB(128, 128, 128);
	pen.CreatePen(penStyle, penWidth, penColor);

	//drawFlag = new BYTE[1000];
}


sysuVideo::CUDrawer::~CUDrawer(void)
{
	/*if (nullptr != drawFlag)
		delete [] drawFlag;*/
}

inline sysuVideo::DRAWERTYPE sysuVideo::CUDrawer::GetDrawerType() const
{
	return sysuVideo::DRAWERTYPE::CUDRAWER;
}

void sysuVideo::CUDrawer::Init()
{
	//enable = TRUE;
	return;

	//if (0 != _wfopen_s(&directStream, filepath, _T("r")))
	//	return;

	//enable = TRUE;
	//BuildIndex();
}
//
//void sysuVideo::CUDrawer::BuildIndex()
//{
//	return;
//
//	static const int bufSize = 1024 * 3;
//	static char buf[bufSize];
//	static unsigned long frmCnt;
//	static unsigned long curFrm;
//
//	if (directStream == NULL)
//		return;
//
//	streamIndex.clear();
//
//	frmCnt = -1;
//
//	while (fgets(buf, bufSize, directStream) != NULL)
//	{
//		sscanf_s(buf, "<%lu,", &curFrm);
//
//		if (curFrm != frmCnt)
//		{
//			streamIndex.push_back(ftell(directStream) - strlen(buf) - 1);
//			frmCnt = curFrm;
//		}
//	}
//
//	indexSize = streamIndex.size();
//
//	dfOffset = 0;
//	dfSize = -1;
//}
//
//void sysuVideo::CUDrawer::Locale(unsigned long index)
//{
//	return;
//
//	if (!enable)
//		return;
//	
//	if (index >= indexSize)
//		return;
//
//	fseek(directStream, streamIndex[index], SEEK_SET);
//	curWorkingFrm = index;
//}

void sysuVideo::CUDrawer::Draw(ImgBlock *block, CDC *pDC)
{
	static CPen *oldPen;
	static RECT curCU;
	static RECT *cu;

	if (!enable || IMGBLOCKTYPETAG::CU_SPLIT != block->type)
		return;
	
	oldPen = pDC->SelectObject(&pen);
	cu = &(block->area);

	//Middle vertical
	pDC->MoveTo((cu->left + cu->right) / 2, cu->top);
	pDC->LineTo((cu->left + cu->right) / 2, cu->bottom); 

	//Middle horizontal
	pDC->MoveTo(cu->left, (cu->top + cu->bottom) / 2);
	pDC->LineTo(cu->right, (cu->top + cu->bottom) / 2);

	//Left vertical
	pDC->MoveTo(cu->left, cu->top);
	pDC->LineTo(cu->left, cu->bottom);

	//Top horizontal
	pDC->MoveTo(cu->left, cu->top);
	pDC->LineTo(cu->right, cu->top);

	pDC->SelectObject(oldPen);

	//if (dfOffset >= dfSize)
	//	readNextLCUDrawingFlag();

	//if (99 == drawFlag[dfOffset++])	//Split need
	//{		
	//	//Middle vertical
	//	pDC->MoveTo((cu->left + cu->right) / 2, cu->top);
	//	pDC->LineTo((cu->left + cu->right) / 2, cu->bottom); 

	//	//Middle horizontal
	//	pDC->MoveTo(cu->left, (cu->top + cu->bottom) / 2);
	//	pDC->LineTo(cu->right, (cu->top + cu->bottom) / 2);
	//}

	//if (IMGBLOCKTYPE::CU != block->type)	// Draw the outline for the lcu
	//{
	//	//Left vertical
	//	pDC->MoveTo(cu->left, cu->top);
	//	pDC->LineTo(cu->left, cu->bottom);

	//	//Right vertical
	//	/*pDC->MoveTo(cu->right, cu->top);
	//	pDC->LineTo(cu->right, cu->bottom);*/

	//	//Top horizontal
	//	pDC->MoveTo(cu->left, cu->top);
	//	pDC->LineTo(cu->right, cu->top);

	//	//Bottom horizontal
	//	/*pDC->MoveTo(cu->left, cu->bottom);
	//	pDC->LineTo(cu->right, cu->bottom);*/
	//}

}

//void sysuVideo::CUDrawer::readNextLCUDrawingFlag()
//{
//	static const int bufSize = 1024 * 3;
//	static int tmp;
//	static char buf[bufSize];
//	static unsigned long frmCnt;
//	static unsigned long curFrm;
//	static char *token, *nextToken;
//
//	fgets(buf, bufSize, directStream);
//	token = strtok_s(buf, " ", &nextToken);
//	sscanf_s(token, "<%lu,%lu>", &frmCnt, &curFrm);
//	dfSize = 0;
//
//	if (frmCnt != curWorkingFrm)
//	{
//		TCHAR sb[100];
//		swprintf_s(sb, _T("curFrm %d, working Frm %d"), frmCnt, curWorkingFrm);
//		MessageBox(NULL, sb, _T("CU"), MB_OK);
//	}
//
//	token = strtok_s(NULL, " ", &nextToken);
//	while (token != NULL && *token != '\n')
//	{		
//		tmp = atoi(token);
//		drawFlag[dfSize] = (BYTE)tmp;
//		++dfSize;
//		token = strtok_s(NULL, " ", &nextToken);
//	}	
//
//	dfOffset = 0;
//}