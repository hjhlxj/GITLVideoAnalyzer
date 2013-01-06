#pragma once


namespace sysuVideo
{
	enum VIDEOREADERTYPE {
		YUVREADER,
		DEFAULTREADER
	};

	class VideoReader
	{
	protected:
		VideoReader(void);	//The class can be access only through factory method

	public:		
		friend class VideoReaderFactory;
		virtual ~VideoReader(void);
		virtual BOOL HasNextFrame() const = 0;
		virtual BOOL HasPreFrame() const = 0;
		virtual BOOL HasNthFrame(unsigned long /*frame number*/) const = 0;
		virtual CImage* GetNextFrame() = 0;
		virtual CImage* GetPreFrame() = 0;
		virtual CImage* GetCurFrame() = 0;
		virtual CImage* GetNthFrame(unsigned long /*#frame*/) = 0;
		virtual BOOL Init(LPVOID /*structure of initialization info*/) = 0;
		virtual BOOL Open(LPCWSTR /*file path*/);
		virtual void Save(LPCWSTR /*file path*/, LPVOID /*structure of save info*/) = 0;
		virtual BOOL IsVideoStreamOpen() const;

		//Getters and Setters
		virtual int GetWidth() const;
		virtual void SetWidth(int);	
		virtual int GetHeight() const;
		virtual void SetHeight(int);	
		virtual double GetFrameRate() const;
		virtual void SetFrameRate(double);
		virtual unsigned long GetFrameCount() const;
		virtual unsigned long GetCurrentFrameNum() const;
		virtual void SetNBPP(int);
		virtual int GetNBPP() const;

	protected:
		int width;
		int height;
		double frameRate;
		int nbpp; //number of bits per pixel
		unsigned long frameCnt;
		unsigned long curFrameCnt;
		BOOL isStreamOpen;
		FILE *videoStream;
	};

	class VideoReaderFactory
	{
	public:
		virtual ~VideoReaderFactory(void);		
		static VideoReaderFactory& GetInstance();
		VideoReader* GetVideoReader(VIDEOREADERTYPE);
	private:
		static BOOL hasInstance;
		static VideoReaderFactory *facInstance;
		VideoReaderFactory(void);
	};
}
