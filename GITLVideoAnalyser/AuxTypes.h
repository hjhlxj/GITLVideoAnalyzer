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
		CU_SPLIT = 99,
		CMD_FLAG
	};

	typedef struct __ImgBlock
	{
		RECT area;
		IMGBLOCKTYPETAG type;
	} ImgBlock, *PImgBlock;

}