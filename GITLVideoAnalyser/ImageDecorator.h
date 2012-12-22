#pragma once

#include "DrawerBase.h"

namespace sysuVideo
{
	class ImageDecorator
	{
	public:
		ImageDecorator(void) : enable(TRUE) {};
		virtual ~ImageDecorator(void) {};
		
		//Pure virtual members
		virtual void AddParams(PDecoratorParams) = 0;
		virtual void Decorate(void * /* pointer to the image */, int /*#num arg*/, ...) = 0;

		//virtual void Bind(FILE * /* drawer info*/);
		virtual BOOL IsReady() = 0;
		virtual void Disable(BOOL);
		virtual BOOL ActivateDrawers(DRAWERTYPE /* Drawer type */, BOOL /* activation code */) = 0;
		virtual void* DoForeachDrawer(DRAWERTYPE /* Drawer type */, void* (*)(DrawerBase *) /* Operation */) = 0;
		//Getters and Setters
		/*virtual BOOL IsEnalbe() const;
		virtual COLORREF GetPenColor() const;
		virtual int GetPenWidth() const;
		virtual int GetPenStyle() const;
		virtual void SetPenColor(COLORREF);
		virtual void SetPenWidth(int);
		virtual void SetPenStyle(int);*/

	private:
		ImageDecorator(const ImageDecorator&) {}
		ImageDecorator &operator=(const ImageDecorator&) {}

	protected: 
		BOOL enable;
		/*int penStyle;
		int penWidth;
		COLORREF penColor;

		FILE *cuInfoStream;*/
	};
}
