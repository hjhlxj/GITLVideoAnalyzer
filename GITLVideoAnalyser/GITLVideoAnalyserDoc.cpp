
// GITLVideoAnalyserDoc.cpp : implementation of the CGITLVideoAnalyserDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GITLVideoAnalyser.h"
#endif

#include "GITLVideoAnalyserDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGITLVideoAnalyserDoc

IMPLEMENT_DYNCREATE(CGITLVideoAnalyserDoc, CDocument)

BEGIN_MESSAGE_MAP(CGITLVideoAnalyserDoc, CDocument)
END_MESSAGE_MAP()


// CGITLVideoAnalyserDoc construction/destruction

CGITLVideoAnalyserDoc::CGITLVideoAnalyserDoc()
{
	// TODO: add one-time construction code here

}

CGITLVideoAnalyserDoc::~CGITLVideoAnalyserDoc()
{
}

BOOL CGITLVideoAnalyserDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	dataReady = FALSE;

	return TRUE;
}




// CGITLVideoAnalyserDoc serialization

void CGITLVideoAnalyserDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		CString filepath = ar.GetFile()->GetFilePath();
		gva.SaveVideo(ar);
	}
	else
	{
		// TODO: add loading code here
		/*ULONGLONG fsize = ar.GetFile()->GetLength();
		fsize = sizeof(BYTE);
		int resRow = 832, resCol = 480;
		curFrame.Create(resRow, resCol, 24);
		int pixelCnt = resRow * resCol,  cnt = 0, rowUV = resRow / 2, colUV = resCol / 2, posUV;
		BYTE *Y = new BYTE[pixelCnt],
			 *U = new BYTE[pixelCnt / 4],
			 *V = new BYTE[pixelCnt / 4],
			 R, G, B;
		ar.Read(Y, pixelCnt);
		ar.Read(U, pixelCnt / 4);
		ar.Read(V, pixelCnt / 4);
		
		for (int i = 0; i < resCol; ++i)
		{
			for (int j = 0; j < resRow; ++j)
			{
				posUV = (i / 2) * rowUV + (j / 2);
				R = Y[cnt] + 1.13983 * (V[posUV] - 128);
				G = Y[cnt] - 0.39465 * (U[posUV] - 128) - 0.58060 * (V[posUV] - 128);
				B = Y[cnt] + 2.03211 * (U[posUV] - 128);
				curFrame.SetPixelRGB(j, i, R, G, B);
				++cnt;
			}
		}

		delete [] Y;
		delete [] U;
		delete [] V;*/

		CString filepath = ar.GetFile()->GetFilePath();
		
		//videoReader = sysuVideo::VideoReaderFactory::GetInstance().GetVideoReader(sysuVideo::VIDEOREADERTYPE::YUVREADER);
				
		//videoReader->Init(&filepath);
		if (gva.OpenVideoFile(&filepath))
			dataReady = TRUE;
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CGITLVideoAnalyserDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CGITLVideoAnalyserDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGITLVideoAnalyserDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGITLVideoAnalyserDoc diagnostics

#ifdef _DEBUG
void CGITLVideoAnalyserDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGITLVideoAnalyserDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGITLVideoAnalyserDoc commands
