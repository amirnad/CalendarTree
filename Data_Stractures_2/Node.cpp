#include "Node.h"

Node::Node(treeKey i_Min1, treeKey i_Min2, treeKey i_Min3, Node* i_Parent, Node* i_LeftChild, Node* i_MiddleChild, Node* i_RightChild) :
	m_LeftChild(i_LeftChild), m_RightChild(i_RightChild), m_MiddleChild(i_MiddleChild), m_Parent(i_Parent), m_Min1(i_Min1), m_Min2(i_Min2), m_Min3(i_Min3) {}

Node::Node(Node* i_LeftChild, Node* i_MiddleChild)
{ //sets a node with a left and a middle children
	setLeftChild(i_LeftChild);
	setMiddleChild(i_MiddleChild);
	i_LeftChild->setParent(this);
	i_MiddleChild->setParent(this);
	setNodeMin(i_LeftChild);
	setNodeMin(i_MiddleChild);
}

Node::~Node() {}

//Checks if a node is a leaf
bool Node::isLeaf()
{
	bool isLeaf = false;

	if ((m_LeftChild == nullptr) && (m_MiddleChild == nullptr) && (m_RightChild == nullptr))
	{
		isLeaf = true;
	}
	else
	{
		isLeaf = false;
	}

	return isLeaf;
}

//Returns the min1\min2\min3
treeKey Node::getMin(int i_WantedMin)
{
	switch (i_WantedMin)
	{
	case 1:
		return m_Min1;
		break;
	case 2:
		return m_Min2;
		break;
	case 3:
		return m_Min3;
		break;
	}
}

//Check the number of children of a node
int Node::checkNumOfChildren()
{
	if ((m_RightChild == nullptr) && (m_MiddleChild == nullptr) && (m_LeftChild != nullptr))
	{
		return ONE_CHILD;
	}
	else if ((m_RightChild == nullptr) && !(this->isLeaf()))
	{
		return TWO_CHILD;
	}
	else
	{
		return THREE_CHILD;
	}
}

//Gets a node and inserts it into a two children father
void Node::insertToTwoChildNode(Node* i_EventToInsert)
{
	treeKey key;
	//first getting the min value of the new node
	if (i_EventToInsert->isLeaf())
	{
		key = i_EventToInsert->getKey();
	}
	else
	{
		key = i_EventToInsert->getMin(1);
	}
	//find and insert in the correct place
	if (key <= m_Min1)
	{
		m_RightChild = m_MiddleChild;
		m_MiddleChild = m_LeftChild;
		m_LeftChild = i_EventToInsert;
		if (i_EventToInsert->isLeaf())
		{
			i_EventToInsert->setNextBrother(m_MiddleChild);
			i_EventToInsert->setPrevBrother(m_MiddleChild->getPrevBrother());
			m_MiddleChild->setPrevBrother(i_EventToInsert);
		}
	}
	else if (key <= m_Min2)
	{
		m_RightChild = m_MiddleChild;
		m_MiddleChild = i_EventToInsert;
		if (i_EventToInsert->isLeaf())
		{
			m_LeftChild->setNextBrother(i_EventToInsert);
			i_EventToInsert->setNextBrother(m_RightChild);
			i_EventToInsert->setPrevBrother(m_LeftChild);
			m_RightChild->setPrevBrother(i_EventToInsert);
		}
	}
	else
	{
		m_RightChild = i_EventToInsert;
		if (i_EventToInsert->isLeaf())
		{
			i_EventToInsert->setPrevBrother(m_MiddleChild);
			i_EventToInsert->setNextBrother(m_MiddleChild->getNextBrother());
			m_MiddleChild->setNextBrother(i_EventToInsert);
		}
	}
	
	fixMins();
	fixParent();
}

/*Dividing a node -  in case we want to insert a node into a 3 children node, we have to divide it first into 2 nodes of 2 children each
					 First we will get the minimum , and then find the correct splitting place, create a new node with 2 values and update the values
					 on the original node , so evetually we have 2 nodes with 2 children each*/
Node* Node::divideNode(Node* i_EventToInsert)
{
	Node* res = nullptr;
	treeKey childKey;
	if (i_EventToInsert->isLeaf())
	{
		childKey = i_EventToInsert->getKey();
	}
	else
	{
		childKey = i_EventToInsert->getMin(1);
	}

	if (childKey <= m_Min1)
	{
		res = new Node(i_EventToInsert, m_LeftChild);
		if (i_EventToInsert->isLeaf())
		{
			i_EventToInsert->setPrevBrother(m_LeftChild->getPrevBrother());
			i_EventToInsert->setNextBrother(m_LeftChild);
			m_LeftChild->setPrevBrother(i_EventToInsert);
		}
		shiftNodeToLeft();
	}
	else if (childKey <= m_Min2)
	{
		res = new Node(m_LeftChild, i_EventToInsert);
		if (i_EventToInsert->isLeaf())
		{
			m_LeftChild->setNextBrother(i_EventToInsert);
			m_MiddleChild->setPrevBrother(i_EventToInsert);
			i_EventToInsert->setPrevBrother(m_LeftChild);
			i_EventToInsert->setNextBrother(m_MiddleChild);
		}
		shiftNodeToLeft();
	}
	else if (childKey <= m_Min3)
	{
		res = new Node(m_LeftChild, m_MiddleChild);
		if (i_EventToInsert->isLeaf())
		{
			m_MiddleChild->setNextBrother(i_EventToInsert);
			m_RightChild->setPrevBrother(i_EventToInsert);
			i_EventToInsert->setNextBrother(m_RightChild);
			i_EventToInsert->setPrevBrother(m_MiddleChild);
		}
		m_LeftChild = i_EventToInsert;
		m_MiddleChild = m_RightChild;
	}
	else
	{
		res = new Node(m_LeftChild, m_MiddleChild);
		if (i_EventToInsert->isLeaf())
		{
			i_EventToInsert->setNextBrother(m_RightChild->getNextBrother());
			i_EventToInsert->setPrevBrother(m_RightChild);
			m_RightChild->setNextBrother(i_EventToInsert);
		}
		m_LeftChild = m_RightChild;
		m_MiddleChild = i_EventToInsert;
		m_RightChild = nullptr;
	}

	m_RightChild = nullptr;
	this->fixParent();
	res->fixParent();
	this->fixMins();
	res->fixMins();
	
	return res;
}

//if any changes were made to some children this function will update to their right parent 
void Node::fixParent()
{
	this->getLeftChild()->setParent(this);
	this->getMiddleChild()->setParent(this);

	if (this->checkNumOfChildren() == THREE_CHILD)
		this->getRightChild()->setParent(this);
}

//delete a child from a three children node
void Node::deleteNodeFromThree(Node* i_WantedEventToDelete)
{
	if (i_WantedEventToDelete == m_LeftChild)
	{
		delete i_WantedEventToDelete;
		m_LeftChild = m_MiddleChild;
		m_MiddleChild = m_RightChild;
		m_Min1 = m_Min2;
		m_Min2 = m_Min3;
	}
	else if (i_WantedEventToDelete == m_MiddleChild)
	{
		delete i_WantedEventToDelete;
		m_MiddleChild = m_RightChild;
		m_Min2 = m_Min3;
	}
	else
	{
		delete i_WantedEventToDelete;
	}

	m_Min3 = -1;
	m_RightChild = nullptr;
}

//delete a child from a 2 children node
void Node::deleteNodeFromTwo(Node* i_WantedEventToDelete)
{
	if (i_WantedEventToDelete == m_LeftChild)
	{
		delete i_WantedEventToDelete;
		m_LeftChild = m_MiddleChild;
	}
	else
	{
		delete m_MiddleChild;
	}

	m_MiddleChild = nullptr;
	m_Min2 = -1;
}

//inspects which child is the node
int Node::findChildPlace()
{
	if (this == m_Parent->getLeftChild())
	{
		return LEFT;
	}
	else if (this == m_Parent->getMiddleChild())
	{
		return MIDDLE;
	}
	else if (this == m_Parent->getRightChild())
	{
		return RIGHT;
	}
	else
	{
		return ROOT;
	}
}

//fix all the minimum values of a node
void Node::fixMins()
{
	if (!isLeaf())
	{
		setMin(1, m_LeftChild);
		setMin(2, m_MiddleChild);
		setMin(3, m_RightChild);
	}
}

int Node::setNodeMin(Node* i_NodeToSet)
{
	int min = -1;
	if (i_NodeToSet != nullptr)
	{

		if (i_NodeToSet->isLeaf())
		{
			min = i_NodeToSet->getKey();
		}
		else
		{
			min = i_NodeToSet->getMin(1);
		}
	}

	return min;
}

void Node::setMin(int i_WantedMin, Node* i_GetMinFrom)
{
	if (i_GetMinFrom != nullptr)
	{
		if (i_GetMinFrom->isLeaf())
		{
			switch (i_WantedMin)
			{
			case 1:
				m_Min1 = i_GetMinFrom->getKey();
				break;

			case 2:
				m_Min2 = i_GetMinFrom->getKey();;
				break;

			case 3:
				m_Min3 = i_GetMinFrom->getKey();;
				break;
			}
		}
		else
		{
			switch (i_WantedMin)
			{
			case 1:
				m_Min1 = i_GetMinFrom->getMin(1);
				break;

			case 2:
				m_Min2 = i_GetMinFrom->getMin(1);
				break;

			case 3:
				m_Min3 = i_GetMinFrom->getMin(1);
				break;
			}
		}
	}
}