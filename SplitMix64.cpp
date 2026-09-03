#include <iostream>
#include <cmath>
#include <vector>

#include "SplitMix64.h"

Splitmix64::Splitmix64() { state = 0; };
Splitmix64::Splitmix64(const uint64_t seed) : state(seed) {};

void Splitmix64::seed(const uint64_t seed) {
	state = seed;
	}

uint64_t Splitmix64::next_int() {
	uint64_t z = (state += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

double Splitmix64::next_float() {
	return next_int() / twoPower64;
}

