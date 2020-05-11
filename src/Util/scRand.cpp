//
#include <Util/scRand.h>

//
namespace
{
	const unsigned int ARY_RAND_INI_[4] = { 0xD9363228, 0x021A79C9, 0x7604820B, 0xD18E1939 };
}

//
sc::Util::Rand::Rand(void)
{
	this->Create_();
	return;
}

//
sc::Util::Rand::Rand(unsigned int a_seed)
{
	this->seed_ = a_seed;
	this->Create_();
	return;
}

//
sc::Util::Rand::~Rand(void)
{
	this->Destroy_();
	return;
}

//
void sc::Util::Rand::Create_(void)
{
	//
	this->seed_ = 0;
	this->Init_();
	return;
}

//
void sc::Util::Rand::Destroy_(void)
{
	return;
}

//
void sc::Util::Rand::Init_(void)
{
	size_t i;
	for (i = 0; i < 4; i++)
	{
		this->ary_rand_tmp_[i] = ARY_RAND_INI_[i] ^ this->seed_;
	}
	return;
}

//
unsigned int sc::Util::Rand::GenerateRand_(void)
{
	unsigned int ret_rand = 0;
	unsigned int tmp_i;

	tmp_i = this->ary_rand_tmp_[0] ^ (this->ary_rand_tmp_[0] << 11);
	this->ary_rand_tmp_[0] = this->ary_rand_tmp_[1];
	this->ary_rand_tmp_[1] = this->ary_rand_tmp_[2]; this->ary_rand_tmp_[2] = this->ary_rand_tmp_[3];
	this->ary_rand_tmp_[3] = (this->ary_rand_tmp_[3] ^ (this->ary_rand_tmp_[3] >> 19)) ^ (tmp_i ^ (tmp_i >> 8));
	ret_rand = this->ary_rand_tmp_[3];

	return ret_rand;
}


//
void sc::Util::Rand::Seed(unsigned int a_seed)
{
	this->seed_ = a_seed;
	this->Init_();
	return;
}

//
unsigned char sc::Util::Rand::GetU08(void)
{
	unsigned char ret_rand = 0;
	unsigned int ret_rand_tmp = this->GenerateRand_();
	ret_rand = ret_rand_tmp & 0xff;
	return ret_rand;
}

//
unsigned int sc::Util::Rand::GetU32(void)
{
	unsigned int ret_rand = this->GenerateRand_();
	return ret_rand;
}
