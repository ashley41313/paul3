#include <ostream>
#include <iostream>
class BitOutputStream {
	private: 
		unsigned char buf;
		int nbits;
		std::ostream & out;

	public:
		BitOutputStream(std::ostream & os) : out(os), buf(0), nbits(0) {}

		void flush() {
			out.put(buf);
			out.flush();
			nbits = buf = 0;
		}
	

		void writeBit(unsigned int bit) {
		
			if( nbits == 8 ) { flush(); } 

			unsigned char newBit = (unsigned char)(bit << (7-nbits)); 
			unsigned char buffer = getBuf();
			buffer = buffer | newBit;
			buf = buffer;
			nbits++;
			
			if( nbits == 8 ) { flush(); } 

		}

		unsigned char getBuf () {
			return buf;
		}

		void setBuf (unsigned char buffer) {
			buf = buffer;
		}


	void writeNumBits(int totalbits) {

		/*write out each bit of the symbol*/
		for(int i = 0; i < 8; i++) {
			writeBit(totalbits>>(7-i));
		}
	}
};
