#include "stdafx.h"
#include "GITLVideoAnalyzer.h"


sysuVideo::GITLVideoAnalyzer::GITLVideoAnalyzer(void)
{
}


sysuVideo::GITLVideoAnalyzer::~GITLVideoAnalyzer(void)
{
}

const CImage& sysuVideo::GITLVideoAnalyzer::GetCurrentFrame() const
{
	CImage c;
	return c;
}

const CImage& sysuVideo::GITLVideoAnalyzer::GetPreviousFrame() 
{
	CImage c;
	return c;
}

const CImage& sysuVideo::GITLVideoAnalyzer::GetNextFrame() 
{
	CImage c;
	return c;
}

const CImage& sysuVideo::GITLVideoAnalyzer::GetNthFrame()
{
	CImage c;
	return c;
}

BOOL sysuVideo::GITLVideoAnalyzer::HasNextFrame() const
{
	return TRUE;
}

BOOL sysuVideo::GITLVideoAnalyzer::HasPreviousFrame() const
{
	return TRUE;
}

BOOL sysuVideo::GITLVideoAnalyzer::HasNthFrame(unsigned long n) const
{
	return TRUE;
}

unsigned long sysuVideo::GITLVideoAnalyzer::GetFrameCount() const
{
	return 0;
}

unsigned long sysuVideo::GITLVideoAnalyzer::GetCurrentFrameCount() const
{
	return 0;
}

int sysuVideo::GITLVideoAnalyzer::GetVideoWidth() const
{
	return 0;
}

int sysuVideo::GITLVideoAnalyzer::GetVideoHeight() const
{
	return 0;
}

void sysuVideo::GITLVideoAnalyzer::ShowCU(BOOL flag)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowCUCost(BOOL flag)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowPU(BOOL flag)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowPUCost(BOOL flag)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowPUInfo(POINT position)
{
}

void sysuVideo::GITLVideoAnalyzer::ShowMV(BOOL flag)
{
}

void sysuVideo::GITLVideoAnalyzer::MagnifyCU(POINT position)
{
}

BOOL sysuVideo::GITLVideoAnalyzer::OpenAnalyticalFile(LPWSTR filepath)
{
	return TRUE;
}

BOOL sysuVideo::GITLVideoAnalyzer::OpenVideoFile(LPWSTR filepath)
{
	return TRUE;
}