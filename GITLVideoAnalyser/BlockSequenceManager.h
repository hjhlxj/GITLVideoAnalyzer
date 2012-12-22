#pragma once

#include <vector>

namespace sysuVideo
{	
	class BlockSequenceManager
	{
	public:		
		explicit BlockSequenceManager(CImage * /* Image layer */, LPWSTR /* Block(CU) sequence file */ = NULL);
		~BlockSequenceManager(void);

	public:
		BOOL GetNextBlock(ImgBlock * /* Block container */);
		BOOL Locale(unsigned long /* #frame */);
		void ReloadFile(LPWSTR /* Block(CU) sequence file */);
		BOOL IsReady();

	private:	//Auxiliary functions
		void BuildIndex();
		BOOL isLCU(RECT * /* CU */);
		void updateBlockSequence();
		BOOL getNextLCU(RECT * /* LCU */);
		BOOL splitContinue(RECT * /* CU */);
		BYTE getCurrentSplitFlag() const;
		BOOL reachAtomicSize(RECT * /* CU block */);
		BOOL localeCUInfo(void);
		void appendPUsOfCurCU(RECT * /* CU */);

		BlockSequenceManager(const BlockSequenceManager&) {};
		BlockSequenceManager &operator=(const BlockSequenceManager&) {};

	private:
		BOOL dataReady;
		FILE *blockStream;
		std::vector<unsigned long> blockIndex;
		unsigned long indexSize;
		unsigned long indexCursor;
		
		std::vector<ImgBlock> blockSeq;
		unsigned long seqSize;
		unsigned long seqCursor;

		//split related variables
		RECT curLCU;
		int imgWidth;
		int imgHeight;
		LPBYTE splitFlags;
		int sfcursor;
		int sflength;
		unsigned long curWorkingFrame;
	};
}