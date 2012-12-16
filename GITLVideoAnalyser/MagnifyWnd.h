#pragma once
#include "afxwin.h"

#include "MemoryDoubleBufferingShower.h"

namespace sysuVideo
{
	class MagnifyWnd :
		public CWnd
	{
	public:
		explicit MagnifyWnd(int /* width */, int /* height */);
		~MagnifyWnd(void);

	public:
		BOOL Show();
		BOOL ShowContent(int srcx, int srcy, const CImage& src);

	private:
		int wndWidth;
		int wndHeight;
		BOOL isWndHiden;

		MemoryDoubleBufferingShower mdbs;
	public:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnClose();
	};

}