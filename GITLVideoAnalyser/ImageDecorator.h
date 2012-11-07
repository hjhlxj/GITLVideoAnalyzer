#pragma once

namespace sysuVideo
{
	class ImageDecorator
	{
	public:
		ImageDecorator(void) : enable(TRUE) {};
		virtual ~ImageDecorator(void) {};
		
		virtual BOOL IsEnalbe() const { return enable; }
		virtual void Disable(BOOL flag) { enable = flag; }
		virtual void AddParams(void *) = 0;
		virtual void Decorate(void * /* pointer to the image */, ...) = 0;

	protected: 
		BOOL enable;
	};
}
