#ifndef _NODE_H_
#define _NODE_H_

#include <iostream>
#include <string>
#include "CalendarEvent.h"

using namespace std;

typedef time_t treeKey;

#define ONE_CHILD 1
#define TWO_CHILD 2
#define THREE_CHILD 3
#define LEFT 4
#define MIDDLE 5
#define RIGHT 6
#define ROOT 7

class Node
{
	Node *leftChild, *middleChild, *rightChild, *parent, *prevBrother, *nextBrother;
	treeKey min1, min2, min3;
	treeKey key;
	CalendarEvent* data;

public:
	Node(const treeKey &_min1 = -1, const treeKey &_min2 = -1, const treeKey &_min3 = -1, Node *_parent = nullptr, Node *_leftChild = nullptr, Node *_middleChild = nullptr, Node *_rightChild = nullptr);
	~Node();

	//leaf functions
	bool isLeaf();
	void makeLeaf();
	void setLeaf(CalendarEvent* ev) { this->data = ev; this->key = ev->getStartTime(); }

	//get functions
	int getmin(int num)const;
	CalendarEvent* getData() { return data; }
	treeKey getKey()const { return key; }
	Node *getLeftChild()const { return leftChild; }
	Node *getMiddleChild()const { return middleChild; }
	Node *getRightChild()const { return rightChild; }
	Node *getParnet()const { return parent; }


	//set functions
	void setLeftChild(Node* new_left) { leftChild = new_left; }
	void setMiddleChild(Node* new_mid) { middleChild = new_mid; }
	void setRightChild(Node* new_right) { rightChild = new_right; }
	void setParent(Node* new_parent) { parent = new_parent; }
	void setMin1(const treeKey &m1) { min1 = m1; }
	void setMin2(const treeKey &m2) { min2 = m2; }
	void setMin3(const treeKey &m3) { min3 = m3; }
	void setNextBrother(Node* nextBrother);
	void setPrevBrother(Node* prevBrother);
	Node* getNextBrother() { return nextBrother; }
	Node* getPrevBrother() { return prevBrother; }

	int checkNumOfChildren();
	int whichAmI();
	void updateParnet();
	void insertToTwoChildNode(Node *newChild);
	void fixParent();
	void fixMins();

	void deleteNodeFromThree(Node *delNode);
	void deleteNodeFromTwo(Node *delNode);

};

#endif