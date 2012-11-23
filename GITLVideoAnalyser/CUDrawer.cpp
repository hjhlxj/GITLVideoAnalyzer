#include "stdafx.h"
#include "CUDrawer.h"


sysuVideo::CUDrawer::CUDrawer(void)
{
	enable = FALSE;

	penWidth = 1;
	penStyle = PS_DASHDOTDOT;
	penColor = RGB(255, 255, 255);
	pen.CreatePen(penStyle, penWidth, penColor);
}


sysuVideo::CUDrawer::~CUDrawer(void)
{
}

void sysuVideo::CUDrawer::Init(LPWSTR filepath)
{
	if (0 != _wfopen_s(&directStream, filepath, _T("r")))
		return;

	enable = TRUE;
	BuildIndex();
}

void sysuVideo::CUDrawer::BuildIndex()
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

	dfOffset = 0;
	dfSize = -1;
}

void sysuVideo::CUDrawer::Locale(unsigned long index)
{
	if (!enable)
		return;
	
	if (index >= indexSize)
		return;

	fseek(directStream, streamIndex[index], SEEK_SET);
	curWorkingFrm = index;
}

void sysuVideo::CUDrawer::Draw(RECT *cu, CDC *pDC)
{
	static CPen *oldPen;
	static RECT curCU;

	if (!enable)
		return;
	
	oldPen = pDC->SelectObject(&pen);

	if (dfOffset >= dfSize)
		readNextLCUDrawingFlag();

	if (drawFlag[dfOffset++] == 99)	//Split need
	{		
		//Middle vertical
		pDC->MoveTo((cu->left + cu->right) / 2, cu->top);
		pDC->LineTo((cu->left + cu->right) / 2, cu->bottom); 

		//Middle horizontal
		pDC->MoveTo(cu->left, (cu->top + cu->bottom) / 2);
		pDC->LineTo(cu->right, (cu->top + cu->bottom) / 2);
	}

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

	pDC->SelectObject(oldPen);
}

void sysuVideo::CUDrawer::readNextLCUDrawingFlag()
{
	static const int bufSize = 1024 * 3;
	static int tmp;
	static char buf[bufSize];
	static unsigned long frmCnt;
	static unsigned long curFrm;
	static char *token, *nextToken;

	fgets(buf, bufSize, directStream);
	token = strtok_s(buf, " ", &nextToken);
	sscanf_s(token, "<%lu,%lu>", &frmCnt, &curFrm);
	dfSize = 0;

	token = strtok_s(NULL, " ", &nextToken);
	while (token != NULL && *token != '\n')
	{		
		tmp = atoi(token);
		drawFlag[dfSize] = (BYTE)tmp;
		++dfSize;
		token = strtok_s(NULL, " ", &nextToken);
	}	

	dfOffset = 0;
}