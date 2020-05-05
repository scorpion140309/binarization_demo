//
#include <Binarization/scBinarizationRandomThreshold.h>

//
namespace
{
	const unsigned char DEFAULT_THRESHOLD = 128;
	//
	const unsigned int ARY_RAND_INI_[4] = { 0xD9363228, 0x021A79C9, 0x7604820B, 0xD18E1939 };
}

//
sc::Binarization::RandomThreshold::RandomThreshold(size_t a_width, size_t a_height) : Base(a_width, a_height)
{
	//
	this->ResetRand_();
	return;
}

//
void sc::Binarization::RandomThreshold::ResetRand_(void)
{
	//
	size_t i;
	for (i = 0; i < 4; i++)
	{
		this->ary_rand_tmp_[i] = ARY_RAND_INI_[i];
	}
	return;
}

//
unsigned int sc::Binarization::RandomThreshold::GetRand_U32_(void)
{
	unsigned int ret_rand = 0;
	unsigned int t;

	t = this->ary_rand_tmp_[0] ^ (this->ary_rand_tmp_[0] << 11);
	this->ary_rand_tmp_[0] = this->ary_rand_tmp_[1];
	this->ary_rand_tmp_[1] = this->ary_rand_tmp_[2]; this->ary_rand_tmp_[2] = this->ary_rand_tmp_[3];
	this->ary_rand_tmp_[3] = (this->ary_rand_tmp_[3] ^ (this->ary_rand_tmp_[3] >> 19)) ^ (t ^ (t >> 8));
	ret_rand = this->ary_rand_tmp_[3];

	return ret_rand;
}

//
unsigned char sc::Binarization::RandomThreshold::GetRand_U08_(void)
{
	unsigned char ret_rand_u08 = 0;
	unsigned int rand_u32 = 0;

	rand_u32 = this->GetRand_U32_();
	ret_rand_u08 = static_cast<unsigned char>(rand_u32 & 0xff);

	return ret_rand_u08;
}
//
int sc::Binarization::RandomThreshold::ExeCore_(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize)
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
				unsigned char thresh = this->GetRand_U08_();
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
int sc::Binarization::RandomThreshold::Exe(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize)
{
	int ret_result = this->ExeCore_(a_p_src_img, a_p_dst_img, a_line_bytesize);
	return ret_result;
}

