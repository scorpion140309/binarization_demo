//
#ifndef SC_LIVE_BIN_CORE_H_
#define SC_LIVE_BIN_CORE_H_

//
#include "LiveBinEnum.h"

//
#include <vector>
#include <map>
//#include <functional>
//#include <map>
#include <string>

namespace cv
{
	class Mat;
}

namespace sc
{
	class LiveBinCore
	{
	private:
		enum class img_type
		{
			e_SRC,
			e_GRAY,
			e_DST,
			_NUM_
		};
		enum class ht_type
		{
			e_FIXED_THRESH,
			e_RANDOM_THRESH,
			e_ERR_DIFFUSION,
			_NUM_
		};

	private:
		std::vector<cv::Mat> ary_img_;
		std::map<int, sc::LiveBin::evt (sc::LiveBinCore::*)()> dict_st_functions_;

		// cv
		int key_;
		int key_delay_;

		//
		std::vector<std::string> ary_filenames_;

		//
		size_t count_x_;
		size_t count_y_;
		std::vector<int> ary_threshold_;

	private:
		void Create_(void);
		void Destroy_(void);

		//
		cv::Mat* GetImgPtr_(sc::LiveBinCore::img_type);

		//
		sc::LiveBin::evt FuncStart_(void);
		sc::LiveBin::evt FuncEnd_(void);
		sc::LiveBin::evt FuncInit_(void);
		sc::LiveBin::evt FuncIdle_(void);
		sc::LiveBin::evt FuncDo_(void);
		sc::LiveBin::evt FuncDone_(void);
		//sc::LiveBin::evt FuncPause_(void);
		sc::LiveBin::evt FuncReset_(void);
		sc::LiveBin::evt FuncFail_(void);
		sc::LiveBin::evt FuncClose_(void);

		//
		sc::LiveBinCore::ht_type halftoning_type_;
		std::map<int, sc::LiveBin::evt(sc::LiveBinCore::*)(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst)> dict_bin_functions_;
		sc::LiveBin::evt Binaliztion_FixedThresh_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst);
		sc::LiveBin::evt Binaliztion_RandomThresh_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst);
		sc::LiveBin::evt Binaliztion_ErrorDiffusion_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst);

		//
		void CommonProc_(void);

	public:
		// 
		LiveBinCore(void);
		// 
		~LiveBinCore(void);

		//
		sc::LiveBin::evt FuncIF(sc::LiveBin::stat a_stat);
	};
}

#endif  //SC_LIVE_BIN_CORE_H_
