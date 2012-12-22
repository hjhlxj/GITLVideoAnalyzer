#include "stdafx.h"
#include "MagnifyWnd.h"


sysuVideo::MagnifyWnd::MagnifyWnd(int width, int height)
{
	wndWidth = width;
	wndHeight = height;
	isWndHiden = TRUE;
    //
    // Register a WNDCLASS.
    //
    CString strWndClass = AfxRegisterWndClass (
        CS_DBLCLKS,                                     // Class style
        AfxGetApp ()->LoadStandardCursor (IDC_ARROW),   // Class cursor
        (HBRUSH) GetStockObject(WHITE_BRUSH),//COLOR_3DFACE + 1),                    // Background brush
        AfxGetApp ()->LoadStandardIcon (IDI_WINLOGO)    // Class icon
    );

    //
    // Create a window.
    //
    CreateEx (0, strWndClass, _T ("CU Magnifier"),
        WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, // | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL);

    //
    // Size the window.
    //
    CRect rect (0, 0, width, height);
    CalcWindowRect (&rect);
	posRect = rect;
	SetWindowPos (&CWnd::wndTopMost, 0, 0, rect.Width(), rect.Height(),
        SWP_NOZORDER | SWP_NOMOVE | SWP_NOREDRAW);
}


sysuVideo::MagnifyWnd::~MagnifyWnd(void)
{
}

BOOL sysuVideo::MagnifyWnd::Show()
{
	if (TRUE != isWndHiden)
		return TRUE;
	
	/*static CRect rect (0, 0, wndWidth, wndHeight);
    CalcWindowRect (&rect);*/
	SetWindowPos (&CWnd::wndTopMost, 0, 0, posRect.Width(), posRect.Height(),
        SWP_NOZORDER | SWP_NOMOVE | SWP_NOREDRAW);

    this->ShowWindow(SW_SHOW);
    this->UpdateWindow();
	isWndHiden = TRUE;
	return TRUE;
}

BOOL sysuVideo::MagnifyWnd::ShowContent(int srcx, int srcy, const CImage& src)
{
	Show();
	return mdbs.ShowImage(this->GetDC(), 0, 0, wndWidth, wndHeight, srcx, srcy, LCUSIZE, LCUSIZE, src);
}

BEGIN_MESSAGE_MAP(sysuVideo::MagnifyWnd, CWnd)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


void sysuVideo::MagnifyWnd::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	//CWnd::OnClose();
	this->ShowWindow(SW_HIDE);
	isWndHiden = TRUE;
}
