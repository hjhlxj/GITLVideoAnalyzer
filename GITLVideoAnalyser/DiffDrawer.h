#pragma once
#include "StreamDirectedDrawer.h"
#include <vector>

namespace sysuVideo
{
	class DiffDrawer :
		public StreamDirectedDrawer
	{
	public:
		DiffDrawer(void);
		~DiffDrawer(void);

	public:
		DRAWERTYPE GetDrawerType() const override;
		virtual void Draw(ImgBlock * /* block */, CDC * /* Device context of the image layer */) override; 
		virtual void Locale(unsigned long /* #frame */) override;
		virtual void Init(LPWSTR /* filepath */, int /*#num arg*/, ...) override;

	protected:	//Auxiliary
		virtual void BuildIndex();

	private:
		unsigned long indexSize;
		std::vector<unsigned long> streamIndex;

		unsigned long curWorkingFrm;

		FILE *cmpStream;
		unsigned long cmpIndexSize;
		std::vector<unsigned long> cmpStreamIndex;
	};

}