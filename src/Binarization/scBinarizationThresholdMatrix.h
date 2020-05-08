//
#ifndef SC_BINARIZATION_THRESHOLD_MATRIX_H_
#define SC_BINARIZATION_THRESHOLD_MATRIX_H_

#include <Binarization/scBinarizationBase.h>

namespace sc
{
	namespace Binarization
	{
		// forward declaration
		class Holladay;

		class ThresholdMatrix : public Base
		{
		private:
			sc::Binarization::Holladay* p_hollday_;
			void Create_(void);
			void Destroy_(void);

			int ExeCore_(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize);
		public:
			ThresholdMatrix(size_t a_width, size_t a_height);
			~ThresholdMatrix(void);
			int Exe(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize);
		};
	}
}

#endif  //SC_BINARIZATION_THRESHOLD_MATRIX_H_
