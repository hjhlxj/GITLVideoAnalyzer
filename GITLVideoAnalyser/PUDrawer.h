#pragma once
#include "CUDrawer.h"

namespace sysuVideo
{
	class PUDrawer :
		public DrawerBase
	{
	public:
		PUDrawer(void);
		~PUDrawer(void);

	public:
		DRAWERTYPE GetDrawerType() const override;
		virtual void Draw(ImgBlock * /* block */, CDC * /* Device context of the image layer */) override;
		void Init(int /*#num arg*/, ...) override;
	};
}
