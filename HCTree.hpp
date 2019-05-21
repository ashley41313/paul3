#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <queue>
#include <vector>
#include <fstream>
#include "HCNode.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

using namespace std;

/** A 'function class' for use as the Compare class in a
 *  priority_queue<HCNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */
class HCNodePtrComp {
public:
    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        return *lhs < *rhs;
    }
};

/** A Huffman Code Tree class.
 *  Not very generic:  Use only if alphabet consists
 *  of unsigned chars.
 */
class HCTree {
private:
    HCNode* root;
    vector<HCNode*> leaves;
    int headerlength;
    int headerBitsRead;
public:
    explicit HCTree() : root(0) {
        leaves = vector<HCNode*>(256, (HCNode*) 0);
	headerlength = 0;
	headerBitsRead = 0;
    }

    ~HCTree();

    void rebuild(HCNode* curr, BitInputStream& infile, int headersize, int right);
    void deleteTree(HCNode *);
    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
    void build(const vector<int>& freqs);

    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    void encode(byte symbol, BitOutputStream& out);

    /*this write out entire 8 bit buffers to the output stream,
    *so we don't need to call writebit individually on
    *each bit
    * */
    void writeByte(int totalbits, BitOutputStream& out);

    /* this counts how many bits will be in the encoded part
    * of the file.
    * */
    int countBits(int symbol);


    /* this counts how many bits we will need to read in as part  
    * of the header
    * */
    void countHeader(HCNode* curr);

	
    /* this function actually writes out all the bits of the header out 
    * and stops aftr headersize bits.
    * */
    void writeHeader(HCNode* curr, BitOutputStream & out); 

    /*returns a referense to the root node of the tree*/
    HCNode* getRoot() {
	return root;
    }

    /*traverses the header and sees how long it will need to be*/	
    int getHeaderLength() {
	return headerlength;
    }

    /*returns a reference to the leaves vector*/
    vector<HCNode*> getLeaves() {
	return leaves;
    }
};

#endif // HCTREE_H
