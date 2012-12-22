#include "stdafx.h"
#include "DiffDrawer.h"


sysuVideo::DiffDrawer::DiffDrawer(void)
{
	enable = FALSE;

	penWidth = 2;
	penStyle = PS_SOLID;
	penColor = RGB(255, 0, 0);
	pen.CreatePen(penStyle, penWidth, penColor);
}


sysuVideo::DiffDrawer::~DiffDrawer(void)
{
	pen.DeleteObject();

	if (cmpStream != NULL)
		delete cmpStream;

	if (directStream != NULL)
		delete directStream;
}

inline sysuVideo::DRAWERTYPE sysuVideo::DiffDrawer::GetDrawerType() const
{
	return sysuVideo::DRAWERTYPE::DIFFDRAWER;
}

void sysuVideo::DiffDrawer::Init(LPWSTR filepath, int argCnt, ...)
{
	va_list ap;

	va_start(ap, argCnt);

	LPWSTR cmpFilePath = va_arg(ap, LPWSTR);

	if (NULL != directStream)
	{
		fclose(cmpStream);
		fclose(directStream);
	}

	if (0 != _wfopen_s(&cmpStream, cmpFilePath, _T("r")))
	{
		MessageBox(NULL, _T("cmp file open falied"), _T("DiffDrawer Init"), MB_OK);
		return;
	}

	if (0 != _wfopen_s(&directStream, filepath, _T("r")))
		return;

	//enable = TRUE;
	BuildIndex();
}

void sysuVideo::DiffDrawer::BuildIndex()
{
	static const int bufSize = 1024 * 3;
	static char buf[bufSize];
	static unsigned long frmCnt;
	static unsigned long curFrm;

	if (NULL == cmpStream || NULL == directStream)
		return;

	cmpStreamIndex.clear();

	frmCnt = -1;

	while (fgets(buf, bufSize, cmpStream) != NULL)
	{
		sscanf_s(buf, "<%lu,", &curFrm);

		if (curFrm != frmCnt)
		{
			cmpStreamIndex.push_back(ftell(cmpStream) - strlen(buf) - 1);
			frmCnt = curFrm;
		}
	}

	cmpIndexSize = cmpStreamIndex.size();

	//if (directStream == NULL)
	//	return;

	streamIndex.clear();

	frmCnt = -1;

	while (NULL != fgets(buf, bufSize, directStream))
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

void sysuVideo::DiffDrawer::Locale(unsigned long index)
{
	if (!enable)
		return;
	
	if (index >= indexSize)
	{
		MessageBox(NULL, _T("MV index overflow"), _T("Oops"), MB_OK);
		return;
	}

	fseek(directStream, streamIndex[index], SEEK_SET);
	fseek(cmpStream, cmpStreamIndex[index], SEEK_SET);
	curWorkingFrm = index;
	//getMVsForNextLCU();
}

void sysuVideo::DiffDrawer::Draw(sysuVideo::ImgBlock *block, CDC *pDC)
{
	static const int bufSize = 1024 * 3;
	static char buf[bufSize];
	static char buf1[bufSize];
	static unsigned long frmCnt, frmCnt1;
	static unsigned long curFrm, curFrm1;
	static int curLCUNum, lcu, lcu1;
	static CPen *oldPen;

	if (!enable)
		return;

	if (IMGBLOCKTYPETAG::CMD_FLAG == block->type)
	{
		curLCUNum = block->area.top;
		fgets(buf, bufSize, directStream); 
		fgets(buf1, bufSize, cmpStream);
		
		sscanf_s(buf, "<%lu, %d>", &frmCnt, &lcu);
		sscanf_s(buf1, "<%lu, %d>", &frmCnt1, &lcu1);
	}
	else if (IMGBLOCKTYPETAG::LCU == block->type)
	{
		

		if (frmCnt != curWorkingFrm || frmCnt1 != curWorkingFrm || lcu != curLCUNum)
		{
			MessageBox(NULL, _T("Indexing crash!"), _T("In DiffDrawer->Draw()"), MB_OK);
			return;
		}

		if (lcu != lcu1)
		{
			MessageBox(NULL, _T("#LCU mismatch in diff file!"), _T("In DiffDrawer->Draw()"), MB_OK);
			return;
		}

		if (0 != memcmp(buf, buf1, strlen(buf)))
		{
			oldPen = pDC->SelectObject(&pen);
			pDC->MoveTo(block->area.top, block->area.left);
			pDC->LineTo(block->area.top, block->area.right);
			pDC->MoveTo(block->area.top, block->area.left);
			pDC->LineTo(block->area.bottom, block->area.left);
			pDC->MoveTo(block->area.top, block->area.right);
			pDC->LineTo(block->area.bottom, block->area.right);
			pDC->MoveTo(block->area.bottom, block->area.left);
			pDC->LineTo(block->area.bottom, block->area.right);
			pDC->SelectObject(oldPen);
		}
	}
}