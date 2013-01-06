#include "stdafx.h"
#include "YUVReader.h"
#include "BlockRelatedDrawer.h"
#include "VideoInformationDlg.h"

sysuVideo::YUVReader::YUVReader(void) :
	coeRV(TCOERV), coeGU(TCOEGU), coeGV(TCOEGV), coeBU(TCOEBU)
{
	/*coeRV = 1.13983;
	coeGU = -0.39465;
	coeGV = -0.58060;
	coeBU = 2.03211;*/
	nbpp = 32;
	Y = U = V = NULL;
	imgDeco = NULL;
}

sysuVideo::YUVReader::~YUVReader(void)
{
	frameBuf.Destroy();
	if (Y != NULL)
		delete [] Y;
	if (U != NULL)
		delete [] U;
	if (V != NULL)
		delete [] V;
	if (imgDeco != NULL)
		delete imgDeco;

}

void sysuVideo::YUVReader::constructFrame()
{
	int rowUV = width / 2, posUV, cnt = 0, bcnt = -1, pitch = frameBuf.GetPitch();
	LPBYTE bits = (LPBYTE)frameBuf.GetBits();
	int R, G, B;
	
	fread(Y, sizeof(BYTE), YCount, videoStream);
	fread(U, sizeof(BYTE), UCount, videoStream);
	fread(V, sizeof(BYTE), VCount, videoStream);

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			posUV = (i / 2) * rowUV + (j / 2);
			R = Y[cnt] + coeRV * (V[posUV] - 128);
			G = Y[cnt] + coeGU * (U[posUV] - 128) + coeGV * (V[posUV] - 128);
			B = Y[cnt] + coeBU * (U[posUV] - 128);
			//frameBuf.SetPixelRGB(j, i, R, G, B);
			R = min(255, max(0, R));
			G = min(255, max(0, G));
			B = min(255, max(0, B));
			bits[++bcnt] = B;
			bits[++bcnt] = G;
			bits[++bcnt] = R;
			bits[++bcnt] = 255;
			++cnt;
		}

		bcnt = -1;
		bits += pitch;
	}
}

CImage* sysuVideo::YUVReader::GetNextFrame() 
{	
	if (!HasNextFrame() || !isStreamOpen)
		throw EXCEPTION_ACCESS_VIOLATION;

	constructFrame();	
	++curFrameCnt;

	return GetCurFrame();
}

CImage* sysuVideo::YUVReader::GetCurFrame() 
{
	//CDC *pDCsrc, *pDCdst;

	//if (img == NULL)
	//	return FALSE;
	//if (img == &frameBuf)	//For initialization
	//	return TRUE;
	//if (!img->IsNull())
	//	img->Destroy();
	//img->Create(width, height, nbpp);
	//
	//pDCsrc = CDC::FromHandle(frameBuf.GetDC());
	//pDCdst = CDC::FromHandle(img->GetDC());
	//pDCdst->BitBlt(0, 0, frameBuf.GetWidth(), frameBuf.GetHeight(), pDCsrc, 0, 0, SRCCOPY);
	//
	//frameBuf.ReleaseDC();
	//img->ReleaseDC();
	//memcpy(img, &frameBuf, sizeof(frameBuf));--

	//imgDeco->Decorate(&frameBuf, (int)curFrameCnt);
	return &frameBuf;
}

CImage* sysuVideo::YUVReader::GetPreFrame() 
{	
	if (!HasPreFrame() || !isStreamOpen)
		throw EXCEPTION_ACCESS_VIOLATION;

	fseek(videoStream, -2 * (YCount + UCount + VCount), SEEK_CUR);
	constructFrame();
	--curFrameCnt;

	return GetCurFrame();
}

CImage* sysuVideo::YUVReader::GetNthFrame(unsigned long frmNum)
{
	if (!HasNthFrame(frmNum) || !isStreamOpen)
		throw EXCEPTION_ACCESS_VIOLATION;

	fseek(videoStream, frmNum * (YCount + UCount + VCount), SEEK_SET);
	constructFrame();
	curFrameCnt = frmNum;

	return GetCurFrame();
}

BOOL sysuVideo::YUVReader::Init(LPVOID initInfo)
{
	CString *filepath;
	long filesize;

	if (filepath = dynamic_cast<CString*>((CString*)initInfo))
	{
		if (!InitViaFilepath(*filepath))
		{
			if (!InitViaInteraction())
				return FALSE;
		}
		else
		{
			LPCWSTR filename = filepath->GetBuffer();
			Open(filename);
			filepath->ReleaseBuffer();
		}
	}

	frameBuf.Create(width, height, nbpp, CImage::createAlphaChannel); 
	YCount = width * height;
	UCount = VCount = YCount / 4;
	Y = new BYTE[YCount];
	U = new BYTE[UCount];
	V = new BYTE[VCount];
	
	fseek(videoStream, 0L, SEEK_END);
	filesize = ftell(videoStream);
	fseek(videoStream, 0L, SEEK_SET);

	frameCnt = filesize / (YCount + UCount + VCount);

	//imgDeco = new BlockRelatedDrawer(&frameBuf);

	curFrameCnt = 0;
	constructFrame();	//Ready for show

	return TRUE;
}

BOOL sysuVideo::YUVReader::InitViaFilepath(CString filepath)
{
	CString fileName = filepath.Right(filepath.GetLength() - filepath.ReverseFind('\\') - 1);
	fileName = fileName.Left(fileName.Find(_T(".yuv")));
		
	CString strFrmRate = fileName.Right(fileName.GetLength() - fileName.ReverseFind('_') - 1);
	fileName = fileName.Left(fileName.ReverseFind('_'));
	fileName = fileName.Right(fileName.GetLength() - fileName.ReverseFind('_') - 1);

	CString strWidth = fileName.Left(fileName.Find(_T("x"))),
		    strHeight = fileName.Right(fileName.GetLength() - fileName.Find(_T("x")) - 1);
	
	width = _wtoi(strWidth);
	height = _wtoi(strHeight);
	frameRate = _wtoi(strFrmRate);

	if (width == 0 || height == 0 || frameRate == 0)
		return FALSE;
	else return TRUE;
}

BOOL sysuVideo::YUVReader::InitViaInteraction()
{
	VideoInformationDlg vid;

	if (IDOK != vid.DoModal())
		return FALSE;

	width = vid.m_VWidth;
	height = vid.m_VHeight;

	return TRUE;
}

void sysuVideo::YUVReader::Save(LPCWSTR filepath, LPVOID saveInfo)
{
	if (!isStreamOpen)
		return;
	unsigned cfn = curFrameCnt;
	int rowUV = width / 2, posUV, cnt = 0, bcnt = -1, pitch = frameBuf.GetPitch(), i, j;
	LPBYTE bits = (LPBYTE)frameBuf.GetBits();
	int R, G, B;
	double *y = new double[YCount],
		   *u = new double[UCount],
		   *v = new double[VCount];

	ZeroMemory(y, YCount * sizeof(double));
	ZeroMemory(u, UCount * sizeof(double));
	ZeroMemory(v, VCount * sizeof(double));

	FILE *outputFile;

	if (0 != _wfopen_s(&outputFile, filepath, _T("wb"))) {
		MessageBox(NULL, _T("Can't not open file for writing"), _T("Operation Failed"), MB_ICONERROR);
		return;
	}

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

	delete [] y;
	delete [] u;
	delete [] v;
}

BOOL sysuVideo::YUVReader::HasNextFrame() const
{
	return curFrameCnt < frameCnt;
}

BOOL sysuVideo::YUVReader::HasPreFrame() const
{
	return curFrameCnt > 0;
}

BOOL sysuVideo::YUVReader::HasNthFrame(unsigned long frmNum) const
{
	return frmNum < frameCnt && frmNum >= 0;
}