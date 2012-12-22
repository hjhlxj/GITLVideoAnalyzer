#pragma once

namespace sysuVideo
{
	class DrawerBase
	{
	public:
		DrawerBase(void);
		virtual ~DrawerBase(void);

	public:
		virtual DRAWERTYPE GetDrawerType() const = 0;
		virtual void Draw(ImgBlock * /* block */, CDC * /* Device context of the image layer */) = 0; 
		
		virtual void PreDrawingFrame(unsigned long /* #frame to be draw */);		
		virtual void Init(LPWSTR /* filepath */, int /*#num arg*/ = 0, ...);
		virtual void Init(int /*#num arg*/ = 0, ...);
		virtual void Enable(BOOL /* flag */);
		virtual BOOL IsEnable() const;
		virtual COLORREF GetPenColor() const;
		virtual int GetPenWidth() const;
		virtual int GetPenStyle() const;
		virtual BOOL SetPenColor(COLORREF);
		virtual BOOL SetPenWidth(int);
		virtual BOOL SetPenStyle(int);
	protected:
		BOOL enable;
		CPen pen;
		int penStyle;
		int penWidth;
		COLORREF penColor;
	};

}