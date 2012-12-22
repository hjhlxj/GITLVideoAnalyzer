#include "stdafx.h"
#include "GITLVideoAnalyzer.h"
#include "BlockRelatedDrawer.h"


sysuVideo::GITLVideoAnalyzer::GITLVideoAnalyzer(void)
{
	bVideoOpen = FALSE;
}


sysuVideo::GITLVideoAnalyzer::~GITLVideoAnalyzer(void)
{
	if (NULL != pImgDecoBase)
		delete pImgDecoBase;

	if (NULL != pImgCmp)
		delete pImgCmp;

	if (NULL != pVReader)
		delete pVReader;
}

const CImage& sysuVideo::GITLVideoAnalyzer::GetCurrentFrame()
{
	if (!bVideoOpen)
		throw EXCEPTION_ACCESS_VIOLATION;

	/*const CImage &c = pVReader->GetCurFrame();

	if (!bDecoReady)
		return c;

	pImgDeco->Decorate((CImage*)&c, curWorkingFrmNum);*/
	return GetNthFrame(curWorkingFrmNum);
}

const CImage& sysuVideo::GITLVideoAnalyzer::GetPreviousFrame() 
{
	if (!bVideoOpen)
		throw EXCEPTION_ACCESS_VIOLATION;

	if (!pVReader->HasPreFrame())
		return GetCurrentFrame();

	const CImage& c = pVReader->GetPreFrame();
	
	if (!pImgDeco->IsReady())
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
	
	if (!pImgDeco->IsReady())
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
	
	if (!pImgDeco->IsReady())
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
	pImgDeco->ActivateDrawers(DRAWERTYPE::CUDRAWER, flag);
}

void sysuVideo::GITLVideoAnalyzer::ShowCUCost(BOOL flag)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowPU(BOOL flag)
{
	pImgDeco->ActivateDrawers(DRAWERTYPE::PUDRAWER, flag);
}

void sysuVideo::GITLVideoAnalyzer::ShowPUCost(BOOL flag)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowPUInfo(POINT position)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowMV(BOOL flag)
{
	pImgDeco->ActivateDrawers(DRAWERTYPE::MVDRAWER, flag);
}

void sysuVideo::GITLVideoAnalyzer::ShowDecisionMode(BOOL flag)
{
	pImgDeco->ActivateDrawers(DRAWERTYPE::MODEDECISIONDRAWER, flag);
}

BOOL sysuVideo::GITLVideoAnalyzer::SwitchDecorator()
{
	static BOOL bBase = TRUE;

	if (NULL == pImgCmp)
		return FALSE;
	
	pImgDeco = bBase ? pImgCmp : pImgDecoBase;
	bBase = !bBase;
	return TRUE;
}

void sysuVideo::GITLVideoAnalyzer::ShowCompareResult(BOOL flag)
{
	pImgDeco->ActivateDrawers(DRAWERTYPE::DIFFDRAWER, flag);
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
		/*pImgDecoBase = new BlockRelatedDrawer((CImage *)&(pVReader->GetCurFrame()),
					_T("d:/master/rc/decoder_cupu.txt"), _T("d:/master/rc/decoder_mv.txt"),
					_T("d:/master/rc/decoder_pred.txt"), _T("d:/master/rc/decoder_cupu.txt"),
					_T("d:/master/rc/decoder_cupu1.txt"));
		pImgCmp = new BlockRelatedDrawer((CImage *)&(pVReader->GetCurFrame()),
					_T("d:/master/rc/decoder_cupu1.txt"), _T("d:/master/rc/decoder_mv.txt"),
					_T("d:/master/rc/decoder_pred.txt"), _T("d:/master/rc/decoder_cupu1.txt"),
					_T("d:/master/rc/decoder_cupu.txt"));*/
		pImgDecoBase = new BlockRelatedDrawer((CImage *)&(pVReader->GetCurFrame()));
		pImgCmp = new BlockRelatedDrawer((CImage *)&(pVReader->GetCurFrame()));
		pImgDeco = pImgDecoBase;
		curWorkingFrmNum = 0;
		return TRUE;
	}
	else
		return FALSE;
}