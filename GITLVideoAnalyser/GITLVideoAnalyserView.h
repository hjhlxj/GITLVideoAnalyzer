
// GITLVideoAnalyserView.h : interface of the CGITLVideoAnalyserView class
//

#pragma once

#include <atlimage.h>

class CGITLVideoAnalyserView : public CView
{
protected: // create from serialization only
	CGITLVideoAnalyserView();
	DECLARE_DYNCREATE(CGITLVideoAnalyserView)

// Attributes
public:
	CGITLVideoAnalyserDoc* GetDocument() const;

// Operations
public:
	BOOL ShowNextFrame(void);
	BOOL ShowPreFrame(void);

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
};

#ifndef _DEBUG  // debug version in GITLVideoAnalyserView.cpp
inline CGITLVideoAnalyserDoc* CGITLVideoAnalyserView::GetDocument() const
   { return reinterpret_cast<CGITLVideoAnalyserDoc*>(m_pDocument); }
#endif

