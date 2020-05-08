//
#include <Binarization/scBinarizationThresholdMatrix.h>
#include <Binarization/scHolladay.h>

//
namespace
{
	const unsigned char DEFAULT_THRESHOLD = 128;
	const int DEFAULT_VEC_X = 10;
	const int DEFAULT_VEC_Y = 10;
	const int DEFAULT_DIV = 1;
}

//
sc::Binarization::ThresholdMatrix::ThresholdMatrix(size_t a_width, size_t a_height) : Base(a_width, a_height)
{
	this->Create_();
	return;
}

//
sc::Binarization::ThresholdMatrix::~ThresholdMatrix(void)
{
	this->Destroy_();
	return;
}

//
void sc::Binarization::ThresholdMatrix::Create_(void)
{
	if (this->p_hollday_)
	{
		delete this->p_hollday_;
	}
	this->p_hollday_ = new sc::Binarization::Holladay(DEFAULT_VEC_X, DEFAULT_VEC_Y, DEFAULT_DIV);
	return;
}
//
void sc::Binarization::ThresholdMatrix::Destroy_(void)
{
	if (this->p_hollday_)
	{
		delete this->p_hollday_;
		this->p_hollday_ = nullptr;
	}
	return;
}

//
int sc::Binarization::ThresholdMatrix::ExeCore_(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize)
{
	int ret_result = -1;
	size_t width = this->Width();
	size_t height = this->Height();
	if (width > 0 && height > 0)
	{
		unsigned char pix_min = this->PixMin();
		unsigned char pix_max = this->PixMax();
		size_t y;
		for (y = 0; y < height; y++)
		{
			const unsigned char* p_current_src_line = a_p_src_img + y * a_line_bytesize;
			unsigned char* p_current_dst_line = a_p_dst_img + y * a_line_bytesize;
			size_t x;
			for (x = 0; x < width; x++)
			{
				int thresh = 1;
				if (p_current_src_line[x] > thresh)
				{
					p_current_dst_line[x] = pix_max;
				}
				else
				{
					p_current_dst_line[x] = pix_min;
				}
			}
		}
		ret_result = 0;
	}
	return ret_result;
}

//
int sc::Binarization::ThresholdMatrix::Exe(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize)
{
	int ret_result = this->ExeCore_(a_p_src_img, a_p_dst_img, a_line_bytesize);
	return ret_result;
}

