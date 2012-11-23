#pragma once

#include <vector>

namespace sysuVideo
{
	class BlockSequenceManager
	{
	public:
		explicit BlockSequenceManager(LPWSTR /* Block(CU) sequence file */, CImage * /* Image layer */);
		~BlockSequenceManager(void);

	public:
		BOOL GetNextBlock(RECT * /* Block container */);
		void BuildIndex();
		BOOL Locale(unsigned long /* #frame */);

	private:	//Auxiliary functions
		void updateBlockSequence();
		BOOL getNextLCU(RECT * /* LCU */);
		BOOL splitContinue(RECT * /* CU */);
		BOOL reachAtomicSize(RECT * /* CU block */);
		void localeCUInfo(void);

	private:
		FILE *blockStream;
		std::vector<unsigned long> blockIndex;
		unsigned long indexSize;
		unsigned long indexCursor;
		
		std::vector<RECT> blockSeq;
		unsigned long seqSize;
		unsigned long seqCursor;

		//split related variables
		RECT curLCU;
		int LCUSIZE;
		int imgWidth;
		int imgHeight;
		LPBYTE splitFlags;
		int sfcursor;
		int sflength;
		unsigned long curWorkingFrame;
	};
}