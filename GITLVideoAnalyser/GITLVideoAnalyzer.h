#pragma once

#include "VideoReader.h"
#include "ImageDecorator.h"
#include "AnalyserControlPanel.h"

namespace sysuVideo
{
	class GITLVideoAnalyzer
	{
	public:
		GITLVideoAnalyzer(void);
		~GITLVideoAnalyzer(void);

		friend AnalyserControlPanel;
	public:
		CImage* GetCurrentFrame();
		CImage* GetPreviousFrame();
		CImage* GetNextFrame();
		CImage* GetNthFrame(unsigned long /*#frame*/);
		BOOL HasNextFrame() const;
		BOOL HasPreviousFrame() const;
		BOOL HasNthFrame(unsigned long) const;
		unsigned long GetFrameCount() const;
		unsigned long GetCurrentFrameCount() const;
		int GetVideoWidth() const;
		int GetVideoHeight() const;

		void ShowCU(BOOL flag = TRUE);
		void ShowCUCost(BOOL flag = TRUE);
		void ShowPU(BOOL flag = TRUE);
		void ShowPUCost(BOOL flag = TRUE);
		void ShowPUInfo(POINT /* Position */);
		void ShowMV(BOOL flag = TRUE);
		void ShowDecisionMode(BOOL flag = TRUE);
		BOOL SwitchDecorator();
		void ShowCompareResult(BOOL flag = TRUE);
		void MagnifyCU(POINT /* Position */);

		BOOL OpenVideoFile(CString * /* File path */);
		BOOL OpenAnalyticalFile(LPWSTR /* File path */);
		BOOL SaveVideo(CArchive& /* File path */);
		BOOL ExportAsImage(CString /*File path*/);

	private:
		VideoReader *pVReader;
		BOOL bVideoOpen;
		ImageDecorator *pImgDeco;
		ImageDecorator *pImgDecoBase;
		ImageDecorator *pImgCmp;

		unsigned long curWorkingFrmNum;
	};
}
