#pragma once

namespace sysuVideo
{
	class ImageDecorator
	{
	public:
		ImageDecorator(void) : enable(TRUE) {};
		virtual ~ImageDecorator(void) {};
		
		//Pure virtual members
		virtual void AddParams(void *) = 0;
		virtual void Decorate(void * /* pointer to the image */, ...) = 0;

		virtual void Bind(FILE * /*CU split info*/);
		virtual void Disable(BOOL);
		
		//Getters and Setters
		virtual BOOL IsEnalbe() const;
		virtual COLORREF GetPenColor() const;
		virtual int GetPenWidth() const;
		virtual int GetPenStyle() const;
		virtual void SetPenColor(COLORREF);
		virtual void SetPenWidth(int);
		virtual void SetPenStyle(int);

	protected: 
		int penStyle;
		int penWidth;
		COLORREF penColor;
		BOOL enable;

		FILE *cuInfoStream;
	};
}
