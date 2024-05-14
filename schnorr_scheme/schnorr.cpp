#include "schnorr.h"
#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <vector>
#include <iostream>

#define QLength 256
#define PLenght 1024

cpp_int N;

void SCHNORR::generateKeys(Peggi& publicKeys, cpp_int& privateKey) {
	std::cout << "---------- KEY GENERATION ----------" << std::endl;
	N = cpp_int(0);

	std::cout << "Generating q..." << std::endl;
	publicKeys.q = generateQ();
	std::cout << "Q: [" << publicKeys.q << "]" << std::endl;

	std::cout << "Generating p..." << std::endl;
	publicKeys.p = generateP(publicKeys.q);
	std::cout << "P: [" << publicKeys.p << "]" << std::endl;

	std::cout << "Generating g..." << std::endl;
	publicKeys.g = generateG(publicKeys.q, publicKeys.p);
	std::cout << "G: [" << publicKeys.g << "]" << std::endl;
	if (publicKeys.g == 0) {
		std::cout << "Generating error" << std::endl;
		return;
	}

	std::cout << "Generating w..." << std::endl;
	privateKey = generateRandom(publicKeys.q);
	std::cout << "W: [" << privateKey << "]" << std::endl;

	std::cout << "Generating y..." << std::endl;
	publicKeys.y = countY(publicKeys.g, publicKeys.q, privateKey, publicKeys.p);
	std::cout << "Y: [" << publicKeys.y << "]" << std::endl;

	std::cout << "------------------------------------" << std::endl << std::endl;
}

cpp_int SCHNORR::identificatorX(Peggi& publicKeys, cpp_int& r) {
	r = generateRandom(publicKeys.g);
	return powm(publicKeys.g, r, publicKeys.p);
}

cpp_int SCHNORR::initE() {
	cpp_int maxValue = pow(cpp_int(2), 72) - 1;
	return generateRandom(maxValue);
}

cpp_int SCHNORR::initS(cpp_int& e, cpp_int& r, cpp_int& q,  cpp_int& w) {
	 return r + (w * e) % q;
}

bool SCHNORR::confirmS(cpp_int& x, cpp_int& s, cpp_int& e, Peggi& publickKeys) {
	cpp_int gs = powm(publickKeys.g, s, publickKeys.p);
	cpp_int ye = powm(publickKeys.y, e, publickKeys.p);
	cpp_int result = (gs * ye) % publickKeys.p;
	
	return result == x ? true : false;
}

cpp_int SCHNORR::generatePrime(const int length) {
	boost::random::mt19937 gen(std::random_device{}());
	boost::random::uniform_int_distribution<cpp_int> dist(1, (cpp_int(1) << length) - 1);

	cpp_int num;
	do {
		num = dist(gen);
	} while (!miller_rabin_test(num, 20));

	return num;
}

cpp_int SCHNORR::generateQ() {
	return generatePrime(QLength);
}

cpp_int SCHNORR::generateP(const cpp_int& q) {
	cpp_int p = cpp_int(1) << (PLenght - 1);
	N = p / q;

	while (true) {
		p = q * N + 1;
		if (miller_rabin_test(p, 20))
			return p;
		N++;
	}
}

cpp_int SCHNORR::generateG(const cpp_int& q, const cpp_int& p) {
	for (cpp_int temp(2); temp < p; temp++) {
		cpp_int g = powm(temp, N, p);

		if (g > 1) {
			if (powm(g, q, p) == cpp_int(1)) {
				return g;
			}
			return 0;
		}
	}
	return 0;
}

cpp_int SCHNORR::generateRandom(const cpp_int& maxValue) {
	boost::random::mt19937 gen(std::random_device{}());
	boost::random::uniform_int_distribution<cpp_int> dist(0, maxValue);
	return dist(gen);
}

cpp_int SCHNORR::countY(const cpp_int& g, const cpp_int& q, const cpp_int& w, const cpp_int& p) {
	cpp_int degree = q - w;
	return powm(g, degree, p);
}