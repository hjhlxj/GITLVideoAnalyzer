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

		virtual BOOL HasNextFrame() const;
		virtual BOOL HasPreFrame() const;
		virtual const CImage& GetNextFrame();
		virtual const CImage& GetPreFrame();
		virtual const CImage& GetCurFrame();
		virtual BOOL Init(LPVOID /*structure of initialization info*/);
		virtual void Save(LPCWSTR /*file path*/, LPVOID /*structure of save info*/);
		virtual BOOL InitViaFilepath(CString /*filepath*/);

	protected:
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
