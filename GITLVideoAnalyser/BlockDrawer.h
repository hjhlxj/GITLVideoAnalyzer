#pragma once
#include "ImageDecorator.h"

namespace sysuVideo
{
	class BlockDrawer :
		public sysuVideo::ImageDecorator
	{
	public:
		explicit BlockDrawer(CImage * /* The target image to be decorated */);
		~BlockDrawer(void);

		virtual void AddParams(void *);
		virtual void Decorate(void * /* pointer to the image */, ...);

	protected:	//Helper function
		void drawBlockInfo(void);
	private:
		BOOL splitContinue(RECT * /* CU block */);
		void drawCU(RECT * /* CU block */, BOOL splitCU = FALSE);
		BOOL getNextLCU(RECT * /* LCU */);
		void localeCUInfo(void);
		BOOL reachAtomicSize(RECT * /* CU block */);
		void moveToCUInfoBeginLine(void);

	private:
		CImage imgLayout;
		CImage *imgBase;
		CDC *pDC;
		CPen pen;
		RECT curLCU;

		int LCUSIZE;

		//split related variables
		unsigned long workingFrameCnt;
		unsigned long frmCnt;
		int CUCnt;
		LPBYTE splitFlags;
		int sfcursor;
		int sflength;
		BOOL frameReset;

		int width;
		int height;
	};
}