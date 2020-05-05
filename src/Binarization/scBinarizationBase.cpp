//
#include <Binarization/scBinarizationBase.h>

// constructor
sc::Binarization::Base::Base(void)
{
	this->Create_(0, 0);
}

// constructor
sc::Binarization::Base::Base(size_t a_width, size_t a_height)
{
	this->Create_(a_width, a_height);
	return;
}

// destructor
sc::Binarization::Base::~Base(void)
{
	this->Destroy_();
	return;
}

//
void sc::Binarization::Base::Create_(size_t a_width, size_t a_height)
{
	this->width_ = a_width;
	this->height_ = a_height;
	this->pix_min_ = 0;
	this->pix_max_ = 255;
	return;
}

//
void sc::Binarization::Base::Destroy_(void)
{
	return;
}

//
void sc::Binarization::Base::Reset(size_t a_width, size_t a_height)
{
	this->Create_(a_width, a_height);
	return;
}

//
size_t sc::Binarization::Base::Width(void) const
{
	return this->width_;
}
//
size_t sc::Binarization::Base::Height(void) const
{
	return this->height_;
}

//
unsigned char sc::Binarization::Base::PixMin(void) const
{
	return this->pix_min_;
}

//
unsigned char sc::Binarization::Base::PixMax(void) const
{
	return this->pix_max_;
}

