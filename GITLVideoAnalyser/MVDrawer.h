#pragma once

#include "StreamDirectedDrawer.h"
#include <vector>

namespace sysuVideo 
{
	class MVDrawer : public StreamDirectedDrawer
	{
	public:
		MVDrawer(void);
		~MVDrawer(void);

	public:
		virtual void Draw(ImgBlcok * /* block */, CDC * /* Device context of the image layer */) override; 
		virtual void Locale(unsigned long /* #frame */) override;
		virtual void Init(LPWSTR /* filepath */) override;

	private:
		void getMVsForNextLCU();
		void drawVector(RECT * /* Area */, POINT /* Coordinate */, CDC * /* Pointer to the device context */);
		POINT lpToDp(RECT * /* Reference pu */, POINT& /* logical origin */, POINT & /* Logical Point */);

	protected:	//Auxiliary
		virtual void BuildIndex() override;

	private:
		unsigned long indexSize;
		std::vector<unsigned long> streamIndex;

		unsigned long curWorkingFrm;

		short *pPUVectors;
		int pVSize;
		int pVOffset;
	};
}
