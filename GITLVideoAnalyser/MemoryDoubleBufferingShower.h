#pragma once

namespace sysuVideo
{

	class MemoryDoubleBufferingShower
	{
	public:
		MemoryDoubleBufferingShower(void);
		~MemoryDoubleBufferingShower(void);

	public:
		BOOL ShowImage(CDC *pDC, int destx, int desty, int width, int height, const CImage& src);
		BOOL ShowImage(CDC *pDC, int destx, int desty, int destWidth, int destHeight, int srcx, 
						int srcy, int srcWidth, int srcHeight, const CImage& src);

	protected:
		BOOL createBuffer(CDC *pDC, int width, int height);

	private:
		BOOL hasBuf;
		int bufWidth, bufHeight;
		CDC memDC;
		CBitmap memBitmap;
		CBitmap *pOldBM;
	};

}
