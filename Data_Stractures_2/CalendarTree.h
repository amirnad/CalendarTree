#ifndef _CALENDARTREE_H_
#define _CALENDARTREE_H_

#include <iostream>
#include "Node.h"
#include <list>

using namespace std;
class CalendarTree
{
	Node *root;

public:
	CalendarTree(Node *root = nullptr) :root(root) {}
	~CalendarTree();
	void freeTree(Node *delRoot);

	void setRoot(Node* newRoot) { root = newRoot; }
	Node* getRoot() { return root; }

	CalendarEvent* insert(CalendarEvent* eventToInsert);
	bool isInsertLegal(CalendarEvent* eventToInsert);
	void insertHelper(Node *currentRoot, Node *nodeToInsert, Node *parent, Node **sibling);
	Node* getNextLevel(Node *curRoot, const treeKey &ID)const;

	void Delete(const treeKey &ID);
	void deleteHelper(Node* p, Node* p_child);
	Node* Find(const treeKey &_ID)const;
	void fixKeys(Node *node);

	void printSorted();

	CalendarEvent* eventAt(time_t newEventStartTime);
	CalendarEvent* eventAfter(time_t newEventStartTime);
	
	CalendarEvent* deleteFirst();
	CalendarEvent* removeFirstEvent(Node* node, Node* parent);
	Node* organizeTree(Node* node, Node* parent);
};

#endif