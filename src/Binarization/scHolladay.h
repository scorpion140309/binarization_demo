//
#ifndef SC_HOLLADAY_H_
#define SC_HOLLADAY_H_

//
#include <Binarization/scBinarizationBase.h>

//
#include <vector>
#include <cmath>
//#include <cstdlib>

//
namespace sc
{
	struct VecI
	{
	public:
		long long x;
		long long y;
		long long div;
	public:
		//
		double Lenght(void) const
		{
			double ret_len = (std::sqrt(this->Lenght2()));
			return ret_len;
		}
		//
		long long Lenght2(void) const
		{
			long long ret_len2 = this->x * this->x + this->y * this->y;
			return ret_len2;
		}
		//
		double Cos(void) const
		{
			double ret_cos = 0.0;
			double len = this->Lenght();
			if (len)
			{
				ret_cos = static_cast<double>(this->x) / len;
			}
			return ret_cos;
		}
		//
		double Sin(void) const
		{
			double ret_cos = 0.0;
			double len = this->Lenght();
			if (len)
			{
				ret_cos = static_cast<double>(this->y) / len;
			}
			return ret_cos;
		}
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

			long long Width(void) const;
			long long Height(void) const;
			long long Shift(void) const;
			const int *MatrixPtrCst(void) const;
			long long TileSize(void) const;
		};
	}
}

#endif  //SC_HOLLADAY_H_
