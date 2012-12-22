#include "stdafx.h"
#include "DrawerBase.h"


sysuVideo::DrawerBase::DrawerBase(void)
{
}


sysuVideo::DrawerBase::~DrawerBase(void)
{
}

void sysuVideo::DrawerBase::PreDrawingFrame(unsigned long /* #frame to be draw */)
{
	//Do nothing here
}

void sysuVideo::DrawerBase::Init(LPWSTR /* filepath */, int /*#num arg*/, ...)
{
	//Do nothing here
}

void sysuVideo::DrawerBase::Init(int /*#num arg*/, ...)
{
	//Do nothing here
}

void sysuVideo::DrawerBase::Enable(BOOL enableFlag = false)
{
	enable = enableFlag;
}

BOOL sysuVideo::DrawerBase::IsEnable() const
{
	return enable;
}
	
COLORREF sysuVideo::DrawerBase::GetPenColor() const 
{
	return penColor;
}

int sysuVideo::DrawerBase::GetPenWidth() const 
{
	return penWidth;
}

int sysuVideo::DrawerBase::GetPenStyle() const 
{
	return penStyle;
}

BOOL sysuVideo::DrawerBase::SetPenColor(COLORREF c) 
{
	penColor = c;	
	return pen.DeleteObject() && pen.CreatePen(penStyle, penWidth, penColor);
}

BOOL sysuVideo::DrawerBase::SetPenWidth(int pw)  
{
	penWidth = pw;
	return pen.DeleteObject() && pen.CreatePen(penStyle, penWidth, penColor);
}

BOOL sysuVideo::DrawerBase::SetPenStyle(int ps)
{
	penStyle = ps;
	return pen.DeleteObject() && pen.CreatePen(penStyle, penWidth, penColor);
}