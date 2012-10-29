#include "stdafx.h"
#include "YUVReader.h"


sysuVideo::YUVReader::YUVReader(void) :
	coeRV(1.13983), coeGU(-0.39465), coeGV(-0.58060), coeBU(2.03211)
{
	/*coeRV = 1.13983;
	coeGU = -0.39465;
	coeGV = -0.58060;
	coeBU = 2.03211;*/
	nbpp = 24;
	Y = U = V = NULL;
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
}

BOOL sysuVideo::YUVReader::GetNextFrame(CImage *img) 
{	
	if (img == NULL || !HasNextFrame() || !isStreamOpen)
		return FALSE;

	int rowUV = width / 2, posUV, cnt = 0;
	BYTE R, G, B;

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
			frameBuf.SetPixelRGB(j, i, R, G, B);
			++cnt;
		}
	}

	++curFrameCnt;

	return GetCurFrame(img);
}

BOOL sysuVideo::YUVReader::GetCurFrame(CImage *img) 
{
	CDC *pDCsrc, *pDCdst;

	if (img == NULL)
		return FALSE;
	if (img == &frameBuf)	//For initialization
		return TRUE;
	if (!img->IsNull())
		img->Destroy();
	img->Create(width, height, nbpp);
	
	pDCsrc = CDC::FromHandle(frameBuf.GetDC());
	pDCdst = CDC::FromHandle(img->GetDC());
	pDCdst->BitBlt(0, 0, frameBuf.GetWidth(), frameBuf.GetHeight(), pDCsrc, 0, 0, SRCCOPY);
	
	frameBuf.ReleaseDC();
	img->ReleaseDC();
	//memcpy(img, &frameBuf, sizeof(frameBuf));--
	return TRUE;
}

BOOL sysuVideo::YUVReader::GetPreFrame(CImage *img) 
{	
	if (img == NULL || !HasPreFrame() || !isStreamOpen)
		return FALSE;

	int rowUV = width / 2, posUV, cnt = 0;
	BYTE R, G, B;

	int tmp = fseek(videoStream, -2 * (YCount + UCount + VCount), SEEK_CUR);

	if (0 == fread(Y, sizeof(BYTE), YCount, videoStream));
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
			frameBuf.SetPixelRGB(j, i, R, G, B);
			++cnt;
		}
	}

	--curFrameCnt;
	return GetCurFrame(img);
}

BOOL sysuVideo::YUVReader::Init(LPVOID initInfo)
{
	CString *filepath;
	long filesize;

	if (filepath = dynamic_cast<CString*>((CString*)initInfo))
	{
		if (!InitViaFilepath(*filepath))
			return FALSE;
		else
		{
			LPCWSTR filename = filepath->GetBuffer();
			Open(filename);
			filepath->ReleaseBuffer();
		}
	}

	frameBuf.Create(width, height, nbpp); 
	YCount = width * height;
	UCount = VCount = YCount / 4;
	Y = new BYTE[YCount];
	U = new BYTE[UCount];
	V = new BYTE[VCount];
	
	fseek(videoStream, 0L, SEEK_END);
	filesize = ftell(videoStream);
	fseek(videoStream, 0L, SEEK_SET);

	frameCnt = filesize / (YCount + UCount + VCount);

	GetNextFrame(&frameBuf);	//Ready for show
	curFrameCnt = 0;

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

void sysuVideo::YUVReader::Save(LPCWSTR filepath, LPVOID saveInfo)
{
}

BOOL sysuVideo::YUVReader::HasNextFrame() const
{
	return this->curFrameCnt < this->frameCnt;
}

BOOL sysuVideo::YUVReader::HasPreFrame() const
{
	return this->curFrameCnt > 0;
}