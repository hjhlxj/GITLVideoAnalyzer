#include "stdafx.h"
#include "StreamDirectedDrawer.h"


sysuVideo::StreamDirectedDrawer::StreamDirectedDrawer(void)
{
	directStream = NULL;
	enable = FALSE;
}

sysuVideo::StreamDirectedDrawer::~StreamDirectedDrawer(void)
{
}

void sysuVideo::StreamDirectedDrawer::PreDrawingFrame(unsigned long frameCnt)
{
	Locale(frameCnt);
}

BOOL sysuVideo::StreamDirectedDrawer::IsEnable() const
{
	return NULL != directStream && enable;
}

void sysuVideo::StreamDirectedDrawer::Enable(BOOL flag)
{
	if (NULL == directStream)
		return;
	enable = flag;
}

//void sysuVideo::StreamDirectedDrawer::Disable(BOOL enableFlag = false)
//{
//	enable = enableFlag;
//}
//
//BOOL sysuVideo::StreamDirectedDrawer::IsEnalbe() const
//{
//	return enable;
//}
//	
//COLORREF sysuVideo::StreamDirectedDrawer::GetPenColor() const 
//{
//	return penColor;
//}
//
//int sysuVideo::StreamDirectedDrawer::GetPenWidth() const 
//{
//	return penWidth;
//}
//
//int sysuVideo::StreamDirectedDrawer::GetPenStyle() const 
//{
//	return penStyle;
//}
//
//BOOL sysuVideo::StreamDirectedDrawer::SetPenColor(COLORREF c) 
//{
//	penColor = c;	
//	return pen.DeleteObject() && pen.CreatePen(penStyle, penWidth, penColor);
//}
//
//BOOL sysuVideo::StreamDirectedDrawer::SetPenWidth(int pw)  
//{
//	penWidth = pw;
//	return pen.DeleteObject() && pen.CreatePen(penStyle, penWidth, penColor);
//}
//
//BOOL sysuVideo::StreamDirectedDrawer::SetPenStyle(int ps)
//{
//	penStyle = ps;
//	return pen.DeleteObject() && pen.CreatePen(penStyle, penWidth, penColor);
//}