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
		void drawBlockInfo();
	private:
		BOOL splitContinue();
		void drawCU(RECT * /* CU block */);
		BOOL getNextLCU(RECT * /* LCU */);

	private:
		CImage imgLayout;
		CImage *imgBase;
		CDC *pDC;
		CPen pen;
		int penStyle;
		int penWidth;
		COLORREF penColor;
		RECT curLCU;

		int LCUSIZE;
	};
}