//
#ifndef SC_LIVE_BIN_H_
#define SC_LIVE_BIN_H_

#include "BinDemoEnum.h"

namespace sc
{
	class BinCore;

	class BinStateMachine
	{
	private:
		sc::BinCore* p_core_;
		sc::BinDemo::stat GetNextStatus_(sc::BinDemo::stat a_current_status, sc::BinDemo::evt a_evt);
		sc::BinDemo::evt CallFunc_(sc::BinDemo::stat a_status);
		int ExecuteCore_(void);
	public:
		// 
		BinStateMachine(void);
		// 
		~BinStateMachine(void);
		// 
		int Execute(void);
	};
}

#endif  //SC_LIVE_BIN_H_
