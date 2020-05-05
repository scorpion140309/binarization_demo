//
#include "LiveBinCore.h"
//#include "LiveBinEnum.h"

//
#include <vector>
#include <algorithm>

// opencv
#pragma warning(push)
#pragma warning ( disable : 4819 )
#include <opencv2/opencv.hpp>
#pragma warning(pop)

//
#include <Binarization/scBinarizationFixedThreshold.h>
#include <Binarization/scBinarizationRandomThreshold.h>
#include <Binarization/scBinarizationErrorDiffusion.h>

//
namespace
{
	constexpr unsigned char PIX_INIT = 64;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
// non-member functions
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//
static unsigned int sttc_my_rand(void)
{
	static unsigned int x = 0xD9363228;
	static unsigned int y = 0x021A79C9;
	static unsigned int z = 0x7604820B;
	static unsigned int w = 0xD18E1939;
	unsigned int t;

	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

///////////////////////////////////////////////////////////////
//
// constructor, destructor
//
///////////////////////////////////////////////////////////////

//
sc::LiveBinCore::LiveBinCore(void)
{
	this->Create_();
	return;
}

//
sc::LiveBinCore::~LiveBinCore(void)
{
	this->Destroy_();
}

//----------------------------------------------------------------
//
// private
//
//----------------------------------------------------------------

//
void sc::LiveBinCore::Create_(void)
{
	size_t num = static_cast<size_t>(sc::LiveBinCore::img_type::_NUM_);
	size_t i;
	for (i = 0; i < num; i++)
	{
		this->ary_img_.push_back(cv::Mat());
	}

	// State Machine Funcitons
	this->dict_st_functions_[static_cast<int>(sc::LiveBin::stat::Start)] = &LiveBinCore::FuncStart_;
	this->dict_st_functions_[static_cast<int>(sc::LiveBin::stat::End)] = &LiveBinCore::FuncEnd_;
	this->dict_st_functions_[static_cast<int>(sc::LiveBin::stat::Init)] = &LiveBinCore::FuncInit_;
	this->dict_st_functions_[static_cast<int>(sc::LiveBin::stat::Idle)] = &LiveBinCore::FuncIdle_;
	this->dict_st_functions_[static_cast<int>(sc::LiveBin::stat::Do)] = &LiveBinCore::FuncDo_;
	this->dict_st_functions_[static_cast<int>(sc::LiveBin::stat::Done)] = &LiveBinCore::FuncDone_;
	this->dict_st_functions_[static_cast<int>(sc::LiveBin::stat::Reset)] = &LiveBinCore::FuncReset_;
	this->dict_st_functions_[static_cast<int>(sc::LiveBin::stat::Fail)] = &LiveBinCore::FuncFail_;
	this->dict_st_functions_[static_cast<int>(sc::LiveBin::stat::Close)] = &LiveBinCore::FuncClose_;
	
	//
	this->key_ = -1;
	this->key_delay_ = 3;

	// array of threshold
	this->ary_threshold_.clear();
	this->ary_threshold_.push_back(0);
	int t;
	for (t = 1; t < 255; t++)
	{
		this->ary_threshold_.push_back(0);
		//this->ary_threshold_.push_back(2 * t - 256);
		//this->ary_threshold_.push_back(127 - t);
		//this->ary_threshold_.push_back(t - 127);
		//this->ary_threshold_.push_back(t-128);
	}
	this->ary_threshold_.push_back(0);

	// Binalizing(Halftoning) Funcitons
	//this->halftoning_type_ = sc::LiveBinCore::ht_type::e_FIXED_THRESH;
	//this->halftoning_type_ = sc::LiveBinCore::ht_type::e_RANDOM_THRESH;
	this->halftoning_type_ = sc::LiveBinCore::ht_type::e_ERR_DIFFUSION;
	this->dict_bin_functions_[static_cast<int>(sc::LiveBinCore::ht_type::e_FIXED_THRESH)] = &LiveBinCore::Binaliztion_FixedThresh_;
	this->dict_bin_functions_[static_cast<int>(sc::LiveBinCore::ht_type::e_RANDOM_THRESH)] = &LiveBinCore::Binaliztion_RandomThresh_;
	this->dict_bin_functions_[static_cast<int>(sc::LiveBinCore::ht_type::e_ERR_DIFFUSION)] = &LiveBinCore::Binaliztion_ErrorDiffusion_;

	//
	this->ary_filenames_.clear();
	this->ary_filenames_.push_back("input.jpg");
	//this->ary_filenames_.push_back("input.png");
	this->ary_filenames_.push_back("gray.png");
	this->ary_filenames_.push_back("dst.png");

	//
	this->count_x_ = 0;
	this->count_y_ = 0;

	return;
}

//
void sc::LiveBinCore::Destroy_(void)
{
	std::vector<cv::Mat>::iterator itr;
	for (itr = this->ary_img_.begin(); itr != this->ary_img_.end(); itr++)
	{
		//delete (*itr);
		//*itr = nullptr;
	}
	this->ary_img_.clear();
	return;
}

//
cv::Mat* sc::LiveBinCore::GetImgPtr_(sc::LiveBinCore::img_type a_type)
{
	cv::Mat* ret_ptr = nullptr;
	size_t id = static_cast<size_t>(a_type);
	if (id < static_cast<size_t>(sc::LiveBinCore::img_type::_NUM_))
	{
		ret_ptr = &(this->ary_img_[id]);
	}

	return ret_ptr;
}



//
sc::LiveBin::evt sc::LiveBinCore::FuncStart_(void)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;

	return ret_evt;
}

//
sc::LiveBin::evt sc::LiveBinCore::FuncEnd_(void)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::_NOTHING_;

	return ret_evt;
}

//
sc::LiveBin::evt sc::LiveBinCore::FuncInit_(void)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;

	//
	cv::destroyAllWindows();

	auto id_src = static_cast<size_t>(sc::LiveBinCore::img_type::e_SRC);
	this->ary_img_[id_src] = cv::imread(this->ary_filenames_[0]);
	if (this->ary_img_[id_src].empty() == false)
	{
		//
		auto id_gray = static_cast<size_t>(sc::LiveBinCore::img_type::e_GRAY);
		if (this->ary_img_[id_src].channels() == 1)
		{
			this->ary_img_[id_src].copyTo(this->ary_img_[id_gray]);
		}
		else
		{
			cv::cvtColor(this->ary_img_[id_src], this->ary_img_[id_gray], cv::COLOR_BGR2GRAY);
		}
		// @@@
		//cv::imwrite("gray.png", this->ary_img_[id_gray]);

		auto id_dst = static_cast<size_t>(sc::LiveBinCore::img_type::e_DST);
		this->ary_img_[id_dst] = cv::Mat(this->ary_img_[id_gray].rows, this->ary_img_[id_gray].cols, CV_8UC1);
		this->ary_img_[id_dst] = cv::Scalar::all(PIX_INIT);

		//
		cv::imshow("src", this->ary_img_[id_src]);
		cv::imshow("gray", this->ary_img_[id_gray]);
		cv::imshow("dst", this->ary_img_[id_dst]);

		ret_evt = sc::LiveBin::evt::GoNext;
	}
	else
	{
		ret_evt = sc::LiveBin::evt::GoFail;
	}
	return ret_evt;
}
//
sc::LiveBin::evt sc::LiveBinCore::FuncIdle_(void)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;

	switch (this->key_)
	{
	case cvkey::ESC:
	case 'q':
		ret_evt = sc::LiveBin::evt::GoExit;
		break;
	case 'r':
	case 'R':
		ret_evt = sc::LiveBin::evt::GoReset;
		break;
	case ' ':
		ret_evt = sc::LiveBin::evt::GoNext;
		break;
	default:
		ret_evt = sc::LiveBin::evt::Stay;
		break;
	}

	return ret_evt;
}
//
sc::LiveBin::evt sc::LiveBinCore::FuncDo_(void)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;

	cv::Mat* p_img_gray = (this->GetImgPtr_(sc::LiveBinCore::img_type::e_GRAY));
	cv::Mat *p_img_dst = (this->GetImgPtr_(sc::LiveBinCore::img_type::e_DST));

	if (this->count_x_ == 0 && this->count_y_ == 0)
	{
		*p_img_dst = cv::Scalar::all(PIX_INIT);
	}

	//
	ret_evt = (this->*dict_bin_functions_[static_cast<int>(this->halftoning_type_)])(p_img_gray, p_img_dst);

	if (ret_evt == sc::LiveBin::evt::Stay)
	{
		switch (this->key_)
		{
		case cvkey::ESC:
		case 'q':
			ret_evt = sc::LiveBin::evt::GoExit;
			break;
		default:
			ret_evt = sc::LiveBin::evt::Stay;
			break;
		}
	}

	//
	cv::imshow("dst", *p_img_dst);

	return ret_evt;
}

//
sc::LiveBin::evt sc::LiveBinCore::FuncDone_(void)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;

	//
	switch (this->halftoning_type_)
	{
	case sc::LiveBinCore::ht_type::e_FIXED_THRESH:
		this->halftoning_type_ = sc::LiveBinCore::ht_type::e_RANDOM_THRESH;
		break;
	case sc::LiveBinCore::ht_type::e_RANDOM_THRESH:
		this->halftoning_type_ = sc::LiveBinCore::ht_type::e_ERR_DIFFUSION;
		break;
	case sc::LiveBinCore::ht_type::e_ERR_DIFFUSION:
		this->halftoning_type_ = sc::LiveBinCore::ht_type::e_FIXED_THRESH;
		break;
	}

	//
	auto id_dst = static_cast<size_t>(sc::LiveBinCore::img_type::e_DST);
	cv::imwrite(this->ary_filenames_[id_dst], this->ary_img_[id_dst]);

	return ret_evt;
}
//
//sc::LiveBin::evt sc::LiveBinCore::FuncPause_(void)
//{
//	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;
//
//	return ret_evt;
//}
//
sc::LiveBin::evt sc::LiveBinCore::FuncReset_(void)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;

	return ret_evt;
}
//
sc::LiveBin::evt sc::LiveBinCore::FuncFail_(void)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;

	return ret_evt;
}

//
sc::LiveBin::evt sc::LiveBinCore::FuncClose_(void)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;

	cv::destroyAllWindows();

	return ret_evt;
}

//
sc::LiveBin::evt sc::LiveBinCore::Binaliztion_FixedThresh_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;

	//
	sc::Binarization::FixedThreshold bin(a_p_img_src->cols, a_p_img_src->rows);
	bin.Exe(a_p_img_src->data, a_p_img_dst->data, a_p_img_src->step);

	return ret_evt;
}

//
sc::LiveBin::evt sc::LiveBinCore::Binaliztion_RandomThresh_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;

	//
	sc::Binarization::RandomThreshold bin(a_p_img_src->cols, a_p_img_src->rows);
	bin.Exe(a_p_img_src->data, a_p_img_dst->data, a_p_img_src->step);

	return ret_evt;
}

//
sc::LiveBin::evt sc::LiveBinCore::Binaliztion_ErrorDiffusion_(const cv::Mat* a_p_img_src, cv::Mat * a_p_img_dst)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;

	//
	sc::Binarization::ErrorDiffusion bin(a_p_img_src->cols, a_p_img_src->rows);
	bin.Exe(a_p_img_src->data, a_p_img_dst->data, a_p_img_src->step);

	return ret_evt;
}

//
void sc::LiveBinCore::CommonProc_(void)
{
	this->key_ = cv::waitKeyEx(this->key_delay_);

	return;
}



///////////////////////////////////////////////////////
//
// public
//
///////////////////////////////////////////////////////

//
sc::LiveBin::evt sc::LiveBinCore::FuncIF(sc::LiveBin::stat a_stat)
{
	sc::LiveBin::evt ret_evt = sc::LiveBin::evt::GoNext;

	//
	this->CommonProc_();

	//
	ret_evt = (this->*dict_st_functions_[static_cast<int>(a_stat)])();

	return ret_evt;
}
