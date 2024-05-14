#include "schnorr.h"
#include <iostream>

void model() {
	int rounds = 10;
	bool log = true;

	Peggi publicKeys;
	cpp_int privateKey; // Only Alice

	cpp_int r;
	cpp_int x;
	cpp_int e;
	cpp_int s;
	bool confirm = false;

	SCHNORR::generateKeys(publicKeys, privateKey);
	x = SCHNORR::identificatorX(publicKeys, r); // Alice to Bob
	if (log) {
		std::cout << "Identificator X: [" << x << "]" << std::endl << std::endl;
		std::cout << "Component R: [" << r << "]" << std::endl << std::endl;
	}

	for (int i = 0; i < rounds; ++i) {
		e = SCHNORR::initE(); //Bob to Alice

		s = SCHNORR::initS(e, r, publicKeys.q, privateKey); // Alice to Bob

		confirm = SCHNORR::confirmS(x, s, e, publicKeys); // Verification (on Bob site)

		if (log) {
			std::cout << "Round " << i + 1 << " [" << std::endl;
			std::cout << "Bob's E: " << e << std::endl << std::endl;
			std::cout << "Alice's S: " << s << std::endl << std::endl;
			std::cout << "Result of verification: " << std::boolalpha << confirm << std::endl;
			std::cout << "]" << std::endl << std::endl;
		}
		else {
			std::cout << "Result of verification [" << i + 1 << "]: " << std::boolalpha << confirm << std::endl;
		}
	}
}

int main() {
	model();
	return 0;
}