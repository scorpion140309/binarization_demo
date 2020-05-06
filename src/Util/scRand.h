//
#ifndef SC_RAND_H_
#define SC_RAND_H_


namespace sc
{
	namespace Util
	{
		class Rand
		{
		private:
			unsigned int seed_;
			unsigned int ary_rand_tmp_[4];
			void Create_(void);
			void Destroy_(void);
			void Init_(void);
			unsigned int GenerateRand_(void);
		public:
			Rand(void);
			Rand(unsigned int a_seed);
			~Rand(void);

			void Seed(unsigned int a_seed);
			unsigned int GetU32(void);
			unsigned char GetU08(void);
		};
	}
}

#endif  //SC_RAND_H_
