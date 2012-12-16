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
		BOOL GetNextBlock(ImgBlock * /* Block container */);
		void BuildIndex();
		BOOL Locale(unsigned long /* #frame */);
		BOOL isLCU(RECT * /* CU */);

	private:	//Auxiliary functions
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