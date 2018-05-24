#include "Node.h"

Node::Node(const treeKey &_min1, const treeKey &_min2, const treeKey &_min3, Node *_parent, Node *_leftChild, Node *_middleChild, Node *_rightChild) :
	leftChild(_leftChild), rightChild(_rightChild), middleChild(_middleChild), parent(_parent), min1(_min1), min2(_min2), min3(_min3) {}

Node::~Node() {}

//Turns node into a leaf
void Node::makeLeaf()
{
	leftChild = nullptr;
	middleChild = nullptr;
	rightChild = nullptr;
}

//Checks if a node is a leaf
bool Node::isLeaf()
{
	bool isLeaf = false;

	if ((leftChild == nullptr) && (middleChild == nullptr) && (rightChild == nullptr))
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

//Updates the parent of a node
void Node::updateParnet()
{
	if (!isLeaf())
	{
		leftChild->setParent(this);
		middleChild->setParent(this);
		if (rightChild != nullptr)
			rightChild->setParent(this);
	}
}

//Check the number of children of a node
int Node::checkNumOfChildren()
{
	if ((rightChild == nullptr) && (middleChild == nullptr) && (leftChild != nullptr))
	{
		return ONE_CHILD;
	}
	else if ((rightChild == nullptr) && !(this->isLeaf()))
	{
		return TWO_CHILD;
	}
	else
	{
		return THREE_CHILD;
	}
}

//puts a new node into a parents that holds only 2 children
void Node::insertToTwoChildNode(Node *newChildToInsert)
{
	if (newChildToInsert->isLeaf()) //in case newChild is leaf node we only need to find its right place and insert it
	{
		if (newChildToInsert->getKey() > min2) //updating the pointers and min values of the parent
		{
			rightChild = newChildToInsert;
			newChildToInsert->setPrevBrother(middleChild);
			newChildToInsert->setNextBrother(middleChild->getNextBrother());
			min3 = newChildToInsert->getKey();
		}
		else if (newChildToInsert->getKey() > min1)
		{
			rightChild = middleChild;
			middleChild = newChildToInsert;
			min3 = min2;
			leftChild->setNextBrother(newChildToInsert);
			newChildToInsert->setPrevBrother(leftChild);
			newChildToInsert->setNextBrother(rightChild);
			rightChild->setPrevBrother(newChildToInsert);
			min2 = newChildToInsert->getKey();
		}
		else
		{
			rightChild = middleChild;
			middleChild = leftChild;
			leftChild = newChildToInsert;
			newChildToInsert->setNextBrother(middleChild);
			newChildToInsert->setPrevBrother(middleChild->getPrevBrother());
			middleChild->setPrevBrother(newChildToInsert);
			min3 = min2;
			min2 = min1;
			min1 = newChildToInsert->getKey();
		}
	}
	else //newChild is not a leaf but an internal node
	{
		if (newChildToInsert->getmin(1) < min1)
		{
			rightChild = middleChild;
			middleChild = leftChild;
			leftChild = newChildToInsert;
			min3 = min2;
			min2 = min1;
			min1 = newChildToInsert->getmin(1);

		}
		else if (newChildToInsert->getmin(1) < min2)
		{
			rightChild = middleChild;
			middleChild = newChildToInsert;
			min3 = min2;
			min2 = newChildToInsert->getmin(1);
		}
		else
		{
			rightChild = newChildToInsert;
			min3 = newChildToInsert->getmin(1);
		}
	}
	newChildToInsert->setParent(this);
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
		min1 = setNodeMin(this->getLeftChild());
		min2 = setNodeMin(this->getMiddleChild());
		min3 = setNodeMin(this->getRightChild());
	}

}

int Node::setNodeMin(Node* nodeToSet)
{
	int min = -1;
	if (nodeToSet != nullptr)
	{

		if (nodeToSet->isLeaf())
		{
			min = nodeToSet->getKey();
		}
		else
		{
			min = nodeToSet->getmin(1);
		}
	}
	return min;
}


void Node::orderNodeToLeft()
{
	leftChild = middleChild;
	middleChild = rightChild;
	rightChild = nullptr;
}

void Node::orderNodeToRight()
{
	rightChild = middleChild;
	middleChild = leftChild;
	leftChild = nullptr;
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
	rightChild = nullptr;
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

	middleChild = nullptr;
	min2 = -1;
}

int Node::whichAmI()
{
	if (this == parent->getLeftChild())
	{
		return LEFT;
	}
	else if (this == parent->getMiddleChild())
	{
		return MIDDLE;
	}
	else if (this == parent->getRightChild())
	{
		return RIGHT;
	}
	else
	{
		return ROOT;
	}
}

void Node::setNextBrother(Node* _nextBrother)
{
	this->nextBrother = _nextBrother;
}

void Node::setPrevBrother(Node* _prevBrother)
{
	this->prevBrother = _prevBrother;
}
