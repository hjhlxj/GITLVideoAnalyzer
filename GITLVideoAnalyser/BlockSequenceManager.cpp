#include "stdafx.h"
#include "BlockSequenceManager.h"
#include <stack>

static void splitCU(RECT& rect, std::stack<RECT>& s)
{
	static RECT subRect;

	//Bottom right
	subRect.top = (rect.top + rect.bottom) / 2;
	subRect.left = (rect.right + rect.left) / 2;
	subRect.right = rect.right;
	subRect.bottom = rect.bottom;
	s.push(subRect);

	//Bottom left
	subRect.right = subRect.left;
	subRect.left = rect.left;
	s.push(subRect);

	//Top right
	subRect.bottom = subRect.top;
	subRect.top = rect.top;
	subRect.left = subRect.right;
	subRect.right = rect.right;
	s.push(subRect);

	//Top left
	subRect.right = subRect.left;
	subRect.left = rect.left;
	s.push(subRect);
}

sysuVideo::BlockSequenceManager::BlockSequenceManager(LPWSTR filepath, CImage *imglayer)
{
	if (0 != _wfopen_s(&blockStream, filepath, _T("r")))
		throw new CFileException(0, -1, filepath);

	imgHeight = imglayer->GetHeight();
	imgWidth = imglayer->GetWidth();	
	splitFlags = new BYTE[(imglayer->GetWidth() / LCUSIZE + 1) * (imglayer->GetHeight() / LCUSIZE + 1) * 4]; 
	seqCursor = 0;
}

sysuVideo::BlockSequenceManager::~BlockSequenceManager(void)
{
	if (blockStream != NULL)
		fclose(blockStream);

	if (splitFlags != NULL)
		delete [] splitFlags;
}

void sysuVideo::BlockSequenceManager::BuildIndex()
{
	static const int bufSize = 1024 * 3;	//3K buffer
	static char buf[bufSize];

	blockIndex.clear();

	unsigned long frmCnt = -1;
	unsigned long curFrm;

	while (fgets(buf, bufSize, blockStream) != NULL)
	{
		sscanf_s(buf, "<%lu,", &curFrm);

		if (curFrm != frmCnt)
		{
			blockIndex.push_back(ftell(blockStream) - strlen(buf) - 1);
			frmCnt = curFrm;
		}
	}

	indexSize = blockIndex.size();
}

BOOL sysuVideo::BlockSequenceManager::GetNextBlock(ImgBlcok *block)
{
	if (block == NULL)
		return FALSE;

	if (seqCursor >= seqSize)		//Reach the end of block sequence of the current frame
		if (!Locale(indexCursor + 1))	//Get the cu blocks for the next frame
			return FALSE;
	
	*block = blockSeq[seqCursor++];
	return TRUE;
}

BOOL sysuVideo::BlockSequenceManager::Locale(unsigned long frmIndex)
{
	if (frmIndex >= indexSize)
		return FALSE;

	if (frmIndex == indexCursor) 
	{
		seqCursor = 0;
		return TRUE;
	}

	fseek(blockStream, blockIndex[frmIndex], SEEK_SET);
	curWorkingFrame = frmIndex;
	updateBlockSequence();

	return TRUE;
}

void sysuVideo::BlockSequenceManager::updateBlockSequence()
{
	static RECT curCU;
	static std::stack<RECT> CUs;
	static ImgBlcok ib;
	static BOOL lcuFlag;

	curLCU.left = -LCUSIZE;
	curLCU.top = curLCU.right = 0;
	curLCU.bottom = LCUSIZE;

	blockSeq.clear();

	while (getNextLCU(&curCU))
	{
		CUs.push(curCU);
		lcuFlag = TRUE;

		while (!CUs.empty())
		{
			curCU = CUs.top();			
			ib.type = lcuFlag ? IMGBLOCKTYPE::LCU : IMGBLOCKTYPE::CU;
			lcuFlag = FALSE;
			ib.area = curCU;
			blockSeq.push_back(ib);
			CUs.pop();
			
			if (splitContinue(&curCU))
			{
				splitCU(curCU, CUs);
			}
			
			appendPUsOfCurCU(&curCU);
		}
	}

	seqSize = blockSeq.size();
	seqCursor = 0;
}

BOOL sysuVideo::BlockSequenceManager::getNextLCU(RECT *lcu)
{		
	/*static int x = 0;
	
	if (x++ > 0)
		return FALSE;*/

	curLCU.left += LCUSIZE;

	if (curLCU.left >= imgWidth)
	{
		if (curLCU.bottom < imgHeight)
		{
			//Move to the head of next row
			curLCU.left = 0;
			curLCU.top = curLCU.bottom;
			curLCU.right = LCUSIZE;
			curLCU.bottom += LCUSIZE;

			//adjust bottom
			curLCU.bottom = min(curLCU.bottom, imgHeight);
		}
		else
		{
			return FALSE;	//Final block has been reached
		}
	}
	else
	{
		//Horizontal movement
		curLCU.right += LCUSIZE;
	}

	//Adjust right
	curLCU.right = min(curLCU.right, imgWidth);
	*lcu = curLCU;

	localeCUInfo();	//Locale the stream to the current LCU split information line

	return TRUE;
}

BOOL sysuVideo::BlockSequenceManager::splitContinue(RECT *cub)
{
	static int times = 0;

	if (reachAtomicSize(cub))
	{
		MessageBox(NULL, _T("reachAtomicSize exec"), _T("oops!"), MB_OK);
		return FALSE;
	}
	else if (sfcursor >= sflength)
	{
		return FALSE;
	}
	else
	{
		return 99 == splitFlags[sfcursor++];
	}
}

BOOL sysuVideo::BlockSequenceManager::reachAtomicSize(RECT *cub)
{
	return abs((cub->right - cub->left)) < (LCUSIZE / 4 / 4);	//Recursive depth is 4
}

void sysuVideo::BlockSequenceManager::localeCUInfo(void)
{
	static const int bufsize = 1024 * 3;
	static char buf[bufsize];	//3K buffer
	static int tmp;
	static char *token, *nextToken;
	static unsigned long frmCnt, CUCnt;
		
	fgets(buf, bufsize, blockStream);
	token = strtok_s(buf, " ", &nextToken);
	sscanf_s(token, "<%lu,%lu>", &frmCnt, &CUCnt);
	sflength = 0;

	if (frmCnt != curWorkingFrame)
	{
		sflength = sfcursor = 0;
		return;
	}

	token = strtok_s(NULL, " ", &nextToken);
	while (token != NULL && *token != '\n')
	{		
		tmp = atoi(token);
		splitFlags[sflength] = (BYTE)tmp;
		++sflength;
		token = strtok_s(NULL, " ", &nextToken);
	}	

	sfcursor = 0;
}

BOOL sysuVideo::BlockSequenceManager::isLCU(RECT *cu)
{
	return LCUSIZE == (cu->bottom - cu->top) && LCUSIZE == (cu->right - cu->left);
}

void sysuVideo::BlockSequenceManager::appendPUsOfCurCU(RECT *cu)
{
	static RECT subRect;
	static ImgBlcok ib;

	ib.type = IMGBLOCKTYPE::PU;

	switch (splitFlags[sfcursor - 1])
	{
	case PartSize::SIZE_2Nx2N:
		break;	// no split

	case PartSize::SIZE_NxN:
		//Bottom right
		subRect.top = (cu->top + cu->bottom) / 2;
		subRect.left = (cu->right + cu->left) / 2;
		subRect.right = cu->right;
		subRect.bottom = cu->bottom;
		ib.area = subRect;
		blockSeq.push_back(ib);

		//Bottom left
		subRect.right = subRect.left;
		subRect.left = cu->left;
		ib.area = subRect;
		blockSeq.push_back(ib);

		//Top right
		subRect.bottom = subRect.top;
		subRect.top = cu->top;
		subRect.left = subRect.right;
		subRect.right = cu->right;
		ib.area = subRect;
		blockSeq.push_back(ib);

		//Top left
		subRect.right = subRect.left;
		subRect.left = cu->left;
		ib.area = subRect;
		blockSeq.push_back(ib);
		break;

	case PartSize::SIZE_2NxN:
		subRect.top = cu->top;
		subRect.right = cu->right;
		subRect.bottom = (cu->bottom + cu->top) / 2;
		subRect.left = cu->left;
		ib.area = subRect;
		blockSeq.push_back(ib);
		subRect.top = subRect.bottom;
		subRect.bottom = cu->bottom;
		blockSeq.push_back(ib);
		break;
	
	case PartSize::SIZE_Nx2N:
		subRect.top = cu->top;
		subRect.right = (cu->right + cu->left) / 2;
		subRect.bottom = cu->bottom;
		subRect.left = cu->left;
		ib.area = subRect;
		blockSeq.push_back(ib);
		subRect.left = subRect.right;
		subRect.right = cu->right;
		blockSeq.push_back(ib);
		break;

#if AMP
	case PartSize::SIZE_2NxnU:			// vertical upper segmentation
		//Upper middle horizontal
		subRect.top = cu->top;
		subRect.right = cu->right;
		subRect.left = cu->left;
		subRect.bottom = cu->top + (cu->bottom - cu->top) / 4;
		ib.area = subRect;
		blockSeq.push_back(ib);
		subRect.top = subRect.bottom;
		subRect.bottom = cu->bottom;
		ib.area = subRect;
		blockSeq.push_back(ib);
		break;

	case SIZE_2NxnD:			// vertical lower segmentation
		//Lower middle horizontal
		subRect.top = cu->top;
		subRect.right = cu->right;
		subRect.left = cu->left;
		subRect.bottom = cu->bottom - (cu->bottom - cu->top) / 4;
		ib.area = subRect;
		blockSeq.push_back(ib);
		subRect.top = subRect.bottom;
		subRect.bottom = cu->bottom;
		ib.area = subRect;
		blockSeq.push_back(ib);
		break;
		break;

	case SIZE_nLx2N:			// horizontal upper segmentation
		//Upper middle vertical
		subRect.top = cu->top;
		subRect.right = cu->left + (cu->right - cu->left) / 4;
		subRect.left = cu->left;
		subRect.bottom = cu->bottom;
		ib.area = subRect;
		blockSeq.push_back(ib);
		subRect.left = subRect.right;
		subRect.right = cu->right;
		ib.area = subRect;
		blockSeq.push_back(ib);
		break;

	case SIZE_nRx2N:			// horizontal lower segmentation
		//Lower middle vertical
		subRect.top = cu->top;
		subRect.right = cu->right - (cu->right - cu->left) / 4;
		subRect.left = cu->left;
		subRect.bottom = cu->bottom;
		ib.area = subRect;
		blockSeq.push_back(ib);
		subRect.left = subRect.right;
		subRect.right = cu->right;
		ib.area = subRect;
		blockSeq.push_back(ib);
		break;
#endif

	default:
		break;
	}
}