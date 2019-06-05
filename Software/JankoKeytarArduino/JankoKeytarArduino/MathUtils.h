#pragma once
#include "math.h"

long mapClamped(long value, long a, long b, long c, long d)
{
	if (value < a) return c;
	if (value > b) return d;

	return map(value, a, b, c, d);
}