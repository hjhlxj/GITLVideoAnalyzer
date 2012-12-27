#pragma once
#include "afxcolorbutton.h"


// PenBrushChooser dialog

class PenBrushChooser : public CPropertyPage
{
	DECLARE_DYNAMIC(PenBrushChooser)

public:
	PenBrushChooser();
	virtual ~PenBrushChooser();

// Dialog Data
	enum { IDD = IDD_DRAWINGPENDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeEditpw();
	afx_msg void OnBnClickedMfccolorbutton();
	void UpdateData();
	BOOL bShowData;

public:	// The interaction helper value
	
	int m_CUPenWidth;
	CMFCColorButton m_CUColor;
	CMFCColorButton m_PUColor;
	int m_PUPenWidth;
	CMFCColorButton m_MVColor;
	int m_MVPenWidth;
	CMFCColorButton m_MSkipColor;
	CMFCColorButton m_MInterColor;
	CMFCColorButton m_MIntraColor;
	CMFCColorButton m_DiffColor;
	int m_DiffPenWidth;

public:	//The actually data
	int pwCU;
	int pwPU;
	int pwMV;
	int pwDIFF;

	COLORREF crCU;
	COLORREF crPU;
	COLORREF crMV;
	COLORREF crDIFF;
	COLORREF crSKIP;
	COLORREF crINTER;
	COLORREF crINTRA;
};
