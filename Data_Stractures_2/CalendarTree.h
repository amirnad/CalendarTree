#ifndef _CALENDARTREE_H_
#define _CALENDARTREE_H_

#include <iostream>
#include "Node.h"
#include <list>

using namespace std;
class CalendarTree
{
	Node* m_Root;

public:
	CalendarTree(Node* i_Root = nullptr) : m_Root(i_Root) {}
	~CalendarTree();
	void freeTree(Node* i_RootToDelete);

	CalendarEvent* insert(CalendarEvent* i_EventToInsert);
	void insertHelper(Node** i_Sibling, Node* i_CurRoot, Node* i_EventToInsert, Node* i_Parent);
	bool isInsertLegal(CalendarEvent* i_EventToInsert);
	CalendarEvent* deleteFirst();
	void deleteHelper(Node* i_CurrParent, Node* i_CurrChild);
	CalendarEvent* removeFirstEvent(Node* i_EventNode, Node* i_Parent);
	Node* getNextLevel(Node* i_CurRoot, treeKey i_Key);
	Node* findEvent(treeKey i_TimeToFind);
	CalendarEvent* eventAt(time_t i_TimeToFind);
	CalendarEvent* eventAfter(time_t i_TimeToFind);
	void fixKeys(Node* i_EventNode);
	Node* organizeTree(Node* i_EventNode, Node* i_Parent);
	void printSorted();
};

#endif