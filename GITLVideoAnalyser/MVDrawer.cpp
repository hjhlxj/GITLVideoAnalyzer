#include "stdafx.h"
#include "MVDrawer.h"


sysuVideo::MVDrawer::MVDrawer(void)
{
	enable = FALSE;

	penWidth = 1;
	penStyle = PS_DASHDOTDOT;
	penColor = RGB(255, 255, 255);
	pen.CreatePen(penStyle, penWidth, penColor);
}


sysuVideo::MVDrawer::~MVDrawer(void)
{
}

void sysuVideo::MVDrawer::Init(LPWSTR filepath)
{
	if (0 != _wfopen_s(&directStream, filepath, _T("r")))
		return;

	enable = TRUE;
	BuildIndex();
}

void sysuVideo::MVDrawer::Draw(ImgBlcok *cu, CDC *pDC)
{
	static CPen *oldPen;
	static RECT curCU;

	if (!enable)
		return;
	
	oldPen = pDC->SelectObject(&pen);



	pDC->SelectObject(oldPen);
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
		return;

	fseek(directStream, streamIndex[index], SEEK_SET);
	curWorkingFrm = index;
}

void sysuVideo::MVDrawer::getMVsForNextLCU()
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
	pVSize = 0;

	token = strtok_s(NULL, " ", &nextToken);
	while (token != NULL && *token != '\n')
	{		
		tmp = atoi(token);
		pLCUVectors[pVSize] = (short)tmp;
		++pVSize;
		token = strtok_s(NULL, " ", &nextToken);
	}	

	pVOffset = 0;
}