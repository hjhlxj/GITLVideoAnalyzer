#include "stdafx.h"
#include "VideoReader.h"
#include "YUVReader.h"

sysuVideo::VideoReaderFactory::VideoReaderFactory(void)
{
}


sysuVideo::VideoReaderFactory::~VideoReaderFactory(void)
{
}

BOOL sysuVideo::VideoReaderFactory::hasInstance = FALSE;
sysuVideo::VideoReaderFactory *sysuVideo::VideoReaderFactory::facInstance = NULL;

sysuVideo::VideoReaderFactory& sysuVideo::VideoReaderFactory::GetInstance()
{
	if (hasInstance)
	{
		return *facInstance;
	}
	else
	{
		facInstance = new sysuVideo::VideoReaderFactory();
		hasInstance = TRUE;
		return *facInstance;
	}
}

sysuVideo::VideoReader* sysuVideo::VideoReaderFactory::GetVideoReader(sysuVideo::VIDEOREADERTYPE vType)
{
	VideoReader *readerPtr = NULL;

	switch(vType)
	{
	case sysuVideo::VIDEOREADERTYPE::YUVREADER:
		readerPtr = new sysuVideo::YUVReader();
		break;
	case sysuVideo::VIDEOREADERTYPE::DEFAULTREADER:
		readerPtr = readerPtr = new sysuVideo::YUVReader();
		break;
	default:
		break;
	}

	return readerPtr;
}