//
#ifndef SC_LIVE_BIN_H_
#define SC_LIVE_BIN_H_

#include "LiveBinEnum.h"

namespace sc
{
	class LiveBinCore;

	class LiveBinSM
	{
	private:
		sc::LiveBinCore* p_core_;
		sc::LiveBin::stat GetNextStatus_(sc::LiveBin::stat a_current_status, sc::LiveBin::evt a_evt);
		sc::LiveBin::evt CallFunc_(sc::LiveBin::stat a_status);
		int ExecuteCore_(void);
	public:
		// 
		LiveBinSM(void);
		// 
		~LiveBinSM(void);
		// 
		int Execute(void);
	};
}

#endif  //SC_LIVE_BIN_H_
