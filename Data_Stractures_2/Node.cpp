#include "Node.h"

Node::Node(const treeKey &m1, const treeKey &m2, const treeKey &m3, Node *p, Node *l, Node *m, Node *r) :
	leftChild(l), rightChild(r), middleChild(m), parent(p), min1(m1), min2(m2), min3(m3) {}

Node::~Node() {}
//takes a node and makes it a leaf node
void Node::makeLeaf()
{
	leftChild = NULL;
	middleChild = NULL;
	rightChild = NULL;
}
//returns true if node is leaf , false if not
bool Node::isLeaf()
{
	if (leftChild == NULL && middleChild == NULL && rightChild == NULL)
		return true;
	else
		return false;
}
//return the min that the user asked for
int Node::getmin(int num)const
{
	switch (num)
	{
	case 1:
		return min1;
		break;
	case 2:
		return min2;
		break;
	case 3:
		return min3;
		break;
	}
}
//goes into the parent field and updates it
void Node::updateParnet()
{
	if (!isLeaf())
	{
		leftChild->setParent(this);
		middleChild->setParent(this);
		if (rightChild != NULL)
			rightChild->setParent(this);
	}
}
//returns the number of the children that the nodes holds
int Node::checkNumOfChildren()
{
	if (rightChild == NULL && middleChild == NULL && leftChild != NULL)
		return ONE_CHILD;
	else if (rightChild == NULL && !(this->isLeaf()))
		return TWO_CHILD;
	else
		return THREE_CHILD;
}
//puts a new node into a parents that holds only 2 children
void Node::insertToTwoChildNode(Node *newChild)
{
	if (newChild->isLeaf()) //in case newChild is leaf node we only need to find its right place and insert it
	{
		if (newChild->getKey() > min2) //updating the pointers and min values of the parent
		{
			rightChild = newChild;
			min3 = newChild->getKey();
		}
		else if (newChild->getKey() > min1)
		{
			rightChild = middleChild;
			middleChild = newChild;
			min3 = min2;
			min2 = newChild->getKey();
		}
		else
		{
			rightChild = middleChild;
			middleChild = leftChild;
			leftChild = newChild;
			min3 = min2;
			min2 = min1;
			min1 = newChild->getKey();
		}
	}
	else //newChild is not a leaf but an internal node
	{
		if (newChild->getmin(1) < min1)
		{
			rightChild = middleChild;
			middleChild = leftChild;
			leftChild = newChild;
			min3 = min2;
			min2 = min1;
			min1 = newChild->getmin(1);
		}
		else if (newChild->getmin(1) < min2)
		{
			rightChild = middleChild;
			middleChild = newChild;
			min3 = min2;
			min2 = newChild->getmin(1);
		}
		else
		{
			rightChild = newChild;
			min3 = newChild->getmin(1);
		}
	}
	newChild->setParent(this);
}
//if any changes were made to some children this function will update to their right parent 
void Node::fixParent()
{
	this->getLeftChild()->setParent(this);
	this->getMiddleChild()->setParent(this);

	if (this->checkNumOfChildren() == THREE_CHILD)
		this->getRightChild()->setParent(this);
}

void Node::fixMins()
{
	if (!isLeaf())
	{
		min1 = this->getLeftChild()->getmin(1);
		min2 = this->getMiddleChild()->getmin(2);

		if (this->checkNumOfChildren() == THREE_CHILD)
			min3 = this->getRightChild()->getmin(3);
		else
			this->min3 = -1;
	}
}
void Node::deleteNodeFromThree(Node *delNode)
{
	if (delNode == leftChild)
	{
		delete delNode;
		leftChild = middleChild;
		middleChild = rightChild;
		min1 = min2;
		min2 = min3;
	}
	else if (delNode == middleChild)
	{
		delete delNode;
		middleChild = rightChild;
		min2 = min3;
	}
	else
		delete delNode;

	min3 = -1;
	rightChild = NULL;
}

void Node::deleteNodeFromTwo(Node *delNode)
{
	if (delNode == leftChild)
	{
		delete delNode;
		leftChild = middleChild;
	}
	else
		delete middleChild;

	middleChild = NULL;
	min2 = -1;
}

int Node::whichAmI()
{
	if (this == parent->getLeftChild())
		return LEFT;
	else if (this == parent->getMiddleChild())
		return MIDDLE;
	else if (this == parent->getRightChild())
		return RIGHT;
	else
		return ROOT;
}

void Node::setNextBrother(Node* _nextBrother)
{
	this->nextBrother = _nextBrother;
}

void Node::setPrevBrother(Node* _prevBrother)
{
	this->prevBrother = _prevBrother;
}
