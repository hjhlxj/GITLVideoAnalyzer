#pragma once

namespace sysuVideo
{
	class StreamDirectedDrawer
	{
	public:
		StreamDirectedDrawer(void);
		virtual ~StreamDirectedDrawer(void);

	public:
		virtual void Draw(RECT * /* block */, CDC * /* Device context of the image layer */) = 0; 
		virtual void Disable(BOOL /* flag */);
		virtual void Locale(unsigned long /* #frame */) = 0;
		virtual void Init(LPWSTR /* filepath */) = 0;

		//Getters and Setters
		virtual BOOL IsEnalbe() const;
		virtual COLORREF GetPenColor() const;
		virtual int GetPenWidth() const;
		virtual int GetPenStyle() const;
		virtual void SetPenColor(COLORREF);
		virtual void SetPenWidth(int);
		virtual void SetPenStyle(int);
		
	protected:	//Auxiliary
		virtual void BuildIndex() = 0;

	protected:
		FILE *directStream;

		BOOL enable;
		CPen pen;
		int penStyle;
		int penWidth;
		COLORREF penColor;
	};
}