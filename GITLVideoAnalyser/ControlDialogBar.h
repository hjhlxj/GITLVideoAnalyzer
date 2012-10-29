#pragma once


// ControlDialogBar dialog

class ControlDialogBar : public CDialogBar
{
	DECLARE_DYNAMIC(ControlDialogBar)

public:
	ControlDialogBar();   // standard constructor
	virtual ~ControlDialogBar();

	virtual void SetTotalFrameCnt(unsigned long fcnt);

// Dialog Data
	enum { IDD = IDD_DIALOGBAR };

protected:
	WCHAR buf[256];
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg LONG OnInitDialog(UINT, LONG);

	DECLARE_MESSAGE_MAP()

public:
	CSpinButtonCtrl m_spin;
	int m_edit;
	int maxFrameCount;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEdit1();
};
