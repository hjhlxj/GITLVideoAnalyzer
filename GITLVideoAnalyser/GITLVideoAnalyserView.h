
// GITLVideoAnalyserView.h : interface of the CGITLVideoAnalyserView class
//

#pragma once

#include <atlimage.h>
#include "MemoryDoubleBufferingShower.h"
#include "MagnifyWnd.h"

class CGITLVideoAnalyserView : public CScrollView
{
protected: // create from serialization only
	CGITLVideoAnalyserView();
	DECLARE_DYNCREATE(CGITLVideoAnalyserView)

// Attributes
public:
	CGITLVideoAnalyserDoc* GetDocument() const;
	void OnInitialUpdate();
// Operations
public:
	BOOL ShowNextFrame(void);
	BOOL ShowPreFrame(void);
	BOOL ShowNthFrame(unsigned long /*#frame*/);

protected:
	const CImage& getBufferedCurrentFrame();

private:
	CPoint dpZero;		// the begin point of the draw
	CPoint lbdCapture;	// the point where left button clicks down
	BOOL isLButtonDown;	// whether the left button is clicked down
	sysuVideo::MemoryDoubleBufferingShower mdbShower;	// double buffering image-shower
	int showWidth;
	int showHeight;
	double magnifyCoe;
	sysuVideo::MagnifyWnd *magWnd;
	CImage &bufCurrentFrame;

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGITLVideoAnalyserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in GITLVideoAnalyserView.cpp
inline CGITLVideoAnalyserDoc* CGITLVideoAnalyserView::GetDocument() const
   { return reinterpret_cast<CGITLVideoAnalyserDoc*>(m_pDocument); }
#endif

