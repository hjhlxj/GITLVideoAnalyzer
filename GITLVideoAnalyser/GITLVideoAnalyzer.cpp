#include "stdafx.h"
#include "GITLVideoAnalyzer.h"
#include "BlockRelatedDrawer.h"


sysuVideo::GITLVideoAnalyzer::GITLVideoAnalyzer(void)
{
	bVideoOpen = FALSE;
	pImgDecoBase = pImgCmp = pImgDeco = NULL;
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
	return pVReader->HasNthFrame(n);
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

BOOL sysuVideo::GITLVideoAnalyzer::SaveVideo(CString *filepath)
{
	if (!bVideoOpen)
		return FALSE;

	const CImage& c = GetNthFrame(0);
	int rowUV = GetVideoWidth() / 2, posUV, cnt = 0, bcnt = -1, pitch = c.GetPitch(), i, j;
	LPBYTE bits;
	int R, G, B,  width = c.GetWidth(), height = c.GetHeight(), YCount, UCount, VCount;
	YCount = width * height;
	UCount = VCount = YCount / 4;
	double *y = new double[YCount],
		   *u = new double[UCount],
		   *v = new double[VCount];
	LPBYTE Y = new BYTE[YCount],
		   U = new BYTE[UCount],
		   V = new BYTE[VCount];

	FILE *outputFile;

	if (0 != _wfopen_s(&outputFile, filepath->GetBuffer(), _T("wb"))) {
		MessageBox(NULL, _T("Can't not open file for writing"), _T("Operation Failed"), MB_ICONERROR);
		filepath->ReleaseBuffer();
		return FALSE;
	}
	filepath->ReleaseBuffer();

	for (unsigned long frmc = 0; frmc < pVReader->GetFrameCount(); ++frmc)
	{
		const CImage& cp = GetNthFrame(frmc);
		cnt = 0;
		bcnt = -1;
		bits = (LPBYTE)cp.GetBits();
		ZeroMemory(y, YCount * sizeof(double));
		ZeroMemory(u, UCount * sizeof(double));
		ZeroMemory(v, VCount * sizeof(double));
		for (i = 0; i < height; ++i)
		{
			for (j = 0; j < width; ++j)
			{
				posUV = (i / 2) * rowUV + (j / 2);
				//R = Y[cnt] + coeRV * (V[posUV] - 128);
				//G = Y[cnt] + coeGU * (U[posUV] - 128) + coeGV * (V[posUV] - 128);
				//B = Y[cnt] + coeBU * (U[posUV] - 128);
				////frameBuf.SetPixelRGB(j, i, R, G, B);
				//R = min(255, max(0, R));
				//G = min(255, max(0, G));
				//B = min(255, max(0, B));
				//bits[++bcnt] = B;
				//bits[++bcnt] = G;
				//bits[++bcnt] = R;
				//bits[++bcnt] = 255;

				R = bits[++bcnt];
				G = bits[++bcnt];
				B = bits[++bcnt];
				++bcnt;	//skip transparent bits

				y[cnt] = .299 * R + .587 * G + .114 * B;
				u[posUV] += (-.14713 * R - .28886 * G + .436 * B);
				v[posUV] += (.615 * R - .51499 * G -.10001 * B);
				++cnt;
			}

			bcnt = -1;
			bits += pitch;
		}

		for (i = 0; i < YCount; ++i)
			Y[i] = y[i] + 128.5;
		for (i = 0; i < UCount; ++i)
		{
			U[i] = u[i] / 4 + 128.5;
			V[i] = v[i] / 4 + 128.5;
		}

		fwrite(Y, sizeof(BYTE), YCount, outputFile);
		fwrite(U, sizeof(BYTE), UCount, outputFile);
		fwrite(V, sizeof(BYTE), VCount, outputFile);
	}

	fclose(outputFile);
	delete [] y;
	delete [] u;
	delete [] v;
	delete [] Y;
	delete [] U;
	delete [] V;

	return TRUE;
}