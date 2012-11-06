#pragma once

namespace sysuVideo
{
	class ImageDecorator
	{
	public:
		ImageDecorator(void) {};
		virtual ~ImageDecorator(void) {};
		
		virtual void addParams(void *) = 0;
		virtual void decorate(void * /* pointer to the image */, ...) = 0;
	};
}
