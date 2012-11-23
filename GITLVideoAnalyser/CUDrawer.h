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

		void Draw(RECT *, CDC *) override;
		void BuildIndex() override;
		void Locale(unsigned long) override;

	protected:
		void Init(LPWSTR /* Filepath */) override;

	private:	//Auxiliary operations
		void readNextLCUDrawingFlag();

	private:
		unsigned long indexSize;
		std::vector<unsigned long> streamIndex;

		unsigned long curWorkingFrm;
		BYTE drawFlag[1000];	//Per-LCU drawing info
		int dfSize;				//drawFlag size
		int dfOffset;			//drawFlag offset
	};
}
