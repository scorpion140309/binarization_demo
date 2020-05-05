//
#ifndef SC_BINARIZATION_FIXED_THRESHOLD_H_
#define SC_BINARIZATION_FIXED_THRESHOLD_H_

#include <Binarization/scBinarizationBase.h>

namespace sc
{
	namespace Binarization
	{
		class FixedThreshold : public Base
		{
		private:
			unsigned char threshold_;
			int ExeCore_(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize);
		public:
			FixedThreshold(size_t a_width, size_t a_height);
			int Exe(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize);
		};
	}
}

#endif  //SC_BINARIZATION_FIXED_THRESHOLD_H_
