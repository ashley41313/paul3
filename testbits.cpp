#include "HCTree.hpp"
#include <fstream>
#include <sstream>
#include <ostream>

using namespace std;
void testbits () {

	/*create the ostream to pass into BitInputStream*/
	ofstream outfile;
	outfile.open("testBits.txt");
	BitOutputStream * bits = new BitOutputStream(outfile);

	/*should write out to the file as 10010000*/
	bits->writeBit(0);
	bits->writeBit(1);
	bits->writeBit(0);
	bits->writeBit(1);
	bits->writeBit(0);
	bits->writeBit(0);
	bits->writeBit(1);
	bits->writeBit(0);
	
	outfile.close();
}

int main () {
	cout << "testing bit output stream." << endl;
	testbits();
	cout << "done testing." << endl;
}
