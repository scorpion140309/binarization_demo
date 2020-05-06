//
#include "BinDemoStateMachine.h"

//
int main(int , char* )
{
	int result = -1;

	sc::BinStateMachine st_mc;
	result = st_mc.Execute();

	return result;
}
