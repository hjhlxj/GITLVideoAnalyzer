#pragma once
#include "ImageDecorator.h"
#include "StreamDirectedDrawer.h"
#include "BlockSequenceManager.h"
#include <list>
#include <vector>

namespace sysuVideo
{
	class BlockRelatedDrawer :
		public sysuVideo::ImageDecorator
	{
	public:
		explicit BlockRelatedDrawer(CImage * /* The target image to be decorated */);
		~BlockRelatedDrawer(void);

		virtual void AddParams(void *) override;
		virtual void Decorate(void * /* pointer to the image */, ...) override;

	protected:	//Helper function
		void drawBlockInfo(void);
	//private:
	//	BOOL splitContinue(RECT * /* CU block */);
	//	void drawCU(RECT * /* CU block */, BOOL splitCU = FALSE);
	//	BOOL getNextLCU(RECT * /* LCU */);
	//	void localeCUInfo(void);
	//	BOOL reachAtomicSize(RECT * /* CU block */);
	//	void moveToCUInfoBeginLine(void);
	//	void buildFileOffsetIndex(LPWSTR /* File path */);

	private:
		CImage imgLayout;
		CImage *imgBase;

		BlockSequenceManager *bsmgr;
		std::list<StreamDirectedDrawer*> activeDrawers;
		std::vector<StreamDirectedDrawer*> drawers;

		unsigned long workingFrameCnt;
		//CDC *pDC;
		//CPen pen;
		//RECT curLCU;

		//int LCUSIZE;

		////split related variables
		//unsigned long workingFrameCnt;
		//unsigned long frmCnt;
		//int CUCnt;
		//LPBYTE splitFlags;
		//int sfcursor;
		//int sflength;
		//BOOL frameReset;

		//int width;
		//int height;

		////indexing related variables
		//std::vector<unsigned long> fileOffsetIndex;
	};
}