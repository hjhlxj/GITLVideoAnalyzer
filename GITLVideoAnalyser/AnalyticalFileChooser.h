#pragma once
#include "afxeditbrowsectrl.h"
#include "resource.h"

// AnalyticalFileChooser dialog

class AnalyticalFileChooser : public CPropertyPage
{
	DECLARE_DYNAMIC(AnalyticalFileChooser)

public:
	AnalyticalFileChooser();
	virtual ~AnalyticalFileChooser();

// Dialog Data
	enum { IDD = IDD_OPENANALYTICALFILEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void UpdateTextIfModify();
	void Reset();

public:
	BOOL bShowText;

	static const int bufSize = 256;
	TCHAR szfCCUPU[bufSize];
	BOOL bfCUPUModify;

	TCHAR szfCMV[bufSize];
	BOOL bfMVModify;

	TCHAR szfCMODE[bufSize];
	BOOL bfMODEModify;

	TCHAR szfRCUPU[bufSize];
	BOOL bfRCUPUModify;

	TCHAR szfRMV[bufSize];
	BOOL bfRMVModify;

	TCHAR szfRMODE[bufSize];
	BOOL bfRMODEModify;

	CMFCEditBrowseCtrl m_CurFrmCUPU;
	CMFCEditBrowseCtrl m_CurFrmMV;
	CMFCEditBrowseCtrl m_CurFrmMode;
	CMFCEditBrowseCtrl m_RefFrmCUPU;
	CMFCEditBrowseCtrl m_RefFrmMV;
	CMFCEditBrowseCtrl m_RefFrmMode;
	afx_msg void OnEnChangeMfceditbrowse();
};
