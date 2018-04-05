#pragma once
#include <algorithm>

template <typename T>
void sortAscending(T& t0, T& t1)
{
	if (t0 > t1) { std::swap(t0, t1); }
}