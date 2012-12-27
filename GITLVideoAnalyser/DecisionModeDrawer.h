#pragma once
#include "StreamDirectedDrawer.h"
#include <vector>
#include <map>

namespace sysuVideo
{

	class DecisionModeDrawer :
		public StreamDirectedDrawer
	{
	public:
		DecisionModeDrawer(void);
		~DecisionModeDrawer(void);

	public:
		DRAWERTYPE GetDrawerType() const override;
		virtual void Draw(ImgBlock * /* block */, CDC * /* Device context of the image layer */) override; 
		virtual void Locale(unsigned long /* #frame */) override;
		virtual void Init(LPWSTR /* filepath */, int /*#num arg*/, ...) override;
		void SetColorEX(COLORREF skip, COLORREF inter, COLORREF intra);

	protected:	//Auxiliary
		virtual void BuildIndex() override;
		void getDMsForNextLCU();

	private:
		unsigned long indexSize;
		std::vector<unsigned long> streamIndex;

		unsigned long curWorkingFrm;
		LPBYTE pDecModes;			//Per-CU decision mode
		int dmSize;				//drawModes size
		int dmOffset;			//drawModes offset

		std::map<DECISIONMODE, CBrush*> brushes;  
	};

}