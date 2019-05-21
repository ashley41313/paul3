/*FILE HEADER: 
 * ashley eckert aeckert@ucsd.edu
 * this file has the uncompress method which is used to decode 
 * a file1 into an original text file2. It does this by calling 
 *  rebuild and building a huffman tree from encoded bits in file 1*/

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
void uncompress(std::string inFile, std::string outFile) {
	int DEC = 10; /*to use in strtol*/
	int bytelength = 8;

	std::ifstream in(inFile); /*open the file and attatch to a stream*/
	BitInputStream * infile = new BitInputStream(in);

	/*GET THE SIZE OF EAHC PART OF THE STREAM*/
	/*READ THE FIRST 8 BITS TO FIND OUT HOW MANY BYTES WILL BE IN THE H*/
	unsigned int headersize = 0;
	for(int i = 0; i < bytelength; i++) {
		headersize = headersize ^ infile->readBit();
	} 
	/*READ THE FIRST 8 BITS TO FIND OUT HOW MANY BITS WILL BE IN THE HPAD*/
	unsigned int headerpadding = 0;
	for(int i = 0; i < bytelength; i++) {
		headerpadding = headerpadding ^ infile->readBit();
	} 
	
	/*READ THE FIRST 8 BITS TO FIND OUT HOW MANY BYTES WILL BE IN ENCODE*/
	unsigned int encodesize = 0;
	for(int i = 0; i < bytelength; i++) {
		encodesize = encodesize ^ infile->readBit();
	} 
	/*READ THE FIRST 8 BITS TO FIND OUT HOW MANY BITS WILL BE INENCODE*/
	unsigned int encodepadding = 0;
	for(int i = 0; i < bytelength; i++) {
		encodepadding = encodepadding ^ infile->readBit();
	} 

	/*numfer of bits to read for the header*/
	int headerbits = (headersize * bytelength) - headerpadding;
	int encodebits = (encodesize * bytelength) - encodepadding;
	/*build the tree using the vector*/
	HCTree* hc = new HCTree();
	HCNode * newRoot = hc->HCTree::getRoot();
	hc->HCTree::rebuild(newRoot, *infile, headerbits, 0);

	/*loop through the padding for the header bits*/
	for(int i = 0; i < headerpadding; i++) {
		int bitt = infile->readBit();
	}

	ofstream out; /*create stream to writeo ut to*/
	/*open output file and write out the vector to rebuild later*/
	out.open(outFile);
	HCNode* start;

	/*write out the decoded string*/
	for(int i = 0; i < encodebits;) {
		start = hc->HCTree::getRoot();
		while(start->c0 && start->c1) {
			int bit = infile->readBitOnesZeros();
			i++;
			if(bit == 0) {
				start = start->c0;
			}
			else {
				start = start->c1;
			}
		}
		out << start->symbol;
	}

	/*close files*/
	in.close();
	out.close();	
	delete hc;				
}

int main(int argc, char** argv) {

	if(argc != 3) {
		/*WRONG AMOUNT OF INPUTS*/
		return 0;
	}
	uncompress(argv[1], argv[2]);
	return 1;
}
