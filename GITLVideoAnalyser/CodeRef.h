#pragma once
#include "afxcmn.h"


// CodeRef dialog

class CodeRef : public CDialogEx
{
	DECLARE_DYNAMIC(CodeRef)

public:
	CodeRef(CWnd* pParent = NULL);   // standard constructor
	virtual ~CodeRef();

// Dialog Data
	enum { IDD = IDD_DIALOGBAR1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSpinButtonCtrl m_spin;
	int m_edit;
};
