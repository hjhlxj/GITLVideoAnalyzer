#include "stdafx.h"
#include "BlockDrawer.h"
#include <cstdarg>

sysuVideo::BlockDrawer::BlockDrawer(CImage *ci)
{
	imgLayout.Create(ci->GetWidth(), ci->GetHeight(), 32, CImage::createAlphaChannel);
	/*if (imgLayout.GetPitch() < 0)
		memset((LPBYTE *)imgLayout.GetBits() + (imgLayout.GetHeight() - 1) * imgLayout.GetPitch(),
			0xFF, 
			imgLayout.GetHeight() * imgLayout.GetWidth());*/
}


sysuVideo::BlockDrawer::~BlockDrawer(void)
{
}

void sysuVideo::BlockDrawer::addParams(void *)
{
	MessageBox(NULL, _T("Not implemented"), _T("Not implemented"), MB_OK);
}

void sysuVideo::BlockDrawer::decorate(void *img, ...)
{
	va_list ap;
	int type;
	static TCHAR buf[250];
	CImage *pcimg = dynamic_cast<CImage *>((CImage *)img);
	va_start(ap, img);
	type = va_arg(ap, int);

	wsprintf(buf, _T("arg = %d"), type);
	MessageBox(NULL, buf, _T("Argument validation"), MB_OK);

	HDC hdc = imgLayout.GetDC();
	CDC *pDC = new CDC;
	pDC->Attach(hdc);
	CPen whitePen(PS_SOLID, 2, RGB(255, 255, 255));
	CPen *oldPen = pDC->SelectObject(&whitePen);

	pDC->MoveTo(imgLayout.GetWidth() / 2, 0);
	pDC->LineTo(imgLayout.GetWidth() / 2, imgLayout.GetHeight()); 
	pDC->MoveTo(0, imgLayout.GetHeight() / 2);
	pDC->LineTo(imgLayout.GetWidth(), imgLayout.GetHeight() / 2);

	DeleteObject(pDC->SelectObject(oldPen));
	pDC->Detach();
	imgLayout.ReleaseDC();

	//imgLayout.BitBlt(pcimg->GetDC(), 0, 0);
	imgLayout.TransparentBlt(pcimg->GetDC(), 0, 0, pcimg->GetWidth(), pcimg->GetHeight());
	pcimg->ReleaseDC();
}
