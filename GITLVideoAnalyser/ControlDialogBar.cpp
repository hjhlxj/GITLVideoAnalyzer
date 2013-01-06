// ControlDialogBar.cpp : implementation file
//

#include "stdafx.h"
#include "GITLVideoAnalyserDoc.h"
#include "GITLVideoAnalyserView.h"
#include "GITLVideoAnalyser.h"
#include "ControlDialogBar.h"
#include "afxdialogex.h"

#include "MainFrm.h"

#include "MagnifyWnd.h"
#include "AnalyserControlPanel.h"

#include "AuxFunc.h"

// ControlDialogBar dialog

IMPLEMENT_DYNAMIC(ControlDialogBar, CDialogBar)

ControlDialogBar::ControlDialogBar() : acp(_T("Analyser Control Panel"))
{
	m_edit = 0;
	maxFrameCount = 99999999;
}

ControlDialogBar::~ControlDialogBar()
{
}

void ControlDialogBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, m_spin);
	DDX_Text(pDX, IDC_EDIT1, m_edit);
	DDV_MinMaxInt(pDX, m_edit, 0, 99999999);
}

BEGIN_MESSAGE_MAP(ControlDialogBar, CDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &ControlDialogBar::OnDeltaposSpin1)
	ON_EN_CHANGE(IDC_EDIT1, &ControlDialogBar::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CUCHECK, &ControlDialogBar::OnBnClickedCUcheck)
	ON_BN_CLICKED(IDC_PUCHECK, &ControlDialogBar::OnBnClickedPUcheck)
	ON_BN_CLICKED(IDC_MVCHECK, &ControlDialogBar::OnBnClickedMVcheck)
	ON_BN_CLICKED(IDC_MODECHECK, &ControlDialogBar::OnBnClickedMODEcheck)
	ON_BN_CLICKED(IDC_BUTTONSHOWDIFF, &ControlDialogBar::OnBnClickedButtonshowdiff)
	ON_BN_CLICKED(IDC_RADIOCURVIDEO, &ControlDialogBar::OnBnClickedRadiocurvideo)
	ON_BN_CLICKED(IDC_RADIOCMPVIDEO, &ControlDialogBar::OnBnClickedRadiocmpvideo)
	ON_BN_CLICKED(IDC_BUTTONCTRLPNL, &ControlDialogBar::OnBnClickedButton2)
END_MESSAGE_MAP()


// ControlDialogBar message handlers
LONG ControlDialogBar::OnInitDialog(UINT wParam, LONG lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);

	if (!UpdateData(FALSE))
	{
		TRACE0("Warning: UpdateData failed during dialog init. \n");
	}
	
	m_spin.SetBuddy(GetDlgItem(IDC_EDIT1));

	((CButton *)GetDlgItem(IDC_RADIOCURVIDEO))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_RADIOCMPVIDEO))->EnableWindow(FALSE);

	return bRet;
}

void ControlDialogBar::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	if (pNMUpDown->iDelta == 1)
	{	
		//if (((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowPreFrame())
		if (GetGITLAppView()->ShowPreFrame())
			m_edit -= 1;
	}
	else
	{
		//if (((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowNextFrame())
		if (GetGITLAppView()->ShowNextFrame())
			m_edit += 1;
	}

	m_edit = max(0, m_edit);
	m_edit = min(maxFrameCount, m_edit);
	wsprintfW(buf, _T("%d"), m_edit);
	isSpinTrigger = TRUE;
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowTextW(buf);
	
	*pResult = 0;
}


void ControlDialogBar::SetTotalFrameCnt(unsigned long fcnt)
{
	wsprintf(buf, _T("%lu"), fcnt);
	((CStatic *)GetDlgItem(IDC_STATIC))->SetWindowTextW(buf);
}

void ControlDialogBar::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowTextW(buf, 250);
	m_edit = StrToIntW(buf);
	m_edit = max(0, m_edit);
	m_edit = min(maxFrameCount, m_edit);
	wsprintfW(buf, _T("%d"), m_edit);
	
	if (isSpinTrigger)		// avoid duplicate drawing due to edit control changes
		isSpinTrigger = FALSE;		
	else 
		//((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowNthFrame(m_edit);
		GetGITLAppView()->ShowNthFrame(m_edit);
	//((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowTextW(buf);
}


void ControlDialogBar::OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisableIfNoHandler)
{
	bDisableIfNoHandler = FALSE;
	CDialogBar::OnUpdateCmdUI(pTarget, FALSE);
}

void ControlDialogBar::OnBnClickedCUcheck()
{
	// TODO: Add your control notification handler code here
	static BOOL checked = FALSE;

	if (checked)
	{
		checked = FALSE;
		((CButton *)GetDlgItem(IDC_CUCHECK))->SetCheck(BST_UNCHECKED);	
	}
	else
	{
		checked = TRUE;
		((CButton *)GetDlgItem(IDC_CUCHECK))->SetCheck(BST_CHECKED);	
	}

	//((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->GetDocument()->gva.ShowCU(checked);
	//((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowNthFrame(m_edit);
	GetGVA().ShowCU(checked);
	GetGITLAppView()->ShowNthFrame(m_edit);
}


void ControlDialogBar::OnBnClickedPUcheck()
{
	// TODO: Add your control notification handler code here
	static BOOL checked = FALSE;

	if (checked)
	{
		checked = FALSE;
		((CButton *)GetDlgItem(IDC_PUCHECK))->SetCheck(BST_UNCHECKED);	
	}
	else
	{
		checked = TRUE;
		((CButton *)GetDlgItem(IDC_PUCHECK))->SetCheck(BST_CHECKED);	
	}

	/*((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->GetDocument()->gva.ShowPU(checked);
	((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowNthFrame(m_edit);*/
	GetGVA().ShowPU(checked);
	GetGITLAppView()->ShowNthFrame(m_edit);
}

void ControlDialogBar::OnBnClickedMVcheck()
{
	// TODO: Add your control notification handler code here
	static BOOL checked = FALSE;

	if (checked)
	{
		checked = FALSE;
		((CButton *)GetDlgItem(IDC_MVCHECK))->SetCheck(BST_UNCHECKED);	
	}
	else
	{
		checked = TRUE;
		((CButton *)GetDlgItem(IDC_MVCHECK))->SetCheck(BST_CHECKED);	
	}

	/*((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->GetDocument()->gva.ShowMV(checked);
	((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowNthFrame(m_edit);*/
	GetGVA().ShowMV(checked);
	GetGITLAppView()->ShowNthFrame(m_edit);
}

void ControlDialogBar::OnBnClickedMODEcheck()
{
	// TODO: Add your control notification handler code here
	static BOOL checked = FALSE;

	if (checked)
	{
		checked = FALSE;
		((CButton *)GetDlgItem(IDC_MODECHECK))->SetCheck(BST_UNCHECKED);	
	}
	else
	{
		checked = TRUE;
		((CButton *)GetDlgItem(IDC_MODECHECK))->SetCheck(BST_CHECKED);	
	}

	/*((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->GetDocument()->gva.ShowDecisionMode(checked);
	((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowNthFrame(m_edit);*/
	GetGVA().ShowDecisionMode(checked);
	GetGITLAppView()->ShowNthFrame(m_edit);
}

void ControlDialogBar::OnBnClickedButtonshowdiff()
{
	// TODO: Add your control notification handler code here
	((CButton *)GetDlgItem(IDC_RADIOCURVIDEO))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_RADIOCMPVIDEO))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_RADIOCURVIDEO))->SetCheck(BST_CHECKED);
	((CButton *)GetDlgItem(IDC_RADIOCMPVIDEO))->SetCheck(BST_UNCHECKED);
	/*((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->GetDocument()->gva.ShowCompareResult(TRUE);
	((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowNthFrame(m_edit);*/
	GetGVA().ShowCompareResult(TRUE);
	GetGITLAppView()->ShowNthFrame(m_edit);
}


void ControlDialogBar::OnBnClickedRadiocurvideo()
{
	// TODO: Add your control notification handler code here

	if (!((CButton *)GetDlgItem(IDC_RADIOCMPVIDEO))->GetCheck())
	{
		/*((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->GetDocument()->gva.SwitchDecorator();
		((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->GetDocument()->gva.ShowCompareResult(TRUE);
		((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowNthFrame(m_edit);*/
		GetGVA().SwitchDecorator();
		GetGVA().ShowCompareResult(TRUE);
		GetGITLAppView()->ShowNthFrame(m_edit);
		((CButton *)GetDlgItem(IDC_RADIOCURVIDEO))->SetCheck(BST_CHECKED);
		((CButton *)GetDlgItem(IDC_RADIOCMPVIDEO))->SetCheck(BST_UNCHECKED);
	}
}


void ControlDialogBar::OnBnClickedRadiocmpvideo()
{
	// TODO: Add your control notification handler code here

	if (!((CButton *)GetDlgItem(IDC_RADIOCURVIDEO))->GetCheck())
	{
		//((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->GetDocument()->gva.SwitchDecorator();
		//((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->GetDocument()->gva.ShowCompareResult(TRUE);
		//((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowNthFrame(m_edit);
		GetGVA().SwitchDecorator();
		GetGVA().ShowCompareResult(TRUE);
		GetGITLAppView()->ShowNthFrame(m_edit);
		((CButton *)GetDlgItem(IDC_RADIOCURVIDEO))->SetCheck(BST_UNCHECKED);
		((CButton *)GetDlgItem(IDC_RADIOCMPVIDEO))->SetCheck(BST_CHECKED);
	}
}

void ControlDialogBar::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	/*FILE *f;
	int err = _wfopen_s(&f, _T("F:\\hi.txt"), _T("w"));
	if (0 != err)
		MessageBox(_T("no permission"), _T("no"), MB_OK);
	fprintf_s(f, "Hi");
	fclose(f);*/
	if (IDOK == acp.DoModal())
		GetGITLAppView()->ShowNthFrame(m_edit);
}
