// PenBrushChooser.cpp : implementation file
//

#include "stdafx.h"
#include "GITLVideoAnalyser.h"
#include "PenBrushChooser.h"
#include "afxdialogex.h"
#include "AuxTypes.h"

// PenBrushChooser dialog

IMPLEMENT_DYNAMIC(PenBrushChooser, CPropertyPage)

PenBrushChooser::PenBrushChooser()
	: CPropertyPage(PenBrushChooser::IDD)
	, m_CUPenWidth(sysuVideo::DEFAULTCUPENWIDTH)
	, m_PUPenWidth(sysuVideo::DEFAULTPUPENWIDTH)
	, m_MVPenWidth(sysuVideo::DEFAULTMVPENWIDTH)
	, m_DiffPenWidth(sysuVideo::DEFAULTDIFFPENWIDTH)
{
	bShowData = TRUE;

	pwCU = sysuVideo::DEFAULTCUPENWIDTH;
	pwPU = sysuVideo::DEFAULTPUPENWIDTH;
	pwMV = sysuVideo::DEFAULTMVPENWIDTH;
	pwDIFF = sysuVideo::DEFAULTDIFFPENWIDTH;
	crCU = sysuVideo::DEFAULTCUCOLOR;
	crPU = sysuVideo::DEFAULTPUCOLOR;
	crMV = sysuVideo::DEFAULTMVCOLOR;
	crDIFF = sysuVideo::DEFAULTDIFFCOLOR;
	crSKIP = sysuVideo::DEFAULTSKIPCOLOR;
	crINTER = sysuVideo::DEFAULTINTERCOLOR;
	crINTRA = sysuVideo::DEFAULTINTRACOLOR;

	m_CUColor.SetColor(crCU);
	m_PUColor.SetColor(crPU);
	m_MVColor.SetColor(crMV);
	m_DiffColor.SetColor(crDIFF);
	m_MSkipColor.SetColor(crSKIP);
	m_MInterColor.SetColor(crINTER);
	m_MIntraColor.SetColor(crINTRA);
}

PenBrushChooser::~PenBrushChooser()
{
}

void PenBrushChooser::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITPWCU, m_CUPenWidth);
	DDV_MinMaxInt(pDX, m_CUPenWidth, 1, 5);
	DDX_Control(pDX, IDC_MFCCOLORBUTTONCU, m_CUColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTONPU, m_PUColor);
	DDX_Text(pDX, IDC_EDITPWPU, m_PUPenWidth);
	DDV_MinMaxInt(pDX, m_PUPenWidth, 1, 5);
	DDX_Control(pDX, IDC_MFCCOLORBUTTONMV, m_MVColor);
	DDX_Text(pDX, IDC_EDITPWMV, m_MVPenWidth);
	DDV_MinMaxInt(pDX, m_MVPenWidth, 1, 5);
	DDX_Control(pDX, IDC_MFCCOLORBUTTONMSKIP, m_MSkipColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTONMINTER, m_MInterColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTONMINTRA, m_MIntraColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTONDIFF, m_DiffColor);
	DDX_Text(pDX, IDC_EDITPWDIFF, m_DiffPenWidth);
	DDV_MinMaxInt(pDX, m_DiffPenWidth, 1, 5);

	if (bShowData)
	{
		m_CUPenWidth = pwCU;
		m_PUPenWidth = pwPU;
		m_MVPenWidth = pwMV;
		m_DiffPenWidth = pwDIFF;

		m_CUColor.SetColor(crCU);
		m_PUColor.SetColor(crPU);
		m_MVColor.SetColor(crMV);
		m_DiffColor.SetColor(crDIFF);
		m_MSkipColor.SetColor(crSKIP);
		m_MInterColor.SetColor(crINTER);
		m_MIntraColor.SetColor(crINTRA);
		bShowData = FALSE;
	}
}


BEGIN_MESSAGE_MAP(PenBrushChooser, CPropertyPage)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTONCU, &PenBrushChooser::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTONPU, &PenBrushChooser::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTONMV, &PenBrushChooser::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTONMSKIP, &PenBrushChooser::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTONMINTER, &PenBrushChooser::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTONMINTRA, &PenBrushChooser::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTONDIFF, &PenBrushChooser::OnBnClickedMfccolorbutton)
	ON_EN_CHANGE(IDC_EDITPWCU, &PenBrushChooser::OnEnChangeEditpw)
	ON_EN_CHANGE(IDC_EDITPWPU, &PenBrushChooser::OnEnChangeEditpw)
	ON_EN_CHANGE(IDC_EDITPWMV, &PenBrushChooser::OnEnChangeEditpw)
	ON_EN_CHANGE(IDC_EDITPWDIFF, &PenBrushChooser::OnEnChangeEditpw)
END_MESSAGE_MAP()

void PenBrushChooser::UpdateData()
{
	pwCU = m_CUPenWidth;
	pwPU = m_PUPenWidth;
	pwMV = m_MVPenWidth;
	pwDIFF = m_DiffPenWidth;
	crCU = m_CUColor.GetColor();
	crPU = m_PUColor.GetColor();
	crMV = m_MVColor.GetColor();
	crDIFF = m_DiffColor.GetColor();
	crSKIP = m_MSkipColor.GetColor();
	crINTER = m_MInterColor.GetColor();
	crINTRA = m_MIntraColor.GetColor();
}

// PenBrushChooser message handlers


void PenBrushChooser::OnBnClickedMfccolorbutton()
{
	// TODO: Add your control notification handler code here
	SetModified(TRUE);
}


void PenBrushChooser::OnEnChangeEditpw()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

//#1015

	// TODO:  Add your control notification handler code here
	SetModified(TRUE);
}
