// ControlDialogBar.cpp : implementation file
//

#include "stdafx.h"
#include "GITLVideoAnalyserDoc.h"
#include "GITLVideoAnalyserView.h"
#include "GITLVideoAnalyser.h"
#include "ControlDialogBar.h"
#include "afxdialogex.h"

#include "MainFrm.h"


// ControlDialogBar dialog

IMPLEMENT_DYNAMIC(ControlDialogBar, CDialogBar)

ControlDialogBar::ControlDialogBar()
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
	ON_EN_KILLFOCUS(IDC_EDIT1, &ControlDialogBar::OnEnKillfocusEdit1)
	ON_EN_CHANGE(IDC_EDIT1, &ControlDialogBar::OnEnChangeEdit1)
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

	return bRet;
}

void ControlDialogBar::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	if (pNMUpDown->iDelta == 1)
	{	
		if (((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowPreFrame())
			m_edit -= 1;
	}
	else
	{
		if (((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowNextFrame())
			m_edit += 1;
	}

	m_edit = max(0, m_edit);
	m_edit = min(maxFrameCount, m_edit);
	wsprintfW(buf, _T("%d"), m_edit);
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowTextW(buf);

	
	*pResult = 0;
}

void ControlDialogBar::OnEnKillfocusEdit1()
{
	// TODO: Add your control notification handler code here

	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowTextW(buf, 250);
	m_edit = StrToIntW(buf);
	m_edit = max(0, m_edit);
	m_edit = min(maxFrameCount, m_edit);
	wsprintfW(buf, _T("%d"), m_edit);
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowTextW(buf);
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
	
	((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->ShowNthFrame(m_edit);
	//((CEdit *)GetDlgItem(IDC_EDIT1))->SetWindowTextW(buf);
}
