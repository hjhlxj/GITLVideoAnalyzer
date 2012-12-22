#pragma once
#include "ImageDecorator.h"
#include "DrawerBase.h"
#include "BlockSequenceManager.h"
#include <list>
#include <vector>

namespace sysuVideo
{
	class BlockDrawersFactory
	{
	public:
		static BlockDrawersFactory* GetInstance();
		DrawerBase* CreateDrawer(DRAWERTYPE /*type*/);

	private:
		BlockDrawersFactory() {};
		BlockDrawersFactory(const BlockDrawersFactory&);
		BlockDrawersFactory& operator=(const BlockDrawersFactory&);
		~BlockDrawersFactory() { if (NULL != ins) delete ins; }
		static BlockDrawersFactory* ins;
		static BOOL bHasInstance;
	};

	class BlockRelatedDrawer :
		public sysuVideo::ImageDecorator
	{
	public:
		// Experiment test only
		explicit BlockRelatedDrawer(CImage *c, LPWSTR bs, LPWSTR mv, LPWSTR md, LPWSTR diff, LPWSTR cmp);

	public:
		explicit BlockRelatedDrawer(CImage * /* The target image to be decorated */);
		~BlockRelatedDrawer(void);

		virtual void AddParams(PDecoratorParams /* params */) override;
		virtual void Decorate(void * /* pointer to the image */, int /*#num arg*/, ...) override;
		BOOL IsReady() override;
		virtual BOOL ActivateDrawers(DRAWERTYPE /* Drawer type */, BOOL /* activation code */ =  TRUE) override;
		virtual void* DoForeachDrawer(DRAWERTYPE /* Drawer type */, void* (*op)(DrawerBase *)) override;
		BlockSequenceManager* GetBlockSequenceManager() const { return bsmgr; }
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
		std::list<DrawerBase*> activeDrawers;
		std::vector<DrawerBase*> drawers;

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