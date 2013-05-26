#pragma once
#include "stdafx.h"
struct pkmprng
{
	uint32 mseed;
	 uint32 previous(){
		return 0xeeb9eb65 * mseed + 0xa3561a1;
	};
	 uint32 previousnum(){
		mseed = previous();
		return mseed;
	};
	 uint32 next(){
		return (0x41c64e6d * mseed) + 0x6073;
	};
	 uint32 nextnum(){
		mseed = next();
		return mseed;
	};
};
