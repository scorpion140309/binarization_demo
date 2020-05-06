//
#ifndef SC_LIVE_BIN_CORE_H_
#define SC_LIVE_BIN_CORE_H_

//
#include "BinDemoEnum.h"

//
#include <vector>
#include <map>
#include <string>

namespace cv
{
	class Mat;
}

namespace sc
{
	class BinCore
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
			e_THRESH_MATRIX,
			_NUM_
		};

	private:
		std::vector<cv::Mat> ary_img_;
		std::map<int, sc::BinDemo::evt (sc::BinCore::*)()> dict_st_functions_;

		// cv
		int key_;
		int key_delay_;

		//
		std::vector<std::string> ary_filenames_;

	private:
		void Create_(void);
		void Destroy_(void);

		//
		cv::Mat* GetImgPtr_(sc::BinCore::img_type);

		//
		sc::BinDemo::evt FuncStart_(void);
		sc::BinDemo::evt FuncEnd_(void);
		sc::BinDemo::evt FuncInit_(void);
		sc::BinDemo::evt FuncIdle_(void);
		sc::BinDemo::evt FuncDo_(void);
		sc::BinDemo::evt FuncDone_(void);
		//sc::LiveBin::evt FuncPause_(void);
		sc::BinDemo::evt FuncReset_(void);
		sc::BinDemo::evt FuncFail_(void);
		sc::BinDemo::evt FuncClose_(void);

		//
		sc::BinCore::ht_type halftoning_type_;
		std::map<int, sc::BinDemo::evt(sc::BinCore::*)(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst)> dict_bin_functions_;
		sc::BinDemo::evt Binaliztion_FixedThresh_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst);
		sc::BinDemo::evt Binaliztion_RandomThresh_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst);
		sc::BinDemo::evt Binaliztion_ErrorDiffusion_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst);
		sc::BinDemo::evt Binaliztion_ThreshMatrix_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst);

		//
		void CommonProc_(void);

	public:
		// 
		BinCore(void);
		// 
		~BinCore(void);

		//
		sc::BinDemo::evt FuncIF(sc::BinDemo::stat a_stat);
	};
}

#endif  //SC_LIVE_BIN_CORE_H_
