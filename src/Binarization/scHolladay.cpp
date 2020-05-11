//
#include <Binarization/scHolladay.h>

// floor
#include <cmath>

//
#include <vector>
#include <algorithm>


//
enum class DOT_COL
{
	e_BLACK = 0,
	e_WHITE
};

// - - - - - - - - - - - - - - - - - - - - -
//
// not member functions (ht_cell)
//
// - - - - - - - - - - - - - - - - - - - - -

//
struct ht_cell
{
	int id;
	double dist2;
	ht_cell(int a_id, double a_dist2) : id(a_id), dist2(a_dist2) {}

	// not implemented
	ht_cell(void) = delete;
};

//
bool operator<(const ht_cell& left, const ht_cell& right)
{
	return left.dist2 < right.dist2;
}

// - - - - - - - - - - - - - - - - - - - - -
//
// not member functions
//
// - - - - - - - - - - - - - - - - - - - - -

//
double sttc_SimultaneousEquations(long long a_mtx_2x2[4], long long a_x, long long a_y, double *a_p_u, double *a_p_v)
{
	double ret_det = static_cast<double>(a_mtx_2x2[0] * a_mtx_2x2[3] - a_mtx_2x2[1] * a_mtx_2x2[2]);
	if (ret_det != 0)
	{
		double inv = 1.0 / ret_det;
		*a_p_u = (a_mtx_2x2[3] * a_x - a_mtx_2x2[1] * a_y) * inv;
		*a_p_v = (- a_mtx_2x2[2] * a_x + a_mtx_2x2[0] * a_y) * inv;
	}
	return ret_det;
}

// this funciton retunrs gcd(a_val_x, a_val_y).
// (*a_p_coef_a) * a_val_x + (*a_r_coef_b) * a_val_y = gcd(a_val_x, a_val_y)
static long long sttc_extGCD(long long a_val_x, long long a_val_y, long long* a_p_coef_a, long long* a_p_coef_b)
{
	long long ret_gcd = 1;
	if (a_val_y == 0)
	{
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
static double sttc_CalcDotPower(double a_u, double a_v)
{
	double ret_dot_power = 0.0;

	double ratio_b = 0.5;	// [0.0, 1.0]
	double pow_b = sttc_power_bw(DOT_COL::e_BLACK, a_u, a_v);
	double pow_w = sttc_power_bw(DOT_COL::e_WHITE, a_u, a_v);

	ret_dot_power = ratio_b * pow_b + (1.0 - ratio_b) * pow_w;

	return ret_dot_power;
}


////////////////////////////////////////////
//
// public constructor, destructor
//
////////////////////////////////////////////

//
sc::Binarization::Holladay::Holladay(void)
{
	this->Create_(10, 10, 1, -10, 10, 1);
	return;
}

//
sc::Binarization::Holladay::Holladay(int a_x, int a_y, int a_div)
{
	this->Create_(a_x, a_y, a_div, -a_y, a_x, a_div);
	return;
}
//
sc::Binarization::Holladay::Holladay(int a_x0, int a_y0, int a_div0, int a_x1, int a_y1, int a_div1)
{
	this->Create_(a_x0, a_y0, a_div0, a_x1, a_y1, a_div1);
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

// x >= 1, y >= 0
void sc::Binarization::Holladay::Create_(int a_x0, int a_y0, int a_div0, int a_x1, int a_y1, int a_div1)
{
	this->ary_vec_[0].x = a_x0;
	this->ary_vec_[0].y = a_y0;
	this->ary_vec_[0].div = a_div0;

	this->ary_vec_[1].x = a_x1;
	this->ary_vec_[1].y = a_y1;
	this->ary_vec_[1].div = a_div1;

	// outer product == 0
	if (this->ary_vec_[0].OuterProduct(this->ary_vec_[1]) == 0)
	{
		this->ary_vec_[0].x = 1;
		this->ary_vec_[0].y = 0;
		this->ary_vec_[1].x = 0;
		this->ary_vec_[1].y = 1;
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

// Width, Height, Shift
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

	//
	long long tmp_h_s0 = this->tile_h_;
	if (this->shift_ != 0)
	{
		long long tmp_s = this->shift_;
		do
		{
			tmp_h_s0 += this->tile_h_;
			tmp_s += this->shift_;
			tmp_s %= this->tile_w_;
		} while (tmp_s != 0);
	}
	this->tile_h_s0_ = tmp_h_s0;

	return;
}

//
void sc::Binarization::Holladay::MakeTile_ThresholdMatrix_(void)
{
	//
	double ary_coef[2] = { 0.0, };
	long long mtx2x2_v0v1[4] = { this->ary_vec_[0].x, this->ary_vec_[1].x, this->ary_vec_[0].y, this->ary_vec_[1].y };

	// delta u & delta v (when x += 1)
	sttc_SimultaneousEquations(mtx2x2_v0v1, this->tile_w_, 0, &(ary_coef[0]), &(ary_coef[1]));
	double x_du = (this->ary_vec_[0].div * ary_coef[0]) / (this->tile_w_);
	double x_dv = (this->ary_vec_[1].div * ary_coef[1]) / (this->tile_w_);

	// delta u & delta v (when y += 1)
	sttc_SimultaneousEquations(mtx2x2_v0v1, 0, this->tile_h_s0_, &(ary_coef[0]), &(ary_coef[1]));
	double y_du = (this->ary_vec_[0].div * ary_coef[0]) / (this->tile_h_s0_);
	double y_dv = (this->ary_vec_[1].div * ary_coef[1]) / (this->tile_h_s0_);

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
			double dot_power = sttc_CalcDotPower(curr_u, curr_v);
			ht_cell data(id, dot_power);
			ary_dist.push_back(data);

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

