//
#include <Binarization/scBinarizationErrorDiffusion.h>

//
#include <vector>

//
namespace
{
	const unsigned char DEFAULT_THRESHOLD = 128;
}

//
sc::Binarization::ErrorDiffusion::ErrorDiffusion(size_t a_width, size_t a_height) : Base(a_width, a_height)
{
	this->threshold_ = DEFAULT_THRESHOLD;
	return;
}

//
int sc::Binarization::ErrorDiffusion::ExeCore_(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize)
{
	int ret_result = -1;
	size_t width = this->Width();
	size_t height = this->Height();
	if (width > 0 && height > 0)
	{
		const int ERR_LINE_NUM = 2;
		const int margin_x = 1;
		size_t ERR_LINE_W = width + 2 * (size_t)margin_x;
		std::vector < std::vector<int>> ary2_err_line(ERR_LINE_NUM);

		// error lines are initialized.
		std::vector < std::vector<int>>::iterator itr;
		for (itr = ary2_err_line.begin(); itr != ary2_err_line.end(); itr++)
		{
			itr->resize(ERR_LINE_W);
		}

		//
		unsigned char pix_min = this->PixMin();
		unsigned char pix_max = this->PixMax();
		size_t y;
		for (y = 0; y < height; y++)
		{
			std::vector<int*> ary_p_err_line(ERR_LINE_NUM);
			int i;
			for (i = 0; i < ERR_LINE_NUM; i++)
			{
				ary_p_err_line[i] = &(ary2_err_line[i][0]);
			}

			const unsigned char* p_current_src_line = a_p_src_img + y * a_line_bytesize;
			unsigned char* p_current_dst_line = a_p_dst_img + y * a_line_bytesize;
			size_t x;
			for (x = 0; x < width; x++)
			{

				//
				const int SHIFT = 4;

				// pixel value
				int pix_value_org = p_current_src_line[x];

				// [0,255] -> [0,4080]
				// (for calculation accuracy)
				int pix_value = pix_value_org << SHIFT;

				// adaptive threshold
				//int threshold = this->ary_threshold_[pix_value_org] << SHIFT;
				// normal threshold
				int threshold = 128 << SHIFT;

				// add error
				int err_scaled = pix_value + ary_p_err_line[0][x + margin_x];

				// compare
				if (err_scaled > threshold)
				{
					p_current_dst_line[x] = pix_max;
					err_scaled -= pix_max << SHIFT;
				}
				else
				{
					p_current_dst_line[x] = pix_min;
				}

				// error diffusion
				//	error = src - dst
				const int ERR_COEF[ERR_LINE_NUM][3] = { {0, 0, 5}, {3, 5, 3} };
				int ly;
				for (ly = 0; ly < ERR_LINE_NUM; ly++)
				{
					int lx;
					for (lx = 0; lx < 3; lx++)
					{
						ary_p_err_line[ly][x + lx] += (err_scaled * ERR_COEF[ly][lx]) >> SHIFT;
					}
				}
			}

			// post line proc
			{
				size_t lx, ly;
				for (ly = ERR_LINE_NUM - 1; ly > 0; ly--)
				{
					for (lx = 0; lx < ERR_LINE_W; lx++)
					{
						ary2_err_line[ly - 1][lx] = ary2_err_line[ly][lx];
					}
				}
				for (lx = 0; lx < ERR_LINE_W; lx++)
				{
					ary2_err_line[ERR_LINE_NUM - 1][lx] = 0;
				}
			}

		}
	}
	return ret_result;
}

//
int sc::Binarization::ErrorDiffusion::Exe(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize)
{
	int ret_result = this->ExeCore_(a_p_src_img, a_p_dst_img, a_line_bytesize);
	return ret_result;
}

