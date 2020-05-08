//
#include <Binarization/scBinarizationThresholdMatrix.h>
#include <Binarization/scHolladay.h>

//
namespace
{
	const int DEFAULT_VEC_X = 10;
	const int DEFAULT_VEC_Y = 5;
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
		int pix_num = static_cast<int>(this->p_hollday_->Width() * this->p_hollday_->Height());
		std::vector<int> ary_scaled_th;
		int range = static_cast<int>(pix_max) - pix_min;
		int t;
		for (t = 0; t < pix_num; t++)
		{
			int th = ((t * (pix_max - 1)) / pix_num) + 1;
			ary_scaled_th.push_back(th);
		}

		long long tile_h = this->p_hollday_->Height();
		const int *p_th_mtx = this->p_hollday_->MatrixPtrCst();
		long long current_y = 0;
		long long current_shift = 0;
		size_t y;
		for (y = 0; y < height; y++)
		{
			long long tile_w = this->p_hollday_->Width();
			long long shift = this->p_hollday_->Shift();

			const unsigned char* p_current_src_line = a_p_src_img + y * a_line_bytesize;
			unsigned char* p_current_dst_line = a_p_dst_img + y * a_line_bytesize;

			//
			const int* p_th_mtx_line = p_th_mtx + current_y * tile_w;
			long long current_x = current_shift;

			size_t x;
			for (x = 0; x < width; x++)
			{
				int matrix_id = p_th_mtx_line[current_x];
				int thresh = ary_scaled_th[matrix_id];
				if (p_current_src_line[x] > thresh)
				{
					p_current_dst_line[x] = pix_max;
				}
				else
				{
					p_current_dst_line[x] = pix_min;
				}

				//
				current_x++;
				if (current_x >= tile_w)
				{
					current_x = 0;
				}
			}
			//
			current_y++;
			if (current_y >= tile_h)
			{
				current_y = 0;
				current_shift += shift;
				if (current_shift >= tile_w)
				{
					current_shift = 0;
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

