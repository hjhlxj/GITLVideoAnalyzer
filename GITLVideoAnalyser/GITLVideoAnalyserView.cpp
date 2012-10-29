
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

IMPLEMENT_DYNCREATE(CGITLVideoAnalyserView, CView)

BEGIN_MESSAGE_MAP(CGITLVideoAnalyserView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CGITLVideoAnalyserView construction/destruction

CGITLVideoAnalyserView::CGITLVideoAnalyserView()
{
	// TODO: add construction code here

}

CGITLVideoAnalyserView::~CGITLVideoAnalyserView()
{
}

BOOL CGITLVideoAnalyserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
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
	
	((CMainFrame *)GetParent())->m_wndToolBox.SetTotalFrameCnt(pDoc->videoReader->GetFrameCount());

	const CImage& cimg = this->GetDocument()->videoReader->GetCurFrame();
	cimg.Draw(pDC->m_hDC, 0, 0, cimg.GetWidth(), cimg.GetHeight());
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
	CView::AssertValid();
}

void CGITLVideoAnalyserView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGITLVideoAnalyserDoc* CGITLVideoAnalyserView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGITLVideoAnalyserDoc)));
	return (CGITLVideoAnalyserDoc*)m_pDocument;
}
#endif //_DEBUG


// CGITLVideoAnalyserView message handlers




BOOL CGITLVideoAnalyserView::ShowNextFrame(void)
{
	CDC *pDC = this->GetDC();
	BOOL showStatus = FALSE;

	const CImage& cimg = this->GetDocument()->videoReader->GetNextFrame();
	cimg.Draw(pDC->m_hDC, 0, 0, cimg.GetWidth(), cimg.GetHeight());
	showStatus = TRUE;
	
	return showStatus;
}

BOOL CGITLVideoAnalyserView::ShowPreFrame(void)
{
	CDC *pDC = this->GetDC();
	BOOL showStatus = FALSE;

	const CImage& cimg = this->GetDocument()->videoReader->GetPreFrame();
	cimg.Draw(pDC->m_hDC, 0, 0, cimg.GetWidth(), cimg.GetHeight());
	showStatus = TRUE;

	return showStatus;
}