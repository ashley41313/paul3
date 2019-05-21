#include <istream>

class BitInputStream {
private: 
	unsigned char buf;
	int nbits;
	std::istream & in;

public:
	BitInputStream(std::istream & is) : in(is), buf(0), nbits(8) {} 


	void fill() {
		buf = in.get();
		nbits = 0;
	}

	unsigned int readBit () {
		if (nbits == 8) { fill(); }

		unsigned int nextBit = (((buf >> (7-nbits)) & 1) << 7-nbits);
		
		nbits++;
		return nextBit;
	}

	unsigned int readBitUncompress (int index) {
		if (nbits == 8) { fill(); }

		unsigned int nextBit = (((buf >> (7-index)) & 1) << 7-index);
		
		nbits++;
		return nextBit;
	}
	
	unsigned int readBitOnesZeros () {
		if (nbits == 8) { fill(); }

		unsigned int nextBit = ((buf >> (7-nbits)) & 1);
		
		nbits++;
		return nextBit;
	}
	
};
