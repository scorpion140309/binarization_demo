//
#ifndef SC_BINARIZATION_BASE_H_
#define SC_BINARIZATION_BASE_H_

namespace sc
{
	namespace Binarization
	{
		class Base
		{
		private:
			size_t width_;
			size_t height_;
			unsigned char pix_min_;
			unsigned char pix_max_;

			void Create_(size_t a_width, size_t a_height);
			void Destroy_(void);
		public:
			Base(void);
			Base(size_t a_width, size_t a_height);
			virtual ~Base(void);
			void Reset(size_t a_width, size_t a_height);
			size_t Width(void) const;
			size_t Height(void) const;
			unsigned char PixMin(void) const;
			unsigned char PixMax(void) const;

			virtual int Exe(const unsigned char* a_p_src_img, unsigned char* a_p_dst_img, size_t a_line_bytesize) = 0;
		};
	}
}

#endif  //SC_BINARIZATION_BASE_H_
