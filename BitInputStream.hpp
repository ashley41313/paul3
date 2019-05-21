/*file header:
 * ashley eckert aeckert@Ucsd.edu
 * This file has the bitinputstream class and it's method
 * this file is used to make bit by bit operations. 
 **/

/*class header
 * the bitinputstream class creates a buffer and does bitwise 
 * operations on the buffer using 0's and 1's, and then only
 * when it has finished populating an entire buffer does it print it
 * out to an ostream. These functions are used by bitoutputstream objects
 * */


#include <istream>

class BitInputStream {
private: 
	unsigned char buf; /*buffer to do bit ops on*/
	int nbits; /*to keep track of position in buffer*/
	std::istream & in; /*the stream to read from*/

public:
	/*the constructor for this bitinputstream obj*/
	BitInputStream(std::istream & is) : in(is), buf(0), nbits(8) {} 

	/*used to get bytes at a time from the input file and put it into
 	* the buffer*/
	void fill() {
		buf = in.get();
		nbits = 0;
	}

	/*extracts one bit at a time at nbits pisition from the buffer and
 	*returns it*/
	unsigned int readBit () {
		if (nbits == 8) { fill(); }

		unsigned int nextBit = (((buf >> (7-nbits)) & 1) << 7-nbits);
		
		nbits++;
		return nextBit;
	}

	/*used in uncompress to put the bit back into its original plaee*/
	unsigned int readBitUncompress (int index) {
		if (nbits == 8) { fill(); }

		unsigned int nextBit = (((buf >> (7-index)) & 1) << 7-index);
		
		nbits++;
		return nextBit;
	}
	
	/*read bits as 0'sand 1's only, not placing them back in their 
 	*original position*/
	unsigned int readBitOnesZeros () {
		if (nbits == 8) { fill(); }

		unsigned int nextBit = ((buf >> (7-nbits)) & 1);
		
		nbits++;
		return nextBit;
	}
	
};
