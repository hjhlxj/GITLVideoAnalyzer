#include "stdafx.h"
#include "DecisionModeDrawer.h"
#include <algorithm>

sysuVideo::DecisionModeDrawer::DecisionModeDrawer(void)
{
	pDecModes = new BYTE[1000];

	brushes.insert(std::make_pair(DECISIONMODE::SKIP, new CBrush(RGB(150, 73, 18))));

	brushes.insert(std::make_pair(DECISIONMODE::INTER, new CBrush(RGB(67, 0, 128))));

	brushes.insert(std::make_pair(DECISIONMODE::INTRA, new CBrush(RGB(255, 67, 67))));
}


sysuVideo::DecisionModeDrawer::~DecisionModeDrawer(void)
{
	if (NULL != pDecModes)
		delete pDecModes;

	std::for_each(brushes.begin(), brushes.end(), 
			[] (std::pair<DECISIONMODE, CBrush*> p) 
			-> void { delete p.second; });
}


void sysuVideo::DecisionModeDrawer::Init(LPWSTR filepath)
{
	enable = TRUE;

	if (0 != _wfopen_s(&directStream, filepath, _T("r")))
		return;

	enable = TRUE;
	BuildIndex();
}

void sysuVideo::DecisionModeDrawer::BuildIndex()
{
	static const int bufSize = 1024 * 3;
	static char buf[bufSize];
	static unsigned long frmCnt;
	static unsigned long curFrm;

	if (directStream == NULL)
		return;

	streamIndex.clear();

	frmCnt = -1;

	while (fgets(buf, bufSize, directStream) != NULL)
	{
		sscanf_s(buf, "<%lu,", &curFrm);

		if (curFrm != frmCnt)
		{
			streamIndex.push_back(ftell(directStream) - strlen(buf) - 1);
			frmCnt = curFrm;
		}
	}

	indexSize = streamIndex.size();

	dmOffset = 0;
	dmSize = -1;
}

void sysuVideo::DecisionModeDrawer::Locale(unsigned long index)
{
	if (!enable)
		return;
	
	if (index >= indexSize)
		return;

	fseek(directStream, streamIndex[index], SEEK_SET);
	curWorkingFrm = index;
}

void sysuVideo::DecisionModeDrawer::Draw(ImgBlock *block, CDC *pDC)
{
	static CBrush defBrush(RGB(0, 0, 0));
	static RECT curCU;

	if (!enable)
		return;

	if (IMGBLOCKTYPETAG::CMD_FLAG == block->type)
	{
		getDMsForNextLCU();
		return;
	}

	if ((IMGBLOCKTYPETAG::PU_HORZ_SPLIT != block->type &&
		IMGBLOCKTYPETAG::PU_VERT_SPLIT != block->type && 
		IMGBLOCKTYPETAG::ACTOMIC_BLOCK != block->type &&
		IMGBLOCKTYPETAG::PU_QUARTILE_SPLIT != block->type))
		return;			// Mode decision is for PU only	

	if (dmOffset >= dmSize - 1)
		return; //getDMsForNextLCU();

	curCU = block->area;

	switch (pDecModes[++dmOffset])
	{
	case DECISIONMODE::SKIP:
		pDC->FillRect(&curCU, brushes.at(DECISIONMODE::SKIP));
		break;

	case DECISIONMODE::INTER:
		pDC->FillRect(&curCU, brushes.at(DECISIONMODE::INTER));
		break;

	case DECISIONMODE::INTRA:
		pDC->FillRect(&curCU, brushes.at(DECISIONMODE::INTRA));
		break;

	default:
		pDC->FillRect(&curCU, &defBrush);
		break;
	}
}


void sysuVideo::DecisionModeDrawer::getDMsForNextLCU()
{
	static const int bufSize = 1024 * 3;
	static int tmp;
	static char buf[bufSize];
	static unsigned long frmCnt;
	static unsigned long curFrm;
	static char *token, *nextToken;

	fgets(buf, bufSize, directStream);
	token = strtok_s(buf, " ", &nextToken);
	sscanf_s(token, "<%lu,%lu>", &frmCnt, &curFrm);
	dmSize = 0;

	if (frmCnt != curWorkingFrm)
	{
		TCHAR sb[100];
		swprintf_s(sb, _T("curFrm %d, working Frm %d"), frmCnt, curWorkingFrm);
		MessageBox(NULL, sb, _T("ModeDecisionUnit"), MB_OK);
	}

	token = strtok_s(NULL, " ", &nextToken);
	while (token != NULL && *token != '\n')
	{		
		tmp = atoi(token);
		pDecModes[dmSize] = (BYTE)tmp;
		++dmSize;
		token = strtok_s(NULL, " ", &nextToken);
	}	

	dmOffset = -1;
}