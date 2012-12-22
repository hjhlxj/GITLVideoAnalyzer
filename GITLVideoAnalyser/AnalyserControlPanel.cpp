#include "stdafx.h"
#include "AnalyserControlPanel.h"
#include "AuxFunc.h"
#include "BlockRelatedDrawer.h"

BEGIN_MESSAGE_MAP(sysuVideo::AnalyserControlPanel, CPropertySheet)
	ON_COMMAND(ID_APPLY_NOW, &AnalyserControlPanel::OnApplyNow)
END_MESSAGE_MAP()


sysuVideo::AnalyserControlPanel::AnalyserControlPanel(LPCTSTR pszCaption, CWnd *pParentWnd) :
        CPropertySheet (pszCaption, pParentWnd, 0)
{
	AddPage(&fileChooser);
}


sysuVideo::AnalyserControlPanel::~AnalyserControlPanel(void)
{
}

static void doInitIfModify(sysuVideo::ImageDecorator* imgDeco, LPTSTR szFP, sysuVideo::DRAWERTYPE t)
{
	static LPTSTR szFilePath;
	szFilePath = szFP;
	imgDeco->DoForeachDrawer(t,	[] (sysuVideo::DrawerBase *pdb) -> void* { pdb->Init(szFilePath); return NULL; });
}

void sysuVideo::AnalyserControlPanel::OnApplyNow()
{
	// TODO: Add your command handler code here
	

	GetActivePage()->UpdateData (TRUE);

	if (&fileChooser == GetActivePage())
	{
		fileChooser.UpdateTextIfModify();
		if (fileChooser.bfCUPUModify)
		{
			((BlockRelatedDrawer *)GetGVA().pImgDecoBase)->GetBlockSequenceManager()->ReloadFile(fileChooser.szfCCUPU);
			/*GetGVA().pImgDecoBase->ActivateDrawers(DRAWERTYPE::CUDRAWER, TRUE);
			GetGVA().pImgDecoBase->ActivateDrawers(DRAWERTYPE::PUDRAWER, TRUE);*/
		}
		if (fileChooser.bfMVModify)
		{
			doInitIfModify(GetGVA().pImgDecoBase, fileChooser.szfCMV, DRAWERTYPE::MVDRAWER);
			/*GetGVA().pImgDecoBase->ActivateDrawers(DRAWERTYPE::MVDRAWER, TRUE);*/
		}
		if (fileChooser.bfMODEModify)
		{
			doInitIfModify(GetGVA().pImgDecoBase, fileChooser.szfCMODE, DRAWERTYPE::MODEDECISIONDRAWER);
			//GetGVA().pImgDecoBase->ActivateDrawers(DRAWERTYPE::MODEDECISIONDRAWER, TRUE);
		}

		if (fileChooser.bfRCUPUModify)
		{
			((BlockRelatedDrawer *)GetGVA().pImgCmp)->GetBlockSequenceManager()->ReloadFile(fileChooser.szfRCUPU);
			/*GetGVA().pImgCmp->ActivateDrawers(DRAWERTYPE::CUDRAWER, TRUE);
			GetGVA().pImgCmp->ActivateDrawers(DRAWERTYPE::PUDRAWER, TRUE);*/
		}
		if (fileChooser.bfRMVModify)
		{
			doInitIfModify(GetGVA().pImgCmp, fileChooser.szfRMV, DRAWERTYPE::MVDRAWER);
			//GetGVA().pImgCmp->ActivateDrawers(DRAWERTYPE::MVDRAWER, TRUE);
		}
		if (fileChooser.bfRMODEModify)
		{
			doInitIfModify(GetGVA().pImgCmp, fileChooser.szfRMODE, DRAWERTYPE::MODEDECISIONDRAWER);
			//GetGVA().pImgCmp->ActivateDrawers(DRAWERTYPE::MODEDECISIONDRAWER, TRUE);
		}
	}

	fileChooser.SetModified(FALSE);
	fileChooser.Reset();
}

INT_PTR sysuVideo::AnalyserControlPanel::DoModal()
{
	INT_PTR ip;
	fileChooser.bShowText = TRUE;
	ip = CPropertySheet::DoModal();
	return ip;
}