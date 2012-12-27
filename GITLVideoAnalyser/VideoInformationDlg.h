#pragma once

#include "Resource.h"

// VideoInformationDlg dialog

class VideoInformationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(VideoInformationDlg)

public:
	VideoInformationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~VideoInformationDlg();

// Dialog Data
	enum { IDD = IDD_DIALOGVIDEOINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_VHeight;
	int m_VWidth;
};
