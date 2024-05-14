#ifndef _SCHNORR_
#define _SCHNORR_

#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;

struct Peggi { cpp_int p, q, g, y; };

class SCHNORR {
public:
	static void generateKeys(Peggi& publicKeys, cpp_int& privateKey);

	static cpp_int identificatorX(Peggi& publicKeys, cpp_int& r);

	static cpp_int initE();

	static cpp_int initS(cpp_int& e, cpp_int& r, cpp_int& q, cpp_int& w);

	static bool confirmS(cpp_int& x, cpp_int& s, cpp_int& e, Peggi& publickKeys);

private:
	static cpp_int generatePrime(const int length);

	static cpp_int generateQ();

	static cpp_int generateP(const cpp_int& q);

	static cpp_int generateG(const cpp_int& q, const cpp_int& p);

	static cpp_int generateRandom(const cpp_int& maxValue);

	static cpp_int countY(const cpp_int& g, const cpp_int& q, const cpp_int& w, const cpp_int& p);
};

#endif