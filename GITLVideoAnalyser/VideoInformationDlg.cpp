// VideoInformationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GITLVideoAnalyser.h"
#include "VideoInformationDlg.h"
#include "afxdialogex.h"


// VideoInformationDlg dialog

IMPLEMENT_DYNAMIC(VideoInformationDlg, CDialogEx)

VideoInformationDlg::VideoInformationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(VideoInformationDlg::IDD, pParent)
	, m_VHeight(0)
	, m_VWidth(0)
{

}

VideoInformationDlg::~VideoInformationDlg()
{
}

void VideoInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITVHEIGHT, m_VHeight);
	DDV_MinMaxInt(pDX, m_VHeight, 1, 999999999);
	DDX_Text(pDX, IDC_EDITVWIDTH, m_VWidth);
	DDV_MinMaxInt(pDX, m_VWidth, 1, 999999999);
}


BEGIN_MESSAGE_MAP(VideoInformationDlg, CDialogEx)
END_MESSAGE_MAP()


// VideoInformationDlg message handlers
