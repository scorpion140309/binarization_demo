//
#include "BinDemoStateMachine.h"
#include "BinDemoCore.h"

// constructor
sc::BinStateMachine::BinStateMachine(void)
{
	this->p_core_ = new sc::BinCore();
	return;
}

// destructor
sc::BinStateMachine::~BinStateMachine(void)
{
	if (this->p_core_)
	{
		delete this->p_core_;
		this->p_core_ = nullptr;
	}
	return;
}

sc::BinDemo::stat sc::BinStateMachine::GetNextStatus_(sc::BinDemo::stat a_current_status, sc::BinDemo::evt a_evt)
{
	sc::BinDemo::stat ret_stat = sc::BinDemo::stat::Start;
	switch (a_current_status)
	{
	case sc::BinDemo::stat::Start:
		switch (a_evt)
		{
		case sc::BinDemo::evt::GoNext:
			ret_stat = sc::BinDemo::stat::Init;
			break;
		}
		break;
	case sc::BinDemo::stat::End:
		break;
	case sc::BinDemo::stat::Init:
		switch (a_evt)
		{
		case sc::BinDemo::evt::GoNext:
			ret_stat = sc::BinDemo::stat::Idle;
			break;
		case sc::BinDemo::evt::GoFail:
			ret_stat = sc::BinDemo::stat::Fail;
			break;
		}
		break;
	case sc::BinDemo::stat::Idle:
		switch (a_evt)
		{
		case sc::BinDemo::evt::GoNext:
			ret_stat = sc::BinDemo::stat::Do;
			break;
		case sc::BinDemo::evt::GoExit:
			ret_stat = sc::BinDemo::stat::Close;
			break;
		case sc::BinDemo::evt::Stay:
			ret_stat = sc::BinDemo::stat::Idle;
			break;
		}
		break;
	case sc::BinDemo::stat::Do:
		switch (a_evt)
		{
		case sc::BinDemo::evt::GoNext:
			ret_stat = sc::BinDemo::stat::Done;
			break;
		case sc::BinDemo::evt::GoExit:
			ret_stat = sc::BinDemo::stat::Close;
			break;
		case sc::BinDemo::evt::GoReset:
			ret_stat = sc::BinDemo::stat::Reset;
			break;
		case sc::BinDemo::evt::Stay:
			ret_stat = sc::BinDemo::stat::Do;
			break;
		}
		break;
	case sc::BinDemo::stat::Done:
		switch (a_evt)
		{
		case sc::BinDemo::evt::GoNext:
			//ret_stat = sc::LiveBin::stat::Init;
			ret_stat = sc::BinDemo::stat::Idle;
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
	case sc::BinDemo::stat::Reset:
		switch (a_evt)
		{
		case sc::BinDemo::evt::GoNext:
			ret_stat = sc::BinDemo::stat::Init;
			break;
		}
		break;
	case sc::BinDemo::stat::Fail:
		switch (a_evt)
		{
		case sc::BinDemo::evt::GoNext:
			ret_stat = sc::BinDemo::stat::Close;
			break;
		}
		break;
	case sc::BinDemo::stat::Close:
		switch (a_evt)
		{
		case sc::BinDemo::evt::GoNext:
			ret_stat = sc::BinDemo::stat::End;
			break;
		}
		break;
	}
	return ret_stat;
}

// 
sc::BinDemo::evt sc::BinStateMachine::CallFunc_(sc::BinDemo::stat a_status)
{
	sc::BinDemo::evt ret_evt = this->p_core_->FuncIF(a_status);

	return ret_evt;
}
// 
int sc::BinStateMachine::ExecuteCore_(void)
{
	int ret_result = -1;
	sc::BinDemo::stat current_stat = sc::BinDemo::stat::Start;
	sc::BinDemo::evt current_evt;
	bool flag_exit = false;
	do
	{
		current_evt = this->CallFunc_(current_stat);
		if (current_stat != sc::BinDemo::stat::End)
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
int sc::BinStateMachine::Execute(void)
{
	return this->ExecuteCore_();
}
