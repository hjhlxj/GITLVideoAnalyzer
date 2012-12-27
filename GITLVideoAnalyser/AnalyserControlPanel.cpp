#include "stdafx.h"
#include "AnalyserControlPanel.h"
#include "AuxFunc.h"
#include "BlockRelatedDrawer.h"
#include "DecisionModeDrawer.h"
#include "DiffDrawer.h"

BEGIN_MESSAGE_MAP(sysuVideo::AnalyserControlPanel, CPropertySheet)
	ON_COMMAND(ID_APPLY_NOW, &AnalyserControlPanel::OnApplyNow)
END_MESSAGE_MAP()


sysuVideo::AnalyserControlPanel::AnalyserControlPanel(LPCTSTR pszCaption, CWnd *pParentWnd) :
        CPropertySheet (pszCaption, pParentWnd, 0)
{
	AddPage(&fileChooser);
	AddPage(&penChooser);
}


sysuVideo::AnalyserControlPanel::~AnalyserControlPanel(void)
{
}

static void doInitIfModify(sysuVideo::ImageDecorator* imgDeco, LPTSTR szFP, sysuVideo::DRAWERTYPE t, LPTSTR szCFP = NULL)
{
	static LPTSTR szFilePath, szCFilePath;
	szFilePath = szFP;
	if (sysuVideo::DRAWERTYPE::DIFFDRAWER != t)
		imgDeco->DoForeachDrawer(t,	[] (sysuVideo::DrawerBase *pdb) -> void* { pdb->Init(szFilePath); return NULL; });
	else
	{
		szCFilePath = szCFP;
		imgDeco->DoForeachDrawer(t,	[] (sysuVideo::DrawerBase *pdb) -> void* 
		{
			((sysuVideo::DiffDrawer*)pdb)->Init(szFilePath, 0, szCFilePath);
			return NULL; 
		});
	}
}

static void changePenWidth(sysuVideo::ImageDecorator* imgDeco, int pw, sysuVideo::DRAWERTYPE t)
{
	static int penWidth;
	penWidth = pw;
	imgDeco->DoForeachDrawer(t,	[] (sysuVideo::DrawerBase *pdb) -> void* { pdb->SetPenWidth(penWidth); return NULL; });
}

static void changePenColor(sysuVideo::ImageDecorator* imgDeco, COLORREF cr, sysuVideo::DRAWERTYPE t,
						   COLORREF cr1 = 0, COLORREF cr2 = 0)
{
	static COLORREF color, color1, color2;

	color = cr;

	if (sysuVideo::DRAWERTYPE::MODEDECISIONDRAWER != t)
		imgDeco->DoForeachDrawer(t,	[] (sysuVideo::DrawerBase *pdb) -> void* { pdb->SetPenColor(color); return NULL; });
	else 
	{
		color1 = cr1;
		color2 = cr2;
		imgDeco->DoForeachDrawer(t,	[] (sysuVideo::DrawerBase *pdb) -> void* 
		{ 
			((sysuVideo::DecisionModeDrawer*)pdb)->SetColorEX(color, color1, color2);
			return NULL; 
		});
	}
}

void sysuVideo::AnalyserControlPanel::OnApplyNow()
{
	// TODO: Add your command handler code here
	

	GetActivePage()->UpdateData (TRUE);

	if (&fileChooser == GetActivePage() && GetGVA().bVideoOpen)
	{
		fileChooser.UpdateTextIfModify();

		if (fileChooser.bfCUPUModify)
			((BlockRelatedDrawer *)GetGVA().pImgDecoBase)->GetBlockSequenceManager()->ReloadFile(fileChooser.szfCCUPU);

		if (fileChooser.bfMVModify)
			doInitIfModify(GetGVA().pImgDecoBase, fileChooser.szfCMV, DRAWERTYPE::MVDRAWER);

		if (fileChooser.bfMODEModify)
			doInitIfModify(GetGVA().pImgDecoBase, fileChooser.szfCMODE, DRAWERTYPE::MODEDECISIONDRAWER);

		if (fileChooser.bfRCUPUModify && 0 != lstrlenW(fileChooser.szfCCUPU))
		{
			((BlockRelatedDrawer *)GetGVA().pImgCmp)->GetBlockSequenceManager()->ReloadFile(fileChooser.szfRCUPU);	
			doInitIfModify(GetGVA().pImgDecoBase, fileChooser.szfCCUPU, DRAWERTYPE::DIFFDRAWER, fileChooser.szfRCUPU);
			doInitIfModify(GetGVA().pImgCmp, fileChooser.szfRCUPU, DRAWERTYPE::DIFFDRAWER, fileChooser.szfCCUPU);
		}

		if (fileChooser.bfRMVModify && 0 != lstrlenW(fileChooser.szfCCUPU))
			doInitIfModify(GetGVA().pImgCmp, fileChooser.szfRMV, DRAWERTYPE::MVDRAWER);

		if (fileChooser.bfRMODEModify && 0 != lstrlenW(fileChooser.szfCCUPU))
			doInitIfModify(GetGVA().pImgCmp, fileChooser.szfRMODE, DRAWERTYPE::MODEDECISIONDRAWER);
	}

	if (&penChooser == GetActivePage() && GetGVA().bVideoOpen)
	{
		if (penChooser.pwCU != penChooser.m_CUPenWidth)
		{
			changePenWidth(GetGVA().pImgCmp, penChooser.m_CUPenWidth, DRAWERTYPE::CUDRAWER);
			changePenWidth(GetGVA().pImgDecoBase, penChooser.m_CUPenWidth, DRAWERTYPE::CUDRAWER);
		}
		if (penChooser.pwPU != penChooser.m_PUPenWidth)
		{
			changePenWidth(GetGVA().pImgCmp, penChooser.m_PUPenWidth, DRAWERTYPE::PUDRAWER);
			changePenWidth(GetGVA().pImgDecoBase, penChooser.m_PUPenWidth, DRAWERTYPE::PUDRAWER);
		}
		if (penChooser.pwMV != penChooser.m_MVPenWidth)
		{
			changePenWidth(GetGVA().pImgCmp, penChooser.m_MVPenWidth, DRAWERTYPE::MVDRAWER);
			changePenWidth(GetGVA().pImgDecoBase, penChooser.m_MVPenWidth, DRAWERTYPE::MVDRAWER);
		}
		if (penChooser.pwDIFF != penChooser.m_DiffPenWidth)
		{
			changePenWidth(GetGVA().pImgCmp, penChooser.m_DiffPenWidth, DRAWERTYPE::DIFFDRAWER);
			changePenWidth(GetGVA().pImgDecoBase, penChooser.m_DiffPenWidth, DRAWERTYPE::DIFFDRAWER);
		}

		if (penChooser.crCU != penChooser.m_CUColor.GetColor())
		{
			changePenColor(GetGVA().pImgCmp, penChooser.m_CUColor.GetColor(), DRAWERTYPE::CUDRAWER);
			changePenColor(GetGVA().pImgDecoBase, penChooser.m_CUColor.GetColor(), DRAWERTYPE::CUDRAWER);
		}
		if (penChooser.crPU != penChooser.m_PUColor.GetColor())
		{
			changePenColor(GetGVA().pImgCmp, penChooser.m_PUColor.GetColor(), DRAWERTYPE::PUDRAWER);
			changePenColor(GetGVA().pImgDecoBase, penChooser.m_PUColor.GetColor(), DRAWERTYPE::PUDRAWER);
		}
		if (penChooser.crMV != penChooser.m_MVColor.GetColor())
		{
			changePenColor(GetGVA().pImgCmp, penChooser.m_MVColor.GetColor(), DRAWERTYPE::MVDRAWER);
			changePenColor(GetGVA().pImgDecoBase, penChooser.m_MVColor.GetColor(), DRAWERTYPE::MVDRAWER);
		}
		if (penChooser.crDIFF != penChooser.m_DiffColor.GetColor())
		{
			changePenColor(GetGVA().pImgCmp, penChooser.m_DiffColor.GetColor(), DRAWERTYPE::DIFFDRAWER);
			changePenColor(GetGVA().pImgDecoBase, penChooser.m_DiffColor.GetColor(), DRAWERTYPE::DIFFDRAWER);
		}
		if (penChooser.crSKIP != penChooser.m_MSkipColor.GetColor() || 
			penChooser.crINTER != penChooser.m_MInterColor.GetColor() ||
			penChooser.crINTRA != penChooser.m_MIntraColor.GetColor())
		{
			changePenColor(GetGVA().pImgCmp, penChooser.m_MSkipColor.GetColor(), DRAWERTYPE::MODEDECISIONDRAWER, 
				penChooser.m_MInterColor.GetColor(), penChooser.m_MIntraColor.GetColor());
			changePenColor(GetGVA().pImgDecoBase, penChooser.m_MSkipColor.GetColor(), DRAWERTYPE::MODEDECISIONDRAWER,
				penChooser.m_MInterColor.GetColor(), penChooser.m_MIntraColor.GetColor());
		}

		penChooser.UpdateData();
	}

	fileChooser.SetModified(FALSE);
	fileChooser.Reset();
	penChooser.SetModified(FALSE);
}

INT_PTR sysuVideo::AnalyserControlPanel::DoModal()
{
	INT_PTR ip;
	fileChooser.bShowText = TRUE;
	penChooser.bShowData = TRUE;
	ip = CPropertySheet::DoModal();
	return ip;
}