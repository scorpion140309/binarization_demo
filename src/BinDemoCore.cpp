//
#include "BinDemoCore.h"
//#include "BinDemoEnum.h"

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
#include <Binarization/scBinarizationThresholdMatrix.h>

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


///////////////////////////////////////////////////////////////
//
// constructor, destructor
//
///////////////////////////////////////////////////////////////

//
sc::BinCore::BinCore(void)
{
	this->Create_();
	return;
}

//
sc::BinCore::~BinCore(void)
{
	this->Destroy_();
}

//----------------------------------------------------------------
//
// private
//
//----------------------------------------------------------------

//
void sc::BinCore::Create_(void)
{
	size_t num = static_cast<size_t>(sc::BinCore::img_type::_NUM_);
	size_t i;
	for (i = 0; i < num; i++)
	{
		this->ary_img_.push_back(cv::Mat());
	}

	// State Machine Funcitons
	this->dict_st_functions_[static_cast<int>(sc::BinDemo::stat::Start)] = &BinCore::FuncStart_;
	this->dict_st_functions_[static_cast<int>(sc::BinDemo::stat::End)] = &BinCore::FuncEnd_;
	this->dict_st_functions_[static_cast<int>(sc::BinDemo::stat::Init)] = &BinCore::FuncInit_;
	this->dict_st_functions_[static_cast<int>(sc::BinDemo::stat::Idle)] = &BinCore::FuncIdle_;
	this->dict_st_functions_[static_cast<int>(sc::BinDemo::stat::Do)] = &BinCore::FuncDo_;
	this->dict_st_functions_[static_cast<int>(sc::BinDemo::stat::Done)] = &BinCore::FuncDone_;
	this->dict_st_functions_[static_cast<int>(sc::BinDemo::stat::Reset)] = &BinCore::FuncReset_;
	this->dict_st_functions_[static_cast<int>(sc::BinDemo::stat::Fail)] = &BinCore::FuncFail_;
	this->dict_st_functions_[static_cast<int>(sc::BinDemo::stat::Close)] = &BinCore::FuncClose_;
	
	//
	this->key_ = -1;
	this->key_delay_ = 3;


	// Binalizing(Halftoning) Funcitons
	//this->halftoning_type_ = sc::BinCore::ht_type::e_FIXED_THRESH;
	//this->halftoning_type_ = sc::BinCore::ht_type::e_RANDOM_THRESH;
	//this->halftoning_type_ = sc::BinCore::ht_type::e_ERR_DIFFUSION;
	this->halftoning_type_ = sc::BinCore::ht_type::e_THRESH_MATRIX;
	this->dict_bin_functions_[static_cast<int>(sc::BinCore::ht_type::e_FIXED_THRESH)] = &BinCore::Binaliztion_FixedThresh_;
	this->dict_bin_functions_[static_cast<int>(sc::BinCore::ht_type::e_RANDOM_THRESH)] = &BinCore::Binaliztion_RandomThresh_;
	this->dict_bin_functions_[static_cast<int>(sc::BinCore::ht_type::e_ERR_DIFFUSION)] = &BinCore::Binaliztion_ErrorDiffusion_;
	this->dict_bin_functions_[static_cast<int>(sc::BinCore::ht_type::e_THRESH_MATRIX)] = &BinCore::Binaliztion_ThreshMatrix_;

	//
	this->ary_filenames_.clear();
	//this->ary_filenames_.push_back("input.jpg");
	this->ary_filenames_.push_back("flower.png");
	//this->ary_filenames_.push_back("sample.png");
	this->ary_filenames_.push_back("gray.png");
	this->ary_filenames_.push_back("dst.png");

	return;
}

//
void sc::BinCore::Destroy_(void)
{
	std::vector<cv::Mat>::iterator itr;
	this->ary_img_.clear();
	return;
}

//
cv::Mat* sc::BinCore::GetImgPtr_(sc::BinCore::img_type a_type)
{
	cv::Mat* ret_ptr = nullptr;
	size_t id = static_cast<size_t>(a_type);
	if (id < static_cast<size_t>(sc::BinCore::img_type::_NUM_))
	{
		ret_ptr = &(this->ary_img_[id]);
	}

	return ret_ptr;
}



//
sc::BinDemo::evt sc::BinCore::FuncStart_(void)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	return ret_evt;
}

//
sc::BinDemo::evt sc::BinCore::FuncEnd_(void)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::_NOTHING_;

	return ret_evt;
}

//
sc::BinDemo::evt sc::BinCore::FuncInit_(void)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	//
	cv::destroyAllWindows();

	auto id_src = static_cast<size_t>(sc::BinCore::img_type::e_SRC);
	this->ary_img_[id_src] = cv::imread(this->ary_filenames_[0]);
	if (this->ary_img_[id_src].empty() == false)
	{
		//
		auto id_gray = static_cast<size_t>(sc::BinCore::img_type::e_GRAY);
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

		auto id_dst = static_cast<size_t>(sc::BinCore::img_type::e_DST);
		this->ary_img_[id_dst] = cv::Mat(this->ary_img_[id_gray].rows, this->ary_img_[id_gray].cols, CV_8UC1);
		this->ary_img_[id_dst] = cv::Scalar::all(PIX_INIT);

		//
		cv::imshow("src", this->ary_img_[id_src]);
		cv::imshow("gray", this->ary_img_[id_gray]);
		cv::imshow("dst", this->ary_img_[id_dst]);

		ret_evt = sc::BinDemo::evt::GoNext;
	}
	else
	{
		ret_evt = sc::BinDemo::evt::GoFail;
	}
	return ret_evt;
}
//
sc::BinDemo::evt sc::BinCore::FuncIdle_(void)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	switch (this->key_)
	{
	case cvkey::ESC:
	case 'q':
		ret_evt = sc::BinDemo::evt::GoExit;
		break;
	case 'r':
	case 'R':
		ret_evt = sc::BinDemo::evt::GoReset;
		break;
	case ' ':
		ret_evt = sc::BinDemo::evt::GoNext;
		break;
	default:
		ret_evt = sc::BinDemo::evt::Stay;
		break;
	}

	return ret_evt;
}
//
sc::BinDemo::evt sc::BinCore::FuncDo_(void)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	cv::Mat* p_img_gray = (this->GetImgPtr_(sc::BinCore::img_type::e_GRAY));
	cv::Mat *p_img_dst = (this->GetImgPtr_(sc::BinCore::img_type::e_DST));

	//
	*p_img_dst = cv::Scalar::all(PIX_INIT);

	//
	ret_evt = (this->*dict_bin_functions_[static_cast<int>(this->halftoning_type_)])(p_img_gray, p_img_dst);

	if (ret_evt == sc::BinDemo::evt::Stay)
	{
		switch (this->key_)
		{
		case cvkey::ESC:
		case 'q':
			ret_evt = sc::BinDemo::evt::GoExit;
			break;
		default:
			ret_evt = sc::BinDemo::evt::Stay;
			break;
		}
	}

	//
	cv::imshow("dst", *p_img_dst);

	return ret_evt;
}

//
sc::BinDemo::evt sc::BinCore::FuncDone_(void)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	//
	switch (this->halftoning_type_)
	{
	case sc::BinCore::ht_type::e_FIXED_THRESH:
		this->halftoning_type_ = sc::BinCore::ht_type::e_RANDOM_THRESH;
		break;
	case sc::BinCore::ht_type::e_RANDOM_THRESH:
		this->halftoning_type_ = sc::BinCore::ht_type::e_ERR_DIFFUSION;
		break;
	case sc::BinCore::ht_type::e_ERR_DIFFUSION:
		this->halftoning_type_ = sc::BinCore::ht_type::e_THRESH_MATRIX;
		break;
	case sc::BinCore::ht_type::e_THRESH_MATRIX:
		this->halftoning_type_ = sc::BinCore::ht_type::e_FIXED_THRESH;
		break;
	}

	//
	auto id_dst = static_cast<size_t>(sc::BinCore::img_type::e_DST);
	cv::imwrite(this->ary_filenames_[id_dst], this->ary_img_[id_dst]);

	return ret_evt;
}
//
sc::BinDemo::evt sc::BinCore::FuncReset_(void)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	return ret_evt;
}
//
sc::BinDemo::evt sc::BinCore::FuncFail_(void)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	return ret_evt;
}

//
sc::BinDemo::evt sc::BinCore::FuncClose_(void)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	cv::destroyAllWindows();

	return ret_evt;
}

//
sc::BinDemo::evt sc::BinCore::Binaliztion_FixedThresh_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	//
	sc::Binarization::FixedThreshold bin(a_p_img_src->cols, a_p_img_src->rows);
	bin.Exe(a_p_img_src->data, a_p_img_dst->data, a_p_img_src->step);

	return ret_evt;
}

//
sc::BinDemo::evt sc::BinCore::Binaliztion_RandomThresh_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	//
	sc::Binarization::RandomThreshold bin(a_p_img_src->cols, a_p_img_src->rows);
	bin.Exe(a_p_img_src->data, a_p_img_dst->data, a_p_img_src->step);

	return ret_evt;
}

//
sc::BinDemo::evt sc::BinCore::Binaliztion_ErrorDiffusion_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	//
	sc::Binarization::ErrorDiffusion bin(a_p_img_src->cols, a_p_img_src->rows);
	bin.Exe(a_p_img_src->data, a_p_img_dst->data, a_p_img_src->step);

	return ret_evt;
}


//
sc::BinDemo::evt sc::BinCore::Binaliztion_ThreshMatrix_(const cv::Mat* a_p_img_src, cv::Mat* a_p_img_dst)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	//
	sc::Binarization::ThresholdMatrix bin(a_p_img_src->cols, a_p_img_src->rows);
	bin.Exe(a_p_img_src->data, a_p_img_dst->data, a_p_img_src->step);

	return ret_evt;
}

//
void sc::BinCore::CommonProc_(void)
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
sc::BinDemo::evt sc::BinCore::FuncIF(sc::BinDemo::stat a_stat)
{
	sc::BinDemo::evt ret_evt = sc::BinDemo::evt::GoNext;

	//
	this->CommonProc_();

	//
	ret_evt = (this->*dict_st_functions_[static_cast<int>(a_stat)])();

	return ret_evt;
}
