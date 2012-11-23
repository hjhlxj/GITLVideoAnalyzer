#pragma once

#include "VideoReader.h"
#include "ImageDecorator.h"

namespace sysuVideo
{
	class GITLVideoAnalyzer
	{
	public:
		GITLVideoAnalyzer(void);
		~GITLVideoAnalyzer(void);

	public:
		const CImage& GetCurrentFrame() const;
		const CImage& GetPreviousFrame();
		const CImage& GetNextFrame();
		const CImage& GetNthFrame();
		BOOL HasNextFrame() const;
		BOOL HasPreviousFrame() const;
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

		void MagnifyCU(POINT /* Position */);

		BOOL OpenVideoFile(LPWSTR /* File path */);
		BOOL OpenAnalyticalFile(LPWSTR /* File path */);

	private:
		VideoReader *pVReader;
		ImageDecorator *pImgDeco;
	};
}
