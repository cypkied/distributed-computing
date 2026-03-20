#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ImaginaryNumber.h"

typedef struct
{
	float a;
	float b;
	float c;
} QuadraticFunction_t;

typedef struct
{
	ImaginaryNumber_t x1;
	ImaginaryNumber_t x2;
} ZeroPlacesQF_t;

QuadraticFunction_t fscanf_qf(FILE* file);
void fprintf_qf(QuadraticFunction_t quadratic_function, FILE* file);

void fprintf_zp_qf(ZeroPlacesQF_t zp_qf, FILE* file);

float delta_qf(QuadraticFunction_t quadratic_function);

ZeroPlacesQF_t find_zp_qt(QuadraticFunction_t quadratic_function);