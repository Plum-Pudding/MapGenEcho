#pragma once

class Splitmix64 {
private:
	uint64_t state;
	const double twoPower64 = pow(2.0, 64);
public:
	Splitmix64();
	Splitmix64(const uint64_t seed);

	void seed(const uint64_t seed);
	uint64_t next_int();
	double next_double();
	double next_double_factor(); //returns a double betwen 0 and 1
};