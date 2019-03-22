#include "random.h"

namespace xo
{
	random_number_generator& global_random_number_generator()
	{
		static auto global_rng = random_number_generator();
		return global_rng;
	}
}
