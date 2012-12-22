#pragma once

#include "stdafx.h" 
#include "GITLVideoAnalyserDoc.h"
#include "GITLVideoAnalyserView.h"
#include "GITLVideoAnalyser.h"
#include "MainFrm.h"

#define GetGITLAppView() ((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())

#define GetGVA() ((CGITLVideoAnalyserView *)((CMainFrame *)::AfxGetMainWnd())->GetActiveView())->GetDocument()->gva

