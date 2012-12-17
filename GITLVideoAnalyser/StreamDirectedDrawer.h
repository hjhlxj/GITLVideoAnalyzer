#pragma once

#include "DrawerBase.h"

namespace sysuVideo
{	
	class StreamDirectedDrawer :
		public DrawerBase
	{
	public:
		StreamDirectedDrawer(void);
		virtual ~StreamDirectedDrawer(void);

	public:
		//virtual void Disable(BOOL /* flag */);
		virtual void PreFrameDrawing(unsigned long /* #frame to be draw */) override;
		virtual void Init(LPWSTR /* filepath */) = 0;

		//Getters and Setters
		/*virtual BOOL IsEnalbe() const;
		virtual COLORREF GetPenColor() const;
		virtual int GetPenWidth() const;
		virtual int GetPenStyle() const;
		virtual BOOL SetPenColor(COLORREF);
		virtual BOOL SetPenWidth(int);
		virtual BOOL SetPenStyle(int);*/
		
	protected:	//Auxiliary
		virtual void BuildIndex() = 0;
		virtual void Locale(unsigned long /* #frame */) = 0;

	protected:
		FILE *directStream;

		/*BOOL enable;
		CPen pen;
		int penStyle;
		int penWidth;
		COLORREF penColor;*/
	};
}