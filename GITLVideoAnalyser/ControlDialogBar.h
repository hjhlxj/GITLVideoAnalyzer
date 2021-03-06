#pragma once
#include "afxwin.h"

#include "AnalyserControlPanel.h"

// ControlDialogBar dialog

class ControlDialogBar : public CDialogBar
{
	DECLARE_DYNAMIC(ControlDialogBar)

public:
	ControlDialogBar();   // standard constructor
	virtual ~ControlDialogBar();

	virtual void SetTotalFrameCnt(unsigned long fcnt);

	virtual void OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisableIfNoHandler); 

// Dialog Data
	enum { IDD = IDD_DIALOGBAR };
private:
	WCHAR buf[256];
	BOOL isSpinTrigger;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg LONG OnInitDialog(UINT, LONG);
	sysuVideo::AnalyserControlPanel acp;
	DECLARE_MESSAGE_MAP()

public:
	CSpinButtonCtrl m_spin;
	int m_edit;
	int maxFrameCount;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCUcheck();
	afx_msg void OnBnClickedPUcheck();
	afx_msg void OnBnClickedMVcheck();
	afx_msg void OnBnClickedMODEcheck();
	afx_msg void OnBnClickedButtonshowdiff();
	afx_msg void OnBnClickedRadiocurvideo();
	afx_msg void OnBnClickedRadiocmpvideo();
	afx_msg void OnBnClickedButton2();
};
