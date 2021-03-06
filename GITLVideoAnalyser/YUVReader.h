#pragma once

#include "VideoReader.h"
#include "ImageDecorator.h"

namespace sysuVideo
{
	class YUVReader :
		public VideoReader
	{
	public:
		YUVReader(void);
		~YUVReader(void);

		virtual BOOL HasNextFrame() const override;
		virtual BOOL HasPreFrame() const override;
		virtual BOOL HasNthFrame(unsigned long /*#frame*/) const override;
		virtual CImage* GetNthFrame(unsigned long /*frame number*/) override;
		virtual CImage* GetNextFrame() override;
		virtual CImage* GetPreFrame() override;
		virtual CImage* GetCurFrame() override;
		virtual BOOL Init(LPVOID /*structure of initialization info*/) override;
		virtual void Save(LPCWSTR /*file path*/, LPVOID /*structure of save info*/) override;
		virtual BOOL InitViaFilepath(CString /*filepath*/);
		virtual BOOL InitViaInteraction();

	protected:	//Member functions
		void constructFrame();

	protected:	//Fields and Attributes
		LPBYTE Y;
		LPBYTE U;
		LPBYTE V;
		CImage frameBuf;
		ImageDecorator *imgDeco;

		int YCount;
		int UCount;
		int VCount;

		const double coeRV;
		const double coeGU;
		const double coeGV;
		const double coeBU;
	};
}
