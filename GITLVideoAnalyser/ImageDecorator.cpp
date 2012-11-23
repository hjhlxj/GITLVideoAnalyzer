#include "stdafx.h"
#include "ImageDecorator.h"

void sysuVideo::ImageDecorator::Disable(BOOL enableFlag = false)
{
	enable = enableFlag;
}

void sysuVideo::ImageDecorator::Bind(FILE *file)
{
	cuInfoStream = file;
}

BOOL sysuVideo::ImageDecorator::IsEnalbe() const
{
	return enable;
}
	
COLORREF sysuVideo::ImageDecorator::GetPenColor() const 
{
	return penColor;
}

int sysuVideo::ImageDecorator::GetPenWidth() const 
{
	return penWidth;
}

int sysuVideo::ImageDecorator::GetPenStyle() const 
{
	return penStyle;
}

void sysuVideo::ImageDecorator::SetPenColor(COLORREF c) 
{
	penColor = c;
}

void sysuVideo::ImageDecorator::SetPenWidth(int pw)  
{
	penWidth = pw;
}

void sysuVideo::ImageDecorator::SetPenStyle(int ps)
{
	penStyle = ps;
}