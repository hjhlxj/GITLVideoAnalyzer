#include "stdafx.h"
#include "MVDrawer.h"


sysuVideo::MVDrawer::MVDrawer(void)
{
	enable = FALSE;

	penWidth = 1;
	penStyle = PS_SOLID;
	penColor = RGB(0, 255, 0);
	pen.CreatePen(penStyle, penWidth, penColor);

	pPUVectors = new short[1000];
}


sysuVideo::MVDrawer::~MVDrawer(void)
{
	if (nullptr != pPUVectors)
		delete [] pPUVectors;
}

void sysuVideo::MVDrawer::Init(LPWSTR filepath)
{
	if (0 != _wfopen_s(&directStream, filepath, _T("r")))
		return;

	enable = TRUE;
	BuildIndex();
}

void sysuVideo::MVDrawer::Draw(ImgBlock *block, CDC *pDC)
{
	static CPen *oldPen;
	static RECT curCU;
	static POINT mv;

	if (!enable)
		return;

	if (IMGBLOCKTYPETAG::CMD_FLAG == block->type)
	{
		getMVsForNextLCU();
		return;
	}

	if ((IMGBLOCKTYPETAG::PU_HORZ_SPLIT != block->type &&
		IMGBLOCKTYPETAG::PU_VERT_SPLIT != block->type && 
		IMGBLOCKTYPETAG::ACTOMIC_BLOCK != block->type &&
		IMGBLOCKTYPETAG::PU_QUARTILE_SPLIT != block->type))
		return;			// Motion vector is for PU only

	if (pVOffset >= pVSize - 1)
	{
		MessageBox(NULL, _T("MV buffer overrun"), _T("Oops"), MB_OK);
		return; //getMVsForNextLCU();
	}

	oldPen = pDC->SelectObject(&pen);	

	switch (pPUVectors[++pVOffset])
	{
	// No motion vector case
	case 0:
		break;

	// One motion vector cases
	case 1:	
	case 2:
		mv.x = pPUVectors[++pVOffset];
		mv.y = pPUVectors[++pVOffset];
		drawVector(&(block->area), mv, pDC);
		break;

	// Two motion vector case
	case 3:
		mv.x = pPUVectors[++pVOffset];
		mv.y = pPUVectors[++pVOffset];
		drawVector(&(block->area), mv, pDC);
		
		mv.x = pPUVectors[++pVOffset];
		mv.y = pPUVectors[++pVOffset];
		drawVector(&(block->area), mv, pDC);
		break;

	default:
		break;
	}

	pDC->SelectObject(oldPen);
}

void sysuVideo::MVDrawer::drawVector(RECT *pu, POINT coordinate, CDC *pDC)
{
	static POINT logicalOrigin;		// The central point of the pu is treated as the logical origin of the motion vector
	static POINT mvTrans;	// The transformation of the coordinate (logical -> device)
		
	logicalOrigin.x = (pu->left + pu->right) / 2;
	logicalOrigin.y = (pu->bottom + pu->top) / 2;

	mvTrans = lpToDp(pu, logicalOrigin, coordinate);
	pDC->MoveTo(logicalOrigin);
	pDC->LineTo(mvTrans);
}

void sysuVideo::MVDrawer::BuildIndex()
{
	static const int bufSize = 1024 * 3;
	static char buf[bufSize];
	static unsigned long frmCnt;
	static unsigned long curFrm;

	if (directStream == NULL)
		return;

	streamIndex.clear();

	frmCnt = -1;

	while (fgets(buf, bufSize, directStream) != NULL)
	{
		sscanf_s(buf, "<%lu,", &curFrm);

		if (curFrm != frmCnt)
		{
			streamIndex.push_back(ftell(directStream) - strlen(buf) - 1);
			frmCnt = curFrm;
		}
	}

	indexSize = streamIndex.size();
}

void sysuVideo::MVDrawer::Locale(unsigned long index)
{
	if (!enable)
		return;
	
	if (index >= indexSize)
	{
		MessageBox(NULL, _T("MV index overflow"), _T("Oops"), MB_OK);
		return;
	}

	fseek(directStream, streamIndex[index], SEEK_SET);
	curWorkingFrm = index;
	//getMVsForNextLCU();
}

void sysuVideo::MVDrawer::getMVsForNextLCU()
{
	static const int bufSize = 1024 * 3;
	static int tmp;
	static char buf[bufSize];
	static unsigned long frmCnt;
	static unsigned long curlcu;
	static char *token, *nextToken;

	fgets(buf, bufSize, directStream);
	token = strtok_s(buf, " ", &nextToken);
	sscanf_s(token, "<%lu,%lu>", &frmCnt, &curlcu);
	
	if (frmCnt != curWorkingFrm)
	{
		TCHAR sb[100];
		swprintf_s(sb, _T("curFrm %d, working Frm %d"), frmCnt, curWorkingFrm);
		MessageBox(NULL, sb, _T("MV"), MB_OK);
	}

	pVSize = 0;
	 
	token = strtok_s(NULL, " ", &nextToken);
	while (token != NULL && *token != '\n')
	{		
		tmp = atoi(token);
		pPUVectors[pVSize] = (short)tmp;
		++pVSize;
		token = strtok_s(NULL, " ", &nextToken);
	}	

	pVOffset = -1;
}

POINT sysuVideo::MVDrawer::lpToDp(RECT *refPU, POINT &lorg, POINT& coordinate)
{
	static POINT transResult;
	static int radius;		// Radius of the pu
	static double x, y;		// Round x and y coordinate
	static int tx, ty;

	if (0 == coordinate.x && 0 == coordinate.y)
	{
		transResult.x = lorg.x;
		transResult.y = lorg.y;
	}
	else
	{
		LongToInt(coordinate.x, &tx);
		LongToInt(coordinate.y, &ty);
		x = (double)tx / 4;
		y = (double)ty / 4;

		/*
		radius = min(refPU->bottom - refPU->top, refPU->right - refPU->left) / 2;
		x = 2 * x + .2;
		y = 2 * y + .2;

		transResult.x = (1 - exp(-1.5 * x * x)) * radius;
		transResult.y = (1 - exp(-1.5 * y * y)) * radius;

		if (abs(x) < 1 && abs(y) < 1)
		{
			transResult.x = transResult.y = 1;
		}				*/

		transResult.x = x;
		transResult.y = y;

		if (abs(transResult.x) < 1 && abs(transResult.y) < 1)
		{
			transResult.x = transResult.y = 1;			
		}
		// Adjust 
		/*transResult.x = min(radius, transResult.x);
		transResult.y = min(radius, transResult.y);*/
		transResult.x = coordinate.x > 0 ? transResult.x : -transResult.x;			
		transResult.y = coordinate.y > 0 ? transResult.y : -transResult.y;
		transResult.x += lorg.x;
		transResult.y += lorg.y;
	}

	return transResult;
}