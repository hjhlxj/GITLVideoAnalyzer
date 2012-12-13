#include "stdafx.h"
#include "MemoryDoubleBufferingShower.h"


sysuVideo::MemoryDoubleBufferingShower::MemoryDoubleBufferingShower(void)
{
	bufWidth = bufHeight = -1;
	hasBuf = FALSE;
}


sysuVideo::MemoryDoubleBufferingShower::~MemoryDoubleBufferingShower(void)
{
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}

BOOL sysuVideo::MemoryDoubleBufferingShower::ShowImage(CDC *pDC, int destx, int desty, int width, int height, const CImage& cimg)
{	
	if (width > bufWidth || height > bufHeight)
		if (!createBuffer(pDC, width, height))
			return FALSE;
	
	memDC.FillSolidRect(0, 0, bufWidth, bufHeight, RGB(255, 255, 255));
	
	cimg.StretchBlt(memDC.m_hDC, destx, desty, width, height,
					0, 0, cimg.GetWidth(), cimg.GetHeight());
	pDC->BitBlt(0, 0, bufWidth, bufHeight, &memDC, 0, 0, SRCCOPY);

	return TRUE;
}

BOOL sysuVideo::MemoryDoubleBufferingShower::createBuffer(CDC *pDC, int width, int height)
{
	if (hasBuf && !(memDC.DeleteDC() && memBitmap.DeleteObject()))
		return FALSE;
	
	if (!(memDC.CreateCompatibleDC(NULL) &&	memBitmap.CreateCompatibleBitmap(pDC, 2 * width, 2 * height)))
		return FALSE;
	
	bufWidth = 2 * width;
	bufHeight =  2 * width;
	hasBuf = TRUE;
	pOldBM = memDC.SelectObject(&memBitmap);
	return TRUE;
}