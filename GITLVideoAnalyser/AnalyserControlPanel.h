#pragma once
#include "afxdlgs.h"
#include "AnalyticalFileChooser.h"

namespace sysuVideo
{
	class AnalyserControlPanel :
		public CPropertySheet
	{
	public:
		AnalyserControlPanel(LPCTSTR pszCaption, CWnd* pParentWnd = NULL);
		~AnalyserControlPanel(void);

	public:
		virtual INT_PTR DoModal() override;
		AnalyticalFileChooser fileChooser;
		DECLARE_MESSAGE_MAP()
		afx_msg void OnApplyNow();
	};

}