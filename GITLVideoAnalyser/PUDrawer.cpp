#include "stdafx.h"
#include "PUDrawer.h"


sysuVideo::PUDrawer::PUDrawer(void)
{
	enable = FALSE;

	penWidth = 1;
	penStyle = PS_SOLID;
	penColor = RGB(255, 255, 255);
	pen.CreatePen(penStyle, penWidth, penColor);
}


sysuVideo::PUDrawer::~PUDrawer(void)
{
}

inline sysuVideo::DRAWERTYPE sysuVideo::PUDrawer::GetDrawerType() const
{
	return sysuVideo::DRAWERTYPE::PUDRAWER;
}

void sysuVideo::PUDrawer::Init()
{
	//enable = TRUE;
	return;
}

void sysuVideo::PUDrawer::Draw(ImgBlock *block, CDC *pDC)
{
	static CPen *oldPen;
	static RECT curCU;
	static RECT *cu;

	/*if (dfOffset >= dfSize)
		readNextLCUDrawingFlag();*/

	if (!enable || (IMGBLOCKTYPETAG::PU_HORZ_SPLIT != block->type &&
		IMGBLOCKTYPETAG::PU_VERT_SPLIT != block->type && 
		IMGBLOCKTYPETAG::ACTOMIC_BLOCK != block->type &&
		IMGBLOCKTYPETAG::PU_QUARTILE_SPLIT != block->type))
	{
		//++dfOffset;
		return;		// skip non-PU drawing
	}

	oldPen = pDC->SelectObject(&pen);
	cu = &(block->area);	

	pDC->MoveTo(cu->left, cu->top);
	pDC->LineTo(cu->right, cu->top);

	pDC->MoveTo(cu->left, cu->top);
	pDC->LineTo(cu->left, cu->bottom);

	pDC->MoveTo(cu->left, cu->bottom);
	pDC->LineTo(cu->right, cu->bottom);

	pDC->MoveTo(cu->right, cu->top);
	pDC->LineTo(cu->right, cu->bottom);

//
//	switch (drawFlag[dfOffset++])
//	{
//	case PartSize::SIZE_NxN:	// symmetric quartile
//		//Middle vertical
//		pDC->MoveTo((cu->left + cu->right) / 2, cu->top);
//		pDC->LineTo((cu->left + cu->right) / 2, cu->bottom); 
//
//		//Middle horizontal
//		pDC->MoveTo(cu->left, (cu->top + cu->bottom) / 2);
//		pDC->LineTo(cu->right, (cu->top + cu->bottom) / 2);
//		break;
//		
//	case PartSize::SIZE_2NxN:	// vertical equally segmentation
//		//Middle horizontal
//		pDC->MoveTo(cu->left, (cu->top + cu->bottom) / 2);
//		pDC->LineTo(cu->right, (cu->top + cu->bottom) / 2);
//		break;
//
//	case PartSize::SIZE_Nx2N:	// horizontal equally segmentation
//		//Middle vertical
//		pDC->MoveTo((cu->left + cu->right) / 2, cu->top);
//		pDC->LineTo((cu->left + cu->right) / 2, cu->bottom); 
//		break;
//
//#if AMP
//	case PartSize::SIZE_2NxnU:			// vertical upper segmentation
//		//Upper middle horizontal
//		pDC->MoveTo(cu->left, (cu->top + cu->bottom) / 2 - (cu->bottom - cu->top) / 4);
//		pDC->LineTo(cu->right, (cu->top + cu->bottom) / 2 - (cu->bottom - cu->top) / 4);
//		break;
//
//		case SIZE_2NxnD:			// vertical lower segmentation
//		//Lower middle horizontal
//		pDC->MoveTo(cu->left, (cu->top + cu->bottom) / 2 + (cu->bottom - cu->top) / 4);
//		pDC->LineTo(cu->right, (cu->top + cu->bottom) / 2 + (cu->bottom - cu->top) / 4);
//		break;
//
//		case SIZE_nLx2N:			// horizontal upper segmentation
//		//Upper middle vertical
//		pDC->MoveTo((cu->left + cu->right) / 2 - (cu->right - cu->left) / 4, cu->top);
//		pDC->LineTo((cu->left + cu->right) / 2 - (cu->right - cu->left) / 4, cu->bottom);
//		break;
//
//		case SIZE_nRx2N:			// horizontal lower segmentation
//		//Lower middle vertical
//		pDC->MoveTo((cu->left + cu->right) / 2 + (cu->right - cu->left) / 4, cu->top);
//		pDC->LineTo((cu->left + cu->right) / 2 + (cu->right - cu->left) / 4, cu->bottom);
//#endif
//
//	default:
//		break;
//		// do nothing
//	}

	pDC->SelectObject(oldPen);
}