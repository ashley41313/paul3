/*file header:
 * ashley eckert aeckert@Ucsd.edu
 * This file has the bitoutputstream class and it's method
 * this file is used to make bit by bit operations. 
 **/

/*class header
 * the bitoutputstream class creates a buffer and does bitwise 
 * operations on the buffer using 0's and 1's, and then only
 * when it has finished populating an entire buffer does it print it
 * out to an ostream. These functions are used by bitoutputstream objects
 * */
#include <ostream>
#include <iostream>
class BitOutputStream {
	private: 
		unsigned char buf; /*the buffer to edit on*/
		int nbits; /*to keep track of position in buffer*/
		std::ostream & out; /*the stream to print out to */

	public:
		/*constructor*/
		BitOutputStream(std::ostream & os) : out(os), buf(0), nbits(0) {}

		/*used to reset buffer and flush the ostream*/
		void flush() {
			out.put(buf);
			out.flush();
			nbits = buf = 0;
		}
	
		/*writes a bit to the buffer*/
		void writeBit(unsigned int bit) {
		
			if( nbits == 8 ) { flush(); } 

			unsigned char newBit = (unsigned char)(bit << (7-nbits)); 
			unsigned char buffer = getBuf();
			buffer = buffer | newBit;
			buf = buffer;
			nbits++;
			
			if( nbits == 8 ) { flush(); } 

		}

		/*returns reference to the buffer*/
		unsigned char getBuf () {
			return buf;
		}

		/*sets the buffer*/
		void setBuf (unsigned char buffer) {
			buf = buffer;
		}

	/*writes out an entire char to the ostream*/
	void writeNumBits(int totalbits) {

		/*write out each bit of the symbol*/
		for(int i = 0; i < 8; i++) {
			writeBit(totalbits>>(7-i));
		}
	}
};
