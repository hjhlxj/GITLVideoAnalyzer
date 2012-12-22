#pragma once

#include "stdafx.h"

namespace sysuVideo
{
	static const int LCUSIZE = 64;

	enum DECISIONMODE : short 
	{
		SKIP,
		INTER,
		INTRA
	};

	enum DRAWERTYPE 
	{
		CUDRAWER = 0,
		PUDRAWER,
		MVDRAWER,
		DIFFDRAWER,
		MODEDECISIONDRAWER,
		NONE
	};

	enum PartSize : short
	{
		SIZE_2Nx2N,			// symmetric motion partition, 2Nx2N
		SIZE_2NxN,          // symmetric motion partition, 2Nx N
		SIZE_Nx2N,			// symmetric motion partition,  Nx2N
		SIZE_NxN,			// symmetric motion partition,  Nx N
#if AMP
		SIZE_2NxnU,			// asymmetric motion partition, 2Nx( N/2) + 2Nx(3N/2)
		SIZE_2NxnD,			// asymmetric motion partition, 2Nx(3N/2) + 2Nx( N/2)
		SIZE_nLx2N,			// asymmetric motion partition, ( N/2)x2N + (3N/2)x2N
		SIZE_nRx2N,			// asymmetric motion partition, (3N/2)x2N + ( N/2)x/2N
#endif
		SIZE_NONE = 15
	};

	enum IMGBLOCKTYPETAG : short
	{
		ACTOMIC_BLOCK = SIZE_2Nx2N,			// symmetric motion partition, 2Nx2N
		PU_HORZ_SPLIT = SIZE_2NxN,          // symmetric motion partition, 2Nx N
		PU_VERT_SPLIT = SIZE_Nx2N,			// symmetric motion partition,  Nx2N
		PU_QUARTILE_SPLIT = SIZE_NxN,			// symmetric motion partition,  Nx N
#if AMP
		SIZE_2NxnU,			// asymmetric motion partition, 2Nx( N/2) + 2Nx(3N/2)
		SIZE_2NxnD,			// asymmetric motion partition, 2Nx(3N/2) + 2Nx( N/2)
		SIZE_nLx2N,			// asymmetric motion partition, ( N/2)x2N + (3N/2)x2N
		SIZE_nRx2N,			// asymmetric motion partition, (3N/2)x2N + ( N/2)x/2N
#endif
		INCOMPLETE_BLOCK = PartSize::SIZE_NONE,
		LCU,
		CU_SPLIT = 99,
		CMD_FLAG
	};

	typedef struct ___ImgBlock
	{
		RECT area;
		IMGBLOCKTYPETAG type;
	} ImgBlock, *PImgBlock;

	enum DECORATOROPCODE : short
	{
		ADD,
		REMOVE,
		CHANGESETTING,
		NONEOP
	};

	typedef struct ___DecoratorParams
	{
		___DecoratorParams ()
		{
			op = DECORATOROPCODE::NONEOP;
			dt = DRAWERTYPE::NONE;
			fp = NULL;
			pimg = NULL;
		}

		CImage *pimg;
		DECORATOROPCODE op;
		DRAWERTYPE dt;
		LPTSTR fp;
		COLORREF c1;
		COLORREF c2;
		COLORREF c3;
		UINT penWith;
	} DecoratorParams, *PDecoratorParams;
}