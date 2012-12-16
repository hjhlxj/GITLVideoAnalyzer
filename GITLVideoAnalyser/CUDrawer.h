#pragma once

#include "StreamDirectedDrawer.h"
#include <vector>

namespace sysuVideo
{
	class CUDrawer : public StreamDirectedDrawer
	{
	public:
		CUDrawer(void);
		~CUDrawer(void);

		void Draw(ImgBlock *, CDC *) override;
		void BuildIndex() override;
		void Locale(unsigned long) override;
		void Init(LPWSTR /* Filepath */) override;

	protected:	//Auxiliary operations
		void readNextLCUDrawingFlag();

	protected:
		unsigned long indexSize;
		std::vector<unsigned long> streamIndex;

		unsigned long curWorkingFrm;
		LPBYTE drawFlag;	//Per-LCU drawing info
		int dfSize;				//drawFlag size
		int dfOffset;			//drawFlag offset
	};
}
