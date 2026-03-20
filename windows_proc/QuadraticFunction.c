#include "QuadraticFunction.h"

QuadraticFunction_t fscanf_qf(FILE* file)
{
	QuadraticFunction_t quadratic_function;
	fscanf_s(file, "%f %f %f", &quadratic_function.a, &quadratic_function.b, &quadratic_function.c);
	return quadratic_function;
}

void fprintf_qf(QuadraticFunction_t quadratic_function, FILE* file)
{
	fprintf_s(file, "%f %f %f\n", quadratic_function.a, quadratic_function.b, quadratic_function.c);
}

void fprintf_zp_qf(ZeroPlacesQF_t zp_qf, FILE* file)
{
	fprintf_s(file, "Re: %f Im: %f, Re: %f Im: %f\n", zp_qf.x1.re, zp_qf.x1.im, zp_qf.x2.re, zp_qf.x2.im);
}

float delta_qf(QuadraticFunction_t quadratic_function)
{
	return quadratic_function.b * quadratic_function.b - 4 * quadratic_function.a * quadratic_function.c;
}

ZeroPlacesQF_t find_zp_qt(QuadraticFunction_t quadratic_function)
{
	float delta = delta_qf(quadratic_function);

	ZeroPlacesQF_t zero_places;

	if (0.0f == delta)
	{
		zero_places.x1.re = -quadratic_function.b / (2 * quadratic_function.a);
		zero_places.x1.im = 0.0f;
		zero_places.x2 = zero_places.x1;
		return zero_places;
	}

	if (0.0f < delta)
	{
		float delta_sqrt = sqrtf(delta);
		zero_places.x1.re = (-quadratic_function.b - delta_sqrt) / (2 * quadratic_function.a);
		zero_places.x1.im = 0.0f;
		zero_places.x2.re = (-quadratic_function.b + delta_sqrt) / (2 * quadratic_function.a);
		zero_places.x2.im = 0.0f;
		return zero_places;
	}

	float delta_sqrt = sqrtf(-delta);
	zero_places.x1.re = (-quadratic_function.b) / (2 * quadratic_function.a);
	zero_places.x1.im = -delta_sqrt / (2 * quadratic_function.a);
	zero_places.x2.re = zero_places.x1.re;
	zero_places.x2.im = -zero_places.x1.im;
	return zero_places;
}