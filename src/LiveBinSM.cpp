//
#include "LiveBinSM.h"
#include "LiveBinCore.h"

// constructor
sc::LiveBinSM::LiveBinSM(void)
{
	this->p_core_ = new sc::LiveBinCore();
	return;
}

// destructor
sc::LiveBinSM::~LiveBinSM(void)
{
	if (this->p_core_)
	{
		delete this->p_core_;
		this->p_core_ = nullptr;
	}
	return;
}

sc::LiveBin::stat sc::LiveBinSM::GetNextStatus_(sc::LiveBin::stat a_current_status, sc::LiveBin::evt a_evt)
{
	sc::LiveBin::stat ret_stat = sc::LiveBin::stat::Start;
	switch (a_current_status)
	{
	case sc::LiveBin::stat::Start:
		switch (a_evt)
		{
		case sc::LiveBin::evt::GoNext:
			ret_stat = sc::LiveBin::stat::Init;
			break;
		}
		break;
	case sc::LiveBin::stat::End:
		break;
	case sc::LiveBin::stat::Init:
		switch (a_evt)
		{
		case sc::LiveBin::evt::GoNext:
			ret_stat = sc::LiveBin::stat::Idle;
			break;
		case sc::LiveBin::evt::GoFail:
			ret_stat = sc::LiveBin::stat::Fail;
			break;
		}
		break;
	case sc::LiveBin::stat::Idle:
		switch (a_evt)
		{
		case sc::LiveBin::evt::GoNext:
			ret_stat = sc::LiveBin::stat::Do;
			break;
		case sc::LiveBin::evt::GoExit:
			ret_stat = sc::LiveBin::stat::Close;
			break;
		case sc::LiveBin::evt::Stay:
			ret_stat = sc::LiveBin::stat::Idle;
			break;
		}
		break;
	case sc::LiveBin::stat::Do:
		switch (a_evt)
		{
		case sc::LiveBin::evt::GoNext:
			ret_stat = sc::LiveBin::stat::Done;
			break;
		case sc::LiveBin::evt::GoExit:
			ret_stat = sc::LiveBin::stat::Close;
			break;
		case sc::LiveBin::evt::GoReset:
			ret_stat = sc::LiveBin::stat::Reset;
			break;
		case sc::LiveBin::evt::Stay:
			ret_stat = sc::LiveBin::stat::Do;
			break;
		}
		break;
	case sc::LiveBin::stat::Done:
		switch (a_evt)
		{
		case sc::LiveBin::evt::GoNext:
			//ret_stat = sc::LiveBin::stat::Init;
			ret_stat = sc::LiveBin::stat::Idle;
			break;
		}
		break;
	//case sc::LiveBin::stat::Pause:
	//	switch (a_evt)
	//	{
	//	case sc::LiveBin::evt::GoNext:
	//		ret_stat = sc::LiveBin::stat::Do;
	//		break;
	//	case sc::LiveBin::evt::GoExit:
	//		ret_stat = sc::LiveBin::stat::Close;
	//		break;
	//	case sc::LiveBin::evt::Stay:
	//		ret_stat = sc::LiveBin::stat::Pause;
	//		break;
	//	}
	//	break;
	case sc::LiveBin::stat::Reset:
		switch (a_evt)
		{
		case sc::LiveBin::evt::GoNext:
			ret_stat = sc::LiveBin::stat::Init;
			break;
		}
		break;
	case sc::LiveBin::stat::Fail:
		switch (a_evt)
		{
		case sc::LiveBin::evt::GoNext:
			ret_stat = sc::LiveBin::stat::Close;
			break;
		}
		break;
	case sc::LiveBin::stat::Close:
		switch (a_evt)
		{
		case sc::LiveBin::evt::GoNext:
			ret_stat = sc::LiveBin::stat::End;
			break;
		}
		break;
	}
	return ret_stat;
}

// 
sc::LiveBin::evt sc::LiveBinSM::CallFunc_(sc::LiveBin::stat a_status)
{
	sc::LiveBin::evt ret_evt = this->p_core_->FuncIF(a_status);

	return ret_evt;
}
// 
int sc::LiveBinSM::ExecuteCore_(void)
{
	int ret_result = -1;
	sc::LiveBin::stat current_stat = sc::LiveBin::stat::Start;
	sc::LiveBin::evt current_evt;
	bool flag_exit = false;
	do
	{
		current_evt = this->CallFunc_(current_stat);
		if (current_stat != sc::LiveBin::stat::End)
		{
			current_stat = this->GetNextStatus_(current_stat, current_evt);
		}
		else
		{
			flag_exit = true;
			ret_result = 0;
		}
	} while (flag_exit == false);
	return ret_result;
}

// 
int sc::LiveBinSM::Execute(void)
{
	return this->ExecuteCore_();
}
