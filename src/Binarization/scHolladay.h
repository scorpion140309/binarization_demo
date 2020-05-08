//
#ifndef SC_HOLLADAY_H_
#define SC_HOLLADAY_H_

//
#include <Binarization/scBinarizationBase.h>

//
#include <vector>

//
namespace sc
{
	struct VecI
	{
		int x;
		int y;
		int div;
	};
	namespace Binarization
	{
		class Holladay
		{
		private:
			sc::VecI ary_vec_[2];
			long long tile_w_;
			long long tile_h_;
			long long shift_;
			std::vector<int> ary_threh_;


			//
			void Create_(int a_x, int a_y, int a_div);
			void Destroy_(void);

			//
			void MakeTile_(void);
			void MakeTile_WHS_(void);
			void MakeTile_ThresholdMatrix_(void);

		public:
			Holladay(void);
			Holladay(int a_x, int a_y, int a_div);
			~Holladay(void);
		};
	}
}

#endif  //SC_HOLLADAY_H_
