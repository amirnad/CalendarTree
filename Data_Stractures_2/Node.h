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
	treeKey m_Key;
	CalendarEvent* m_Data;
	treeKey m_Min1, m_Min2, m_Min3;
	Node* m_LeftChild = nullptr, *m_MiddleChild = nullptr, *m_RightChild = nullptr;
	Node* m_PrevBrother = nullptr, *m_NextBrother = nullptr;
	Node* m_Parent = nullptr;

public:
	Node(
		treeKey i_Min1 = -1,
		treeKey i_Min2 = -1,
		treeKey i_Min3 = -1,
		Node* i_Parent = nullptr,
		Node* i_LeftChild = nullptr,
		Node* i_MiddleChild = nullptr,
		Node* i_RightChild = nullptr);
	Node(Node* i_LeftChild, Node* i_MiddleChild);
	~Node();

	//leaf functions
	bool isLeaf();
	void setLeaf(CalendarEvent* i_EventToInsert) { m_Data = i_EventToInsert; m_Key = i_EventToInsert->getStartTime(); }

	//get functions
	treeKey getKey() { return m_Key; }
	CalendarEvent* getData() { return m_Data; }
	treeKey getMin(int i_WantedMin);
	Node *getLeftChild() { return m_LeftChild; }
	Node *getMiddleChild() { return m_MiddleChild; }
	Node *getRightChild() { return m_RightChild; }
	Node* getNextBrother() { return m_NextBrother; }
	Node* getPrevBrother() { return m_PrevBrother; }
	Node *getParnet() { return m_Parent; }

	//set functions
	void setLeftChild(Node* i_LeftChild) { m_LeftChild = i_LeftChild; }
	void setMiddleChild(Node* i_MiddleChild) { m_MiddleChild = i_MiddleChild; }
	void setRightChild(Node* i_RightChild) { m_RightChild = i_RightChild; }
	void setParent(Node* i_Parent) { m_Parent = i_Parent; }
	void setMin1(treeKey i_Min1) { m_Min1 = i_Min1; }
	void setMin2(treeKey i_Min2) { m_Min2 = i_Min2; }
	void setMin3(treeKey i_Min3) { m_Min3 = i_Min3; }
	void setNextBrother(Node* i_NextBrother) { m_NextBrother = i_NextBrother; }
	void setPrevBrother(Node* i_PrevBrother) { m_PrevBrother = i_PrevBrother; }
	void setMin(int i_WantedMin, Node* i_GetMinFrom);
	int setNodeMin(Node* i_NodeToSet);

	void deleteNodeFromThree(Node* i_WantedEventToDelete);
	void deleteNodeFromTwo(Node* i_WantedEventToDelete);
	Node* divideNode(Node* i_EventToInser);
	int checkNumOfChildren();
	int findChildPlace();
	void insertToTwoChildNode(Node* i_EventToInsert);
	void fixParent();
	void fixMins();
	void shiftNodeToLeft() { m_LeftChild = m_MiddleChild, m_MiddleChild = m_RightChild, m_RightChild = nullptr; }
	void shiftNodeToRight() { m_RightChild = m_MiddleChild, m_MiddleChild = m_LeftChild, m_LeftChild = nullptr; }



};

#endif