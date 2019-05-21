
/*FILE HEADER
 * Ashley Eckert, aeckert@ucsd.edu
 * this file includes the DictionaryTrie class which I chose 
 * to be a ternary search trie. it stores words and how many
 * times those words are likely to appear. The user then can use
 * this to determine if a word is likely to be next*/

/*CLASS HEADER
 *The Dictionary Trie Class is used to store TrieNodes that create
 * words.
 */


#include "HCTree.hpp"
#include "HCNode.hpp"
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <iostream>
#include <fstream>

/** Use the Huffman algorithm to build a Huffman coding trie.
*  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
*  the frequency of occurrence of byte i in the message.
*  POSTCONDITION:  root points to the root of the trie,
*  and leaves[i] points to the leaf node containing byte i.
*/
void HCTree::build(const vector<int>& freqs) {
	
	/*create a pq*/
	std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp> pq;

	/*traverse the vector and if nonzero freq, add to PQ*/
	for(int i = 0; i < (int)freqs.size(); i++) {
		
		if(freqs[i] != 0) {
			HCNode* newNode = new HCNode(freqs[i], i, 0, 0, 0);
			newNode->count = freqs[i];
			pq.push(newNode);
		}
	}

	if(!pq.size()) { /*if empty*/
		return;
	}	

	/*to pop the top 2 from the pq*/
	HCNode* firstNode;
	HCNode* secNode;
	HCNode* pNode;

	/*NOW that pq is organized, create the actual tree*/
	while(pq.size() > 1) { 
		
		/*grab first node from the pq*/
		firstNode = pq.top();
		
		/*its a leaf node, so leaves[index] points to it*/
		if(!firstNode->c0 && !firstNode->c1) {
			this->leaves[(int)firstNode->symbol] = firstNode;
		}
		
		/*create Parent Node to merge the other two*/
		pNode = new HCNode(0, firstNode->symbol, 0, 0, 0);
		pNode->c0 = firstNode;
		firstNode->p = pNode;

		pq.pop(); /*POP to get the next one*/	
		secNode = pq.top(); /*get the next node*/
		
		/*its a leaf node, so leaves[index] points to it*/
		if(!secNode->c0 && !secNode->c1) {
			this->leaves[(int)secNode->symbol] = secNode;
		}
		
		/*set the right node*/
		pNode->c1 = secNode;
		secNode->p = pNode;

		/*the PNODE will hold the ascii value of the greatest character*/
		pNode->symbol = secNode->symbol;

		pq.pop();

		/*set the count of the node, and push onto queue*/
		pNode->count = pNode->c0->count + pNode->c1->count;
		pq.push(pNode);
	}

	/*when there is one node left in PQ, it's the root node of HC tree*/
	root = pq.top();
}


/*read in pairs of ( ____, ____ ) and creates nodes for them, the left part of
 * the pair is the parent and the right part is the child. EVERY time there is a 
 * parent node, it will have a set of pairs, and if it has no child for the left
 * the right side will be a copy of the same index, otherwise it will have the
 * index of its child. 
 * if it's a middle node in the tree, out array will point to it so we can
 * easily access that node again if later, because of rrecursion, we need to
 * add another child. We will know if there is a leaf node if the second 
 * part of the set is encase inside [], then we will read in the information 
 * inside as ascii symbols.  
*/
void HCTree::rebuild(HCNode* curr, BitInputStream& infile, int headersize, int right) {
	int bytelength = 8;
	
	int bit = infile.readBitOnesZeros(); /*read in the bit*/
cout << "DEBUGGING BITS: " << bit << endl;
	
	headerBitsRead++; /*keep track of how much of header you've read*/

	if(bit == 1) { /*BASE CASE, a leaf node*/
		
		if(!root) {
		this->root = new HCNode(0,0,0,0,0);
		curr = root;
		}	

		else { 
			if(right) { 
				curr->c1 = new HCNode(0, 0, 0, 0, 0); 
				curr = curr->c1;
			}
			else { 
				curr->c0 = new HCNode(0, 0, 0, 0, 0); 
				curr = curr->c0;
			}
		}

		/*read in next 8 bits*/
		unsigned int symbolBuf = 0; /*to store the symbol*/
		for(int i = 0; i < bytelength; i++) {
			int bittt = infile.readBitOnesZeros();
			symbolBuf = symbolBuf ^ (bittt << (7-i));
cout << "DEBUGGING BITS: " << bittt << endl;
			headerBitsRead++;
		}

		curr->symbol = symbolBuf;
		this->leaves[symbolBuf] = curr; /*set the leaves vector to pointhr*/
		return;
	}	

	else if(bit == 0) {
		if(!root) {
		this->root = new HCNode(0,0,0,0,0);
		curr = root;
		}	
	
		else {	
			if(right) { 
				curr->c1 = new HCNode(0, 0, 0, 0, 0); 
				curr = curr->c1;
			}
			else { 
				curr->c0 = new HCNode(0, 0, 0, 0, 0); 
				curr = curr->c0;
			}
		}
	
		rebuild(curr, infile, headersize, 0);
		
		/*if still bits to read, call recursively on right side*/
		if(headerBitsRead < headersize - 1) {
			rebuild(curr, infile, headersize, 1);
		}
			
	}
}


int HCTree::countBits(int symbol) {
	/*create nodes to use when encoding*/
	HCNode* parentNode;
	HCNode* childNode;
	vector<int> code;
	vector<HCNode*> myLeaves = this->getLeaves();
	int bitCount = 0;

	childNode = myLeaves[symbol];
	int frequency = childNode->count;
	/*traverse up and put the code in REVERSE into vector*/
	while(childNode->p) { /*until reaching the root node*/

		parentNode = childNode->p;	

		/*decide which child, which number is the code*/
		if(childNode == parentNode->c0) { code.push_back(0); }
		else { code.push_back(1); }				
		
		childNode = parentNode; /*move up 1*/	
	}

	/*NOW pop from the vector into the output file in correct*/
	for(int j = 0; j < code.size();) {
		/*instead of just putting into the file, call this 
		* function to encode the BIT*/
		bitCount++; /*increase number of bits instead of writing out*/
		code.pop_back();
	}/*end of encoding for 1 char*/

	return bitCount * frequency;
}

void HCTree::writeByte(int totalbits, BitOutputStream& out) {

	/*write out each bit of the symbol*/
	for(int i = 0; i < 8; i++) {
		out.writeBit(totalbits>>(7-i));
	}
}


void HCTree::encode(byte symbol, BitOutputStream& out) {
	/*create nodes to use when encoding*/
	HCNode* parentNode;
	HCNode* childNode;
	vector<int> code;
	vector<HCNode*> myLeaves = this->getLeaves();

	childNode = myLeaves[(int)symbol];

	/*traverse up and put the code in REVERSE into vector*/
	while(childNode->p) { /*until reaching the root node*/

		parentNode = childNode->p;	

		/*decide which child, which number is the code*/
		if(childNode == parentNode->c0) { code.push_back(0); }
		else { code.push_back(1); }				
		
		childNode = parentNode; /*move up 1*/	
	}

	/*NOW pop from the vector into the output file in correct*/
	for(int j = 0; j < code.size();) {
		/*instead of just putting into the file, call this 
		* function to encode the BIT*/
		out.writeBit(code.back());
cout << "debigging bits: " << code.back();	
		code.pop_back();
	}/*end of encoding for 1 char*/

} /*END OF GETTING CHARS AND ENCODING THEM*/


/*just like writeheader, but isntead of writing each bit, coutns how many times
 * it should write each bit. so we can store the length of the header,
 * headerlength will be a globalvar*/
void HCTree::countHeader(HCNode* curr) {
	/*base case*/
	if(!curr->c0 && !curr->c1) {
		headerlength++;
		/*write out each bit of the symbol*/
		for(int i = 0; i < 8; i++) {
			headerlength++;
		}
		return;
	}

	headerlength++;
	if(curr->c0) { 
		countHeader(curr->c0);
	}
	if(curr->c1) { 
		countHeader(curr->c1);
	}
}

/*curr will initially point to the root node of the tree, but will traverse
 * down recursively in this same function writing down the pairs of edges to 
 * rebuild the tree later on in our ouput file. 
 * add another right child to that node. ALSO it will detect a leaf node by
 * if there is a symbol encased by parenthesis (), then it will return from the 
 * function.
 *
 * will let know how any bits to read at the END OF THIS FILE by:
 *
 * will return an INT which will be the header length
 * */ 
void HCTree::writeHeader(HCNode* curr, BitOutputStream & out) {

	/*base case*/
	if(!curr->c0 && !curr->c1) {
		out.writeBit(1); /*write 1 bit*/
		/*write out each bit of the symbol*/
		for(int i = 0; i < 8; i++) {
			out.writeBit(1&(curr->symbol>>(7-i))); 
		}
		return;
	}

	out.writeBit(0); /*NOT a leaf, so write 0*/
	headerlength++;
	if(curr->c0) { 
		writeHeader(curr->c0, out);
	}
	if(curr->c1) { 
		writeHeader(curr->c1, out);
	}
}

/*the tree deconstructor will pick out the root of the tree and recursively 
 * traverse all left and right childen and delete each and every single one*/
HCTree::~HCTree() {
	deleteTree(this->getRoot());
	//delete this;
	/*delete leaves*/
	this->getLeaves() = std::vector<HCNode*>();
}

/*takes in the root and recursively travels down the children and deletes 
 * all fo them*/
void HCTree::deleteTree(HCNode* curr) {

	if(curr->c0) {
		deleteTree(curr->c0);
	}

	if(curr->c1) {
		deleteTree(curr->c1);
	}

	delete curr;
}
