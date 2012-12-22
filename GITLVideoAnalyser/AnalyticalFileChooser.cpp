// AnalyticalFileChooser.cpp : implementation file
//

#include "stdafx.h"
#include "GITLVideoAnalyser.h"
#include "AnalyticalFileChooser.h"
#include "afxdialogex.h"


// AnalyticalFileChooser dialog

IMPLEMENT_DYNAMIC(AnalyticalFileChooser, CPropertyPage)

AnalyticalFileChooser::AnalyticalFileChooser()
	: CPropertyPage(AnalyticalFileChooser::IDD)
{
	ZeroMemory(szfCCUPU, bufSize * sizeof(TCHAR));
	ZeroMemory(szfCMV, bufSize * sizeof(TCHAR));
	ZeroMemory(szfCMODE, bufSize * sizeof(TCHAR));
	ZeroMemory(szfRCUPU, bufSize * sizeof(TCHAR));
	ZeroMemory(szfRMV, bufSize * sizeof(TCHAR));
	ZeroMemory(szfRMODE, bufSize * sizeof(TCHAR));
	bfCUPUModify = bfMVModify = bfMODEModify = FALSE;
	bfRCUPUModify = bfRMVModify = bfRMODEModify = FALSE;
}


AnalyticalFileChooser::~AnalyticalFileChooser()
{
}

void AnalyticalFileChooser::Reset()
{
	bfCUPUModify = bfMVModify = bfMODEModify = FALSE;
	bfRCUPUModify = bfRMVModify = bfRMODEModify = FALSE;
}

void AnalyticalFileChooser::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSECFCUPU, m_CurFrmCUPU);
	DDX_Control(pDX, IDC_MFCEDITBROWSECFMV, m_CurFrmMV);
	DDX_Control(pDX, IDC_MFCEDITBROWSECFMODE, m_CurFrmMode);
	DDX_Control(pDX, IDC_MFCEDITBROWSERFCUPU, m_RefFrmCUPU);
	DDX_Control(pDX, IDC_MFCEDITBROWSERFMODE, m_RefFrmMode);
	DDX_Control(pDX, IDC_MFCEDITBROWSERFMV, m_RefFrmMV);
	
	if (bShowText)
	{
		m_CurFrmCUPU.SetWindowTextW(szfCCUPU);
		m_CurFrmMV.SetWindowTextW(szfCMV);
		m_CurFrmMode.SetWindowTextW(szfCMODE);
		m_RefFrmCUPU.SetWindowTextW(szfRCUPU);
		m_RefFrmMV.SetWindowTextW(szfRMV);
		m_RefFrmMode.SetWindowTextW(szfRMODE);
		bShowText = FALSE;
	}

	m_CurFrmCUPU.EnableFileBrowseButton();
	m_CurFrmMV.EnableFileBrowseButton();
	m_CurFrmMode.EnableFileBrowseButton();
	m_RefFrmCUPU.EnableFileBrowseButton();
	m_RefFrmMV.EnableFileBrowseButton();
	m_RefFrmMode.EnableFileBrowseButton();
}


BEGIN_MESSAGE_MAP(AnalyticalFileChooser, CPropertyPage)
	ON_EN_CHANGE(IDC_MFCEDITBROWSECFCUPU, &AnalyticalFileChooser::OnEnChangeMfceditbrowse)
	ON_EN_CHANGE(IDC_MFCEDITBROWSECFMV, &AnalyticalFileChooser::OnEnChangeMfceditbrowse)
	ON_EN_CHANGE(IDC_MFCEDITBROWSECFMODE, &AnalyticalFileChooser::OnEnChangeMfceditbrowse)
	ON_EN_CHANGE(IDC_MFCEDITBROWSERFCUPU, &AnalyticalFileChooser::OnEnChangeMfceditbrowse)
	ON_EN_CHANGE(IDC_MFCEDITBROWSERFMV, &AnalyticalFileChooser::OnEnChangeMfceditbrowse)
	ON_EN_CHANGE(IDC_MFCEDITBROWSERFMODE, &AnalyticalFileChooser::OnEnChangeMfceditbrowse)
END_MESSAGE_MAP()


// AnalyticalFileChooser message handlers

static BOOL checkTextModify(CMFCEditBrowseCtrl &ctrl, LPTSTR text)
{
	static const int bsize = AnalyticalFileChooser::bufSize;
	static TCHAR tmpBuf[bsize];
	static int textLen;
	static BOOL change;

	textLen = ctrl.GetWindowTextLengthW();
	ctrl.GetWindowTextW(tmpBuf, bsize);
	StrTrim(tmpBuf,  _T(" \t"));
	change =  0 == memcmp(tmpBuf, text, textLen * sizeof(TCHAR)) ? FALSE : TRUE;

	/*if (change)
		lstrcpynW(text, tmpBuf, bsize);*/
	return change;
}

static void changeText(CMFCEditBrowseCtrl &ctrl, LPTSTR text)
{
	static const int bsize = AnalyticalFileChooser::bufSize;
	static TCHAR tmpBuf[bsize];
	static int textLen;
	static BOOL change;

	textLen = ctrl.GetWindowTextLengthW();
	ctrl.GetWindowTextW(tmpBuf, bsize);
	StrTrim(tmpBuf, _T(" \t"));
	lstrcpynW(text, tmpBuf, bsize);
}

void AnalyticalFileChooser::OnEnChangeMfceditbrowse()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.


	// TODO:  Add your control notification handler code here
	
	/*bfCUPUModify = checkTextModify(m_CurFrmCUPU, szfCCUPU);
	bfMVModify = checkTextModify(m_CurFrmMV, szfCMV);
	bfMODEModify = checkTextModify(m_CurFrmMode, szfCMODE);
	bfRCUPUModify = checkTextModify(m_RefFrmCUPU, szfRCUPU);
	bfRMVModify = checkTextModify(m_RefFrmMV, szfRMV);
	bfRMODEModify = checkTextModify(m_RefFrmMode, szfRMODE);*/

	SetModified(TRUE);
}

void AnalyticalFileChooser::UpdateTextIfModify()
{
	bfCUPUModify = checkTextModify(m_CurFrmCUPU, szfCCUPU);
	bfMVModify = checkTextModify(m_CurFrmMV, szfCMV);
	bfMODEModify = checkTextModify(m_CurFrmMode, szfCMODE);
	bfRCUPUModify = checkTextModify(m_RefFrmCUPU, szfRCUPU);
	bfRMVModify = checkTextModify(m_RefFrmMV, szfRMV);
	bfRMODEModify = checkTextModify(m_RefFrmMode, szfRMODE);

	if (bfCUPUModify)
		changeText(m_CurFrmCUPU, szfCCUPU);

	if (bfMVModify)
		changeText(m_CurFrmMV, szfCMV);
	
	if (bfMODEModify)
		changeText(m_CurFrmMode, szfCMODE);
	
	if (bfRCUPUModify)
		changeText(m_RefFrmCUPU, szfRCUPU);
	
	if (bfRMVModify)
		changeText(m_RefFrmMV, szfRMV);
	
	if (bfRMODEModify)
		changeText(m_RefFrmMode, szfRMODE);
}
