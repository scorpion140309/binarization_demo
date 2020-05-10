//
#include <Binarization/scHolladay.h>

// floor
#include <cmath>

//
#include <vector>
#include <algorithm>


// @@@
#include <iostream>

//
namespace
{
	long long st_scale = 5;
}

//
enum class DOT_COL
{
	e_BLACK = 0,
	e_WHITE
};

// - - - - - - - - - - - - - - - - - - - - -
//
// not member functions
//
// - - - - - - - - - - - - - - - - - - - - -

// this funciton retunrs gcd(a_val_x, a_val_y).
// (*a_p_coef_a) * a_val_x + (*a_r_coef_b) * a_val_y = gcd(a_val_x, a_val_y)
static long long sttc_extGCD(long long a_val_x, long long a_val_y, long long* a_p_coef_a, long long* a_p_coef_b)
{
	long long ret_gcd = 1;
	if (a_val_y == 0) {
		*a_p_coef_a = 1;
		*a_p_coef_b = 0;
		ret_gcd = a_val_x;
	}
	else
	{
		ret_gcd = sttc_extGCD(a_val_y, a_val_x % a_val_y, a_p_coef_b, a_p_coef_a);
		*a_p_coef_b -= a_val_x / a_val_y * (*a_p_coef_a);
	}
	return ret_gcd;
}

//
static double sttc_power_bw(DOT_COL a_bw, double a_u, double a_v)
{
	double ret_pow = 0.0;

	constexpr double BLACK_CU = 0.501, BLACK_CV = 0.502;
	double curr_u, curr_v;
	double dist_u, dist_v;
	if (a_bw == DOT_COL::e_BLACK)
	{
		curr_u = a_u;
		curr_v = a_v;
	}
	else
	{
		curr_u = (a_u + BLACK_CU) - std::floor(a_u + BLACK_CU);
		curr_v = (a_v + BLACK_CV) - std::floor(a_v + BLACK_CV);
	}
	dist_u = (BLACK_CU - curr_u);
	dist_v = (BLACK_CV - curr_v);
	double dist = std::sqrt(dist_u * dist_u + dist_v * dist_v);

	//
	constexpr double SMALL = 1.0e-10;
	if (a_bw == DOT_COL::e_BLACK)
	{
		ret_pow = 1.0 / (dist + SMALL);
	}
	else
	{
		ret_pow = -1.0 / (dist + SMALL);
	}

	return ret_pow;
}

//
struct ht_cell
{
	int id;
	double dist2;
	ht_cell(int a_id, double a_dist2) : id(a_id), dist2(a_dist2) {}

	// ŽÀ‘•‚µ‚È‚¢
	ht_cell(void) = delete;
};

//
bool operator<(const ht_cell& left, const ht_cell& right)
{
	return left.dist2 < right.dist2;
}

////////////////////////////////////////////
//
// public constructor, destructor
//
////////////////////////////////////////////

//
sc::Binarization::Holladay::Holladay(void)
{
	this->Create_(10, 10, 1);
	return;
}

//
sc::Binarization::Holladay::Holladay(int a_x, int a_y, int a_div)
{
	this->Create_(a_x, a_y, a_div);
	return;
}

//
sc::Binarization::Holladay::~Holladay(void)
{
	return;
}

//------------------------------------------
//
// private
//
//------------------------------------------
// @@@
// x >= 1, y >= 0
void sc::Binarization::Holladay::Create_(int a_x, int a_y, int a_div)
{
	this->ary_vec_[0].x = a_x;
	this->ary_vec_[0].y = a_y;
	this->ary_vec_[0].div = a_div;

	this->ary_vec_[1].x = - a_y;
	this->ary_vec_[1].y = a_x;
	this->ary_vec_[1].div = a_div;

	// @@@
	volatile bool v_flag_hoge = false;
	v_flag_hoge = true;
	if (v_flag_hoge)
	{
		this->ary_vec_[0].x = st_scale * 5;
		this->ary_vec_[0].y = st_scale * 2;
		this->ary_vec_[0].div = 1;
		this->ary_vec_[1].x = st_scale * (-3);
		this->ary_vec_[1].y = st_scale * 4;
		this->ary_vec_[1].div = 1;
	}
	//
	this->MakeTile_();

	return;
}

//
void sc::Binarization::Holladay::Destroy_(void)
{
	return;
}


// @@@
// not fixed
void sc::Binarization::Holladay::MakeTile_WHS_(void)
{
	// pixel num = outer product
	long long pix_num = this->ary_vec_[0].x * this->ary_vec_[1].y - this->ary_vec_[1].x * this->ary_vec_[0].y;
	if (pix_num < 0)
	{
		pix_num = -pix_num;
	}
	if (pix_num != 0)
	{
		// https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
		// ax+by=gcd(a,b)
		long long ary_coef[2] = { 0, };
		this->tile_h_ = sttc_extGCD(this->ary_vec_[0].y, this->ary_vec_[1].y, &(ary_coef[0]), &(ary_coef[1]));
		this->tile_w_ = pix_num / this->tile_h_;
		long long ax_by = (ary_coef[0] * this->ary_vec_[0].x + ary_coef[1] * this->ary_vec_[1].x);
		this->shift_ = this->tile_w_ - ax_by;
		this->shift_ %= this->tile_w_;
		if (this->shift_ < 0)
		{
			this->shift_ = this->shift_ + this->tile_w_;
		}
	}
	else
	{
		this->tile_h_ = 1;
		this->tile_w_ = 1;
		this->shift_ = 0;
		this->ary_vec_[0] = { 1, 0 };
		this->ary_vec_[1] = { 0, 1 };
	}
	return;
}

// @@@
// Bug is NOT fixed.
void sc::Binarization::Holladay::MakeTile_ThresholdMatrix_(void)
{
	//
	double len0 = this->ary_vec_[0].Lenght();
	double len1 = this->ary_vec_[1].Lenght();
	// delta u & delta v (when x += 1)
	double x_du = static_cast<double>(this->ary_vec_[0].div) * this->ary_vec_[0].Cos() / len0;
	double x_dv = static_cast<double>(this->ary_vec_[1].div) * this->ary_vec_[1].Cos() / len1;
	x_du = 2.0 / (13 * st_scale);
	x_dv = -1.0 / (13 * st_scale);

	// delta u & delta v (when y += 1)
	double y_du = static_cast<double>(this->ary_vec_[0].div) * this->ary_vec_[0].Sin() / len0;
	double y_dv = static_cast<double>(this->ary_vec_[1].div) * this->ary_vec_[1].Sin() / len1;
	y_du = 3.0 / (26 * st_scale);
	y_dv = 5.0 / (26 * st_scale);

	//
	std::vector<ht_cell> ary_dist;

	int y;
	int id;
	for (y = 0, id = 0	; y < this->tile_h_; y++)
	{
		double curr_u = (y * y_du);
		curr_u = curr_u - std::floor(curr_u);
		double curr_v = (y * y_dv) - std::floor(y * y_dv);
		curr_v = curr_v - std::floor(curr_v);
		int x;
		for (x = 0; x < this->tile_w_; x++, id++)
		{
			double ratio_b = 0.5;	// [0, 1]

			int new_th = 1;
			double pow_b = sttc_power_bw(DOT_COL::e_BLACK, curr_u, curr_v);
			double pow_w = sttc_power_bw(DOT_COL::e_WHITE, curr_u, curr_v);
			double pow_dot = ratio_b * pow_b + (1.0 - ratio_b) * pow_w;
			ht_cell data(id, pow_dot);
			ary_dist.push_back(data);
			this->ary_threh_.push_back(new_th);

			//
			curr_u += x_du;
			curr_v += x_dv;
			curr_u = curr_u - std::floor(curr_u);
			curr_v = curr_v - std::floor(curr_v);
		}
	}
	//
	std::sort(ary_dist.begin(), ary_dist.end());
	
	//
	this->ary_threh_.resize(this->tile_w_ * this->tile_h_);
	int th_level;
	std::vector<ht_cell>::iterator itr;
	for (itr = ary_dist.begin(), th_level = 0; itr != ary_dist.end(); itr++, th_level++)
	{
		this->ary_threh_[itr->id] = th_level;
	}

	// @@@
	//for (y = 0, id = 0; y < this->tile_h_; y++)
	//{
	//	int x;
	//	for (x = 0; x < this->tile_w_; x++, id++)
	//	{
	//		std::cout << this->ary_threh_[id] << ",";
	//	}
	//	std::cout << std::endl;
	//}

	return;
}

//
void sc::Binarization::Holladay::MakeTile_(void)
{
	this->MakeTile_WHS_();
	this->MakeTile_ThresholdMatrix_();
	return;
}

////////////////////////////////////////////
//
// public
//
////////////////////////////////////////////

//
long long sc::Binarization::Holladay::Width(void) const
{
	return this->tile_w_;
}

//
long long sc::Binarization::Holladay::Height(void) const
{
	return this->tile_h_;
}

//
long long sc::Binarization::Holladay::Shift(void) const
{
	return this->shift_;
}

//
const int* sc::Binarization::Holladay::MatrixPtrCst(void) const
{
	const int* ret_ptr = nullptr;
	if (this->ary_threh_.empty() == false)
	{
		ret_ptr = &(this->ary_threh_[0]);
	}
	return ret_ptr;
}

//
long long sc::Binarization::Holladay::TileSize(void) const
{
	return this->tile_w_ * this->tile_h_;
}

