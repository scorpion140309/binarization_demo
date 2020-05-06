//
#ifndef SC_LIVE_BIN_ENUM_H_
#define SC_LIVE_BIN_ENUM_H_

namespace sc
{
	namespace BinDemo
	{
		enum class stat
		{
			Start,
			End,
			Init,
			Idle,
			Do,
			Done,
			//Pause,
			Reset,
			Fail,
			Close,
		};
		enum class evt
		{
			_NOTHING_,
			GoNext,
			GoExit,
			GoReset,
			GoFail,
			Stay,
		};
	}
}

namespace cvkey
{
	constexpr int ESC = 27;
	constexpr int F01 = 0x00700000;
	constexpr int F02 = 0x00710000;
	constexpr int F03 = 0x00720000;
	constexpr int F04 = 0x00730000;
	constexpr int F05 = 0x00740000;
	constexpr int F06 = 0x00750000;
	constexpr int F07 = 0x00760000;
	constexpr int F08 = 0x00770000;
	constexpr int F09 = 0x00780000;
	constexpr int F10 = 0x00790000;
	constexpr int F11 = 0x007A0000;
	constexpr int F12 = 0x007B0000;
}

#endif  //SC_LIVE_BIN_ENUM_H_
