
// GITLVideoAnalyserDoc.h : interface of the CGITLVideoAnalyserDoc class
//


#pragma once

#include "GITLVideoAnalyzer.h"

class CGITLVideoAnalyserDoc : public CDocument
{
protected: // create from serialization only
	CGITLVideoAnalyserDoc();
	DECLARE_DYNCREATE(CGITLVideoAnalyserDoc)

// Attributes
public:
	sysuVideo::GITLVideoAnalyzer gva;
	BOOL dataReady;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CGITLVideoAnalyserDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
