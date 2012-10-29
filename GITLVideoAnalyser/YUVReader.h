#pragma once

#include "VideoReader.h"


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
		virtual BOOL GetNextFrame(CImage *);
		virtual BOOL GetPreFrame(CImage *);
		virtual BOOL GetCurFrame(CImage *);
		virtual BOOL Init(LPVOID /*structure of initialization info*/);
		virtual void Save(LPCWSTR /*file path*/, LPVOID /*structure of save info*/);
		virtual BOOL InitViaFilepath(CString /*filepath*/);

	protected:
		LPBYTE Y;
		LPBYTE U;
		LPBYTE V;
		CImage frameBuf;

		int YCount;
		int UCount;
		int VCount;

		const double coeRV;
		const double coeGU;
		const double coeGV;
		const double coeBU;
	};
}
