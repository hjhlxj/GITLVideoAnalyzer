#pragma once

#include "StreamDirectedDrawer.h"
#include <vector>

namespace sysuVideo
{
	class CUDrawer : public DrawerBase //StreamDirectedDrawer
	{
	public:
		CUDrawer(void);
		~CUDrawer(void);

		DRAWERTYPE GetDrawerType() const override;
		void Draw(ImgBlock *, CDC *) override;
		void Init() override;

		//void BuildIndex() override;
		//void Locale(unsigned long) override;

	protected:	//Auxiliary operations
		//void readNextLCUDrawingFlag();

	protected:
		//unsigned long indexSize;
		//std::vector<unsigned long> streamIndex;

		//unsigned long curWorkingFrm;
		//LPBYTE drawFlag;	//Per-LCU drawing info
		//int dfSize;				//drawFlag size
		//int dfOffset;			//drawFlag offset
	};
}
