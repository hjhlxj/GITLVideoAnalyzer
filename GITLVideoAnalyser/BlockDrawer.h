#pragma once
#include "ImageDecorator.h"

namespace sysuVideo
{
	class BlockDrawer :
		public sysuVideo::ImageDecorator
	{
	public:
		explicit BlockDrawer(CImage * /* The target image to be decorated */);
		~BlockDrawer(void);

		virtual void addParams(void *);
		virtual void decorate(void * /* pointer to the image */, ...);
		
	private:
		CImage imgLayout;
	};
}