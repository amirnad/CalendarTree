#ifndef _CALENDARTREE_H_
#define _CALENDARTREE_H_

#include <iostream>
#include "Node.h"
#include <list>

using namespace std;
enum eTypeOfFind { RegularFind = 1, EventAt, EventAfter };
class CalendarTree
{
	Node *root;
	list<Node> leafList;

public:
	CalendarTree(Node *root = NULL) :root(root) {}
	~CalendarTree();

	void setRoot(Node* newRoot) { root = newRoot; }
	Node* getRoot() { return root; }

	CalendarEvent* insert(CalendarEvent* event);
	void insertHelper(Node* newNode, Node* newNodeParent);
	Node* divideNode(Node* p, Node *child);
	void Delete(const treeKey &ID);
	void deleteHelper(Node* p, Node* p_child);
	Node* Find(const treeKey &_ID, eTypeOfFind whichFind)const;
	Node* findParent(const treeKey &ID)const;
	void fixKeys(Node *node);
	void freeTree(Node *delRoot);
	void PlaceLeafsInList(Node* node);
	void printSorted();
	bool isInsertLegal(CalendarEvent* eventToInsert);

	CalendarEvent* eventAt(time_t startTime);
	CalendarEvent* eventAfter(time_t startTime);
	CalendarEvent* deleteFirst();
	
};

#endif