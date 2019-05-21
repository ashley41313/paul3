#include "HCTree.hpp"
#include "HCNode.hpp"
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <iostream>
#include <fstream>

using namespace std;

/*this method is called from the main method. It has two file names 
 * passed into it which will be used to encode a string. inFile will 
 * have a message that we will encode into chars of 0's and 1's and
 * store into outFile. Basically, this method reads from inFile and counts
 * the occurences of each letter. It than calls BUILD (and HCTREE) method
 * to create a tree. It then uses that tree to encode the chars. It then
 * puts those encoded chars into the outfile.*/
void compress(std::string inFile, std::string outFile) {
	int bytelength = 8; 

	/*create vector to store counts in*/
    	vector<int> freqs = vector<int>(256, 0);

	std::ifstream in(inFile); /*create the stream to read file from*/
	/*open the input file, and read in all the chars*/	
	int empty = 1;
	
	char c; 
	while(in.get(c)) { 
		freqs[(int)c] += 1; /*increment number at that index*/		
		empty = 0;
	}

	/*print out for empty file*/
	if(empty) {
		cout << "Invalid input file. Please try again." << endl;
	}

	in.close();/*CLOSE THE FILE*/

/*BUILD TREE*/
	HCTree* hc = new HCTree();
	hc->HCTree::build(freqs);

	ofstream out(outFile, ios_base::app | ios_base::out);

	if(!hc->HCTree::getRoot()) { return; }

	BitOutputStream * outStream = new BitOutputStream(out);

	/*count toal bits for the encode part*/ 
	int totalbits = 0;
	for(int i = 0 ; i < 256; i++) {
		if(freqs[i] != 0) {
			totalbits += hc->HCTree::countBits(i);	
		}
	}
	
	/*get root of tree, and count toal # of bits in header*/
	HCNode * start = hc->HCTree::getRoot();
	hc->HCTree::countHeader(start);
	int hlength = hc->HCTree::getHeaderLength(); 

	/*get the padding of the header bits*/
	int headerpadding = bytelength - (hlength % bytelength);
	int headerbytes =  (hlength + headerpadding) / bytelength;

	int encodepadding = bytelength - (totalbits %  bytelength); 
	int encodebytes = (totalbits + encodepadding) / bytelength;
/*DEBUGU*/
	cout << "headersize: " << headerbytes << endl;
	cout << "headerpadding: " << headerpadding << endl;
	cout << "encodesize: " << encodebytes << endl;
	cout << "encodepadding: " << encodepadding << endl;

	/*write out the size of each part of the stream*/
	outStream->writeNumBits(headerbytes);
	outStream->writeNumBits(headerpadding);
	outStream->writeNumBits(encodebytes);
	outStream->writeNumBits(encodepadding);

	hc->HCTree::writeHeader(start, *outStream); 
	outStream->flush();
	
	in.open(inFile);
	
/*CALL ENCODE*/
	while(in.get(c)) {
		hc->HCTree::encode(c, *outStream);
	}	
	
	outStream->flush();

	/*close the files you're done*/
	in.close();
	delete hc;
}


int main(int argc, char** argv) {

	if(argc != 3) {
		/*WRONG AMOUNT OF INPUTS*/
		return 0;
	}
	compress(argv[1], argv[2]);
	return 1;
}
