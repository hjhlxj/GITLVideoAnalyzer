#include "stdafx.h"
#include "VideoReader.h"


sysuVideo::VideoReader::VideoReader(void)
{
	curFrameCnt = 0;
	height = 0;
	width = 0;
	frameCnt = 0;
	frameRate = 0;
	isStreamOpen = FALSE;
}


sysuVideo::VideoReader::~VideoReader(void)
{
	if (isStreamOpen)
		fclose(videoStream);
}

inline int sysuVideo::VideoReader::GetWidth() const
{
	return this->height;
}

inline void sysuVideo::VideoReader::SetWidth(int widthVal) 
{
	this->width = widthVal;
}

inline int sysuVideo::VideoReader::GetHeight() const
{
	return this->height;
}

inline void sysuVideo::VideoReader::SetHeight(int heightVal) 
{
	this->height = heightVal;
}

inline double sysuVideo::VideoReader::GetFrameRate() const
{
	return this->frameRate;
}

inline void sysuVideo::VideoReader::SetFrameRate(double frameRateVal) 
{
	this->frameRate = frameRateVal;
}

inline unsigned long sysuVideo::VideoReader::GetFrameCount() const 
{
	return this->frameCnt;
}

inline unsigned long sysuVideo::VideoReader::GetCurrentFrameNum() const 
{
	return this->curFrameCnt;
}

inline void sysuVideo::VideoReader::SetNBPP(int nBitsPerPixel)
{
	this->nbpp = nBitsPerPixel;
}

inline int sysuVideo::VideoReader::GetNBPP() const
{
	return this->nbpp;
}

BOOL sysuVideo::VideoReader::Open(LPCWSTR fileName) 
{	
	BOOL openSuccess = TRUE;
	
	if (0 != _wfopen_s(&videoStream, fileName, _T("rb")))
		openSuccess = FALSE;
	else	
		isStreamOpen = TRUE;	

	return openSuccess;
}

inline BOOL sysuVideo::VideoReader::IsVideoStreamOpen() const
{	
	return isStreamOpen;
}