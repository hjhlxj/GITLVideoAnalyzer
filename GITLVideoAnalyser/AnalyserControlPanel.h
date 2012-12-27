#pragma once
#include "afxdlgs.h"
#include "AnalyticalFileChooser.h"
#include "PenBrushChooser.h"

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
		PenBrushChooser penChooser;

		DECLARE_MESSAGE_MAP()
		afx_msg void OnApplyNow();
	};

}