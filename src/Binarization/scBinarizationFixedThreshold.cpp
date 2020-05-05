//
#include <Binarization/scBinarizationFixedThreshold.h>

//
namespace
{
	const unsigned char DEFAULT_THRESHOLD = 128;
}

//
sc::Binarization::FixedThreshold::FixedThreshold(size_t a_width, size_t a_height) : Base(a_width, a_height)
{
	this->threshold_ = DEFAULT_THRESHOLD;
	return;
}

//
int sc::Binarization::FixedThreshold::ExeCore_(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize)
{
	int ret_result = -1;
	size_t width = this->Width();
	size_t height = this->Height();
	if (width > 0 && height > 0)
	{
		unsigned char thresh = this->threshold_;
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
	}
	return ret_result;
}

//
int sc::Binarization::FixedThreshold::Exe(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize)
{
	int ret_result = this->ExeCore_(a_p_src_img, a_p_dst_img, a_line_bytesize);
	return ret_result;
}

