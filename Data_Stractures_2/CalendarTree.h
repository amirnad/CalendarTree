#ifndef _CALENDARTREE_H_
#define _CALENDARTREE_H_

#include <iostream>
#include "Node.h"
#include <list>

using namespace std;
class CalendarTree
{
	Node *root;
	list<Node> leafList;

public:
	CalendarTree(Node *root = nullptr) :root(root) {}
	~CalendarTree();

	void setRoot(Node* newRoot) { root = newRoot; }
	Node* getRoot() { return root; }

	CalendarEvent* insert(CalendarEvent* eventToInsert);
	//void insertHelper(Node* newNode, Node* newNodeParent);
	void insertHelper(Node *currentRoot, Node *nodeToInsert, Node *parent, Node **sibling);

	//Node* divideNode(Node* p, Node *child);
	void Delete(const treeKey &ID);
	void deleteHelper(Node* p, Node* p_child);
	Node* Find(const treeKey &_ID)const;
	Node* findParent(const treeKey &ID)const;
	Node* getNextLevel(Node *curRoot, const treeKey &ID)const;
	void fixKeys(Node *node);
	void freeTree(Node *delRoot);
	void printSorted();
	bool isInsertLegal(CalendarEvent* eventToInsert);

	CalendarEvent* eventAt(time_t startTime);
	CalendarEvent* eventAfter(time_t startTime);
	CalendarEvent* deleteFirst();
	CalendarEvent* removeFirstEvent(Node* node, Node* parent);
	Node* organizeTree(Node* node, Node* Parent);
};

#endif