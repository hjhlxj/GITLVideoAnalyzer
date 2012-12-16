#pragma once
#include "CUDrawer.h"

namespace sysuVideo
{
	class PUDrawer :
		public CUDrawer
	{
	public:
		PUDrawer(void);
		~PUDrawer(void);

	public:
		virtual void Draw(ImgBlock * /* block */, CDC * /* Device context of the image layer */) override; 
	};
}
