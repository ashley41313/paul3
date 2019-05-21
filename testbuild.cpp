


// Created by Ashley Eckert on April 29th, 2019 
// This is a file to test insert, and the predict completions of TST

#include "HCTree.hpp"
#include <fstream>
#include <sstream>
using namespace std;

void testbuild () {

	/*create tree*/
	HCTree * hc = new HCTree();

	/*create vector*/
	vector<int> freqs = vector<int>(256, 0);
	/*will only have a, b, c, d*/
	freqs[65] = 2;
	freqs[66] = 2;
	freqs[67] = 5;
	freqs[68] = 7;
	
	hc->build(freqs);

	HCNode * newRoot = hc->getRoot();
	/*traverse the root and see if it's as expected*/
	cout << "This should be D and 16: " <<  newRoot->symbol << " " << newRoot->count << endl;
	cout << "this should be D and 7: " << newRoot->c0->symbol << " " << newRoot->c0->count << endl;	
	cout << "this should be B and 9: " << newRoot->c1->symbol << " " << newRoot->c1->count << endl;	
	cout << "this should be C and 5: " << newRoot->c1->c1->symbol << " " << newRoot->c1->c1->count << endl;	
	/*traverse pattern for a, print out the 1's and 0's and then a*/

}

int main() {

	cout << "TESTING MY BUILD FUNCTION" << endl;
	testbuild();
	cout << "DONE TESTS." << endl;
}
