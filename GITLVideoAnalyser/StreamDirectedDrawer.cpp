#include "stdafx.h"
#include "StreamDirectedDrawer.h"


sysuVideo::StreamDirectedDrawer::StreamDirectedDrawer(void)
{
}

sysuVideo::StreamDirectedDrawer::~StreamDirectedDrawer(void)
{
}

void sysuVideo::StreamDirectedDrawer::Disable(BOOL enableFlag = false)
{
	enable = enableFlag;
}

BOOL sysuVideo::StreamDirectedDrawer::IsEnalbe() const
{
	return enable;
}
	
COLORREF sysuVideo::StreamDirectedDrawer::GetPenColor() const 
{
	return penColor;
}

int sysuVideo::StreamDirectedDrawer::GetPenWidth() const 
{
	return penWidth;
}

int sysuVideo::StreamDirectedDrawer::GetPenStyle() const 
{
	return penStyle;
}

void sysuVideo::StreamDirectedDrawer::SetPenColor(COLORREF c) 
{
	penColor = c;
}

void sysuVideo::StreamDirectedDrawer::SetPenWidth(int pw)  
{
	penWidth = pw;
}

void sysuVideo::StreamDirectedDrawer::SetPenStyle(int ps)
{
	penStyle = ps;
}