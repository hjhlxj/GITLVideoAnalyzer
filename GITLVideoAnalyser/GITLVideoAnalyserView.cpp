
// GITLVideoAnalyserView.cpp : implementation of the CGITLVideoAnalyserView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GITLVideoAnalyser.h"
#endif

#include "GITLVideoAnalyserDoc.h"
#include "GITLVideoAnalyserView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGITLVideoAnalyserView

IMPLEMENT_DYNCREATE(CGITLVideoAnalyserView, CScrollView)

BEGIN_MESSAGE_MAP(CGITLVideoAnalyserView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CGITLVideoAnalyserView construction/destruction
static CImage ___cimgforinit;

CGITLVideoAnalyserView::CGITLVideoAnalyserView() : bufCurrentFrame(___cimgforinit)
{
	// TODO: add construction code here

	isLButtonDown = FALSE;
	dpZero.x = dpZero.y = showWidth = showHeight = 0;
	magnifyCoe = 1;
	magWnd = new sysuVideo::MagnifyWnd(192, 192);
	magWnd->ShowWindow(SW_HIDE);	// Hide the magnify window during initialization
}

CGITLVideoAnalyserView::~CGITLVideoAnalyserView()
{
	if (NULL != magWnd)
		delete magWnd;
}

BOOL CGITLVideoAnalyserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CGITLVideoAnalyserView drawing

void CGITLVideoAnalyserView::OnDraw(CDC* pDC)
{
	CGITLVideoAnalyserDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	if (!pDoc->dataReady)
		return;
	
	((CMainFrame *)GetParent())->m_wndToolBox.SetTotalFrameCnt(pDoc->gva.GetFrameCount());

	const CImage& cimg = this->GetDocument()->gva.GetCurrentFrame();
	bufCurrentFrame = cimg;
	if (showWidth <= 0 || showHeight <= 0)
	{
		showWidth = cimg.GetWidth();
		showHeight = cimg.GetHeight();
	}

	//cimg.Draw(pDC->m_hDC, 0, 0, cimg.GetWidth(), cimg.GetHeight());
	mdbShower.ShowImage(pDC, dpZero.x, dpZero.y, showWidth, showHeight, cimg);
}


// CGITLVideoAnalyserView printing

BOOL CGITLVideoAnalyserView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGITLVideoAnalyserView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGITLVideoAnalyserView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGITLVideoAnalyserView diagnostics

#ifdef _DEBUG
void CGITLVideoAnalyserView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGITLVideoAnalyserView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CGITLVideoAnalyserDoc* CGITLVideoAnalyserView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGITLVideoAnalyserDoc)));
	return (CGITLVideoAnalyserDoc*)m_pDocument;
}
#endif //_DEBUG

// Auxiliary functions
BOOL CGITLVideoAnalyserView::ShowNextFrame(void)
{
	CDC *pDC = this->GetDC();
	BOOL showStatus = FALSE;

	const CImage& cimg = this->GetDocument()->gva.GetNextFrame();
	//cimg.Draw(pDC->m_hDC, dpZero.x, dpZero.y, cimg.GetWidth(), cimg.GetHeight());
	mdbShower.ShowImage(pDC, dpZero.x, dpZero.y, showWidth, showHeight, cimg);
	bufCurrentFrame = cimg;
	showStatus = TRUE;
	
	return showStatus;
}

BOOL CGITLVideoAnalyserView::ShowPreFrame(void)
{
	CDC *pDC = this->GetDC();
	BOOL showStatus = FALSE;

	const CImage& cimg = this->GetDocument()->gva.GetPreviousFrame();
	//cimg.Draw(pDC->m_hDC, dpZero.x, dpZero.y, cimg.GetWidth(), cimg.GetHeight());
	mdbShower.ShowImage(pDC, dpZero.x, dpZero.y, showWidth, showHeight, cimg);
	bufCurrentFrame = cimg;
	showStatus = TRUE;

	return showStatus;
}

BOOL CGITLVideoAnalyserView::ShowNthFrame(unsigned long frmNum)
{
	CDC *pDC = this->GetDC();
	BOOL showStatus = FALSE;

	const CImage& cimg = this->GetDocument()->gva.GetNthFrame(frmNum);
	//cimg.Draw(pDC->m_hDC, dpZero.x, dpZero.y, cimg.GetWidth(), cimg.GetHeight());
	mdbShower.ShowImage(pDC, dpZero.x, dpZero.y, showWidth, showHeight, cimg);
	bufCurrentFrame = cimg;
	showStatus = TRUE;

	return showStatus;
}

BOOL CGITLVideoAnalyserView::ShowWithRefresh()
{
	CDC *pDC = this->GetDC();
	BOOL showStatus = FALSE;

	const CImage& cimg = this->GetDocument()->gva.GetCurrentFrame();
	//cimg.Draw(pDC->m_hDC, dpZero.x, dpZero.y, cimg.GetWidth(), cimg.GetHeight());
	mdbShower.ShowImage(pDC, dpZero.x, dpZero.y, showWidth, showHeight, cimg);
	bufCurrentFrame = cimg;
	showStatus = TRUE;

	return showStatus;
}

const CImage& CGITLVideoAnalyserView::getBufferedCurrentFrame()
{
	return bufCurrentFrame;
}


// CGITLVideoAnalyserView message handlers


BOOL CGITLVideoAnalyserView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	CDC *pDC = this->GetDC();
	BOOL showStatus = FALSE;

	if (zDelta > 0)
		magnifyCoe += .05;
	else
		magnifyCoe -= .05;
	
	//const CImage& cimg = this->GetDocument()->gva.GetCurrentFrame();
	const CImage& cimg = getBufferedCurrentFrame();

	showWidth = cimg.GetWidth() * magnifyCoe;
	showHeight = cimg.GetHeight() * magnifyCoe;
	mdbShower.ShowImage(pDC, dpZero.x, dpZero.y, showWidth, showHeight, cimg);
	showStatus = TRUE;

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt) && showStatus;
}

void CGITLVideoAnalyserView::OnInitialUpdate()
{
    // TODO: Add your specialized code here and/or call the base class
    CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CGITLVideoAnalyserView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnLButtonDown(nFlags, point);

	isLButtonDown = TRUE;
	lbdCapture = point;
}


void CGITLVideoAnalyserView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnLButtonUp(nFlags, point);
	isLButtonDown = FALSE;
}


void CGITLVideoAnalyserView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnMouseMove(nFlags, point);

	if (!isLButtonDown)
		return;

	CDC *pDC = this->GetDC();
	BOOL showStatus = FALSE;
	static CPoint pOffset;
	//const CImage& cimg = this->GetDocument()->gva.GetCurrentFrame();
	const CImage& cimg = getBufferedCurrentFrame();
	
	pOffset = point - lbdCapture;
	dpZero += pOffset;
	lbdCapture += pOffset;
	mdbShower.ShowImage(pDC, dpZero.x, dpZero.y, showWidth, showHeight, cimg);
}


void CGITLVideoAnalyserView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	static POINT targetCUPos;

	CScrollView::OnLButtonDblClk(nFlags, point);

	targetCUPos.x = (point.x - dpZero.x) / magnifyCoe;
	targetCUPos.y = (point.y - dpZero.y) / magnifyCoe;

	//const CImage& cimg = this->GetDocument()->gva.GetCurrentFrame();
	const CImage& cimg = getBufferedCurrentFrame();

	if (targetCUPos.x < 0 || targetCUPos.y < 0 || 
		targetCUPos.x > cimg.GetWidth() || targetCUPos.y > cimg.GetHeight())
		return;

	targetCUPos.x -= targetCUPos.x % sysuVideo::LCUSIZE;
	targetCUPos.y -= targetCUPos.y % sysuVideo::LCUSIZE;

	magWnd->ShowContent(targetCUPos.x, targetCUPos.y, cimg);
}
