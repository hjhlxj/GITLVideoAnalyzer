#include "stdafx.h"
#include "GITLVideoAnalyzer.h"
#include "BlockRelatedDrawer.h"


sysuVideo::GITLVideoAnalyzer::GITLVideoAnalyzer(void)
{
	bVideoOpen = bDecoReady = FALSE;
}


sysuVideo::GITLVideoAnalyzer::~GITLVideoAnalyzer(void)
{
}

const CImage& sysuVideo::GITLVideoAnalyzer::GetCurrentFrame() const
{
	if (!bVideoOpen)
		throw EXCEPTION_ACCESS_VIOLATION;

	const CImage &c = pVReader->GetCurFrame();

	if (!bDecoReady)
		return c;

	pImgDeco->Decorate((CImage*)&c, curWorkingFrmNum);
	return c;
}

const CImage& sysuVideo::GITLVideoAnalyzer::GetPreviousFrame() 
{
	if (!bVideoOpen)
		throw EXCEPTION_ACCESS_VIOLATION;

	if (!pVReader->HasPreFrame())
		return GetCurrentFrame();

	const CImage& c = pVReader->GetPreFrame();
	
	if (!bDecoReady)
		return c;

	pImgDeco->Decorate((CImage*)&c, --curWorkingFrmNum);
	return c;
}

const CImage& sysuVideo::GITLVideoAnalyzer::GetNextFrame() 
{
	if (!bVideoOpen)
		throw EXCEPTION_ACCESS_VIOLATION;

	if (!pVReader->HasNextFrame())
		return GetCurrentFrame();

	const CImage& c = pVReader->GetNextFrame();
	
	if (!bDecoReady)
		return c;

	pImgDeco->Decorate((CImage*)&c, ++curWorkingFrmNum);
	return c;
}

const CImage& sysuVideo::GITLVideoAnalyzer::GetNthFrame(unsigned long frmNum)
{
	if (!bVideoOpen)
		throw EXCEPTION_ACCESS_VIOLATION;

	if (!pVReader->HasNthFrame(frmNum))
		return GetCurrentFrame();

	const CImage& c = pVReader->GetNthFrame(frmNum);
	
	if (!bDecoReady)
		return c;

	pImgDeco->Decorate((CImage*)&c, frmNum);
	curWorkingFrmNum = frmNum;
	return c;
}

BOOL sysuVideo::GITLVideoAnalyzer::HasNextFrame() const
{
	return pVReader->HasNextFrame();
}

BOOL sysuVideo::GITLVideoAnalyzer::HasPreviousFrame() const
{
	return pVReader->HasPreFrame();
}

BOOL sysuVideo::GITLVideoAnalyzer::HasNthFrame(unsigned long n) const
{
	return FALSE;
}

unsigned long sysuVideo::GITLVideoAnalyzer::GetFrameCount() const
{
	return pVReader->GetFrameCount();
}

unsigned long sysuVideo::GITLVideoAnalyzer::GetCurrentFrameCount() const
{
	return pVReader->GetCurrentFrameNum();
}

int sysuVideo::GITLVideoAnalyzer::GetVideoWidth() const
{
	return pVReader->GetWidth();
}

int sysuVideo::GITLVideoAnalyzer::GetVideoHeight() const
{
	return pVReader->GetHeight();
}

void sysuVideo::GITLVideoAnalyzer::ShowCU(BOOL flag)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowCUCost(BOOL flag)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowPU(BOOL flag)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowPUCost(BOOL flag)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowPUInfo(POINT position)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowMV(BOOL flag)
{
}

void sysuVideo::GITLVideoAnalyzer::MagnifyCU(POINT position)
{
}

BOOL sysuVideo::GITLVideoAnalyzer::OpenAnalyticalFile(LPWSTR filepath)
{
	return TRUE;
}

BOOL sysuVideo::GITLVideoAnalyzer::OpenVideoFile(CString *filepath)
{
	pVReader = VideoReaderFactory::GetInstance().GetVideoReader(sysuVideo::VIDEOREADERTYPE::YUVREADER);
	if (pVReader->Init(filepath))
	{
		bVideoOpen = TRUE;
		pImgDeco = new BlockRelatedDrawer((CImage *)&(pVReader->GetCurFrame()));
		bDecoReady = TRUE;
		curWorkingFrmNum = 0;
		return TRUE;
	}
	else
		return FALSE;
}