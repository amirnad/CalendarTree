#include "CalendarTree.h"

//will free the tree
CalendarTree::~CalendarTree()
{
	freeTree(root);
	root = nullptr;
}

//goes from treeNode to treeNode and deletes it
void CalendarTree::freeTree(Node *delRoot)
{
	if (delRoot == nullptr)
		return;
	else if (delRoot->isLeaf())
		delete delRoot;
	else
	{
		freeTree(delRoot->getLeftChild());
		freeTree(delRoot->getMiddleChild());
		freeTree(delRoot->getRightChild());
	}
}

//gets a key or data from the input and adds it to the right place in the 2-3 tree
CalendarEvent* CalendarTree::insert(CalendarEvent* newEventToInsert)
{
	Node* newNodeToInsert, *tempParent;
	CalendarEvent* res = nullptr;

	if (isInsertLegal(newEventToInsert))
	{
		newNodeToInsert = new Node;
		newNodeToInsert->setLeaf(newEventToInsert);

		//The tree is empty
		if (root == NULL)
		{
			root = newNodeToInsert;
		}

		//There is already one leaf
		else if (root->isLeaf())
		{
			Node* newRoot;
			if (root->getKey() > newEventToInsert->getStartTime())
			{
				newRoot = new Node(newEventToInsert->getStartTime(), root->getKey(), -1, NULL, newNodeToInsert, root, NULL);
				newNodeToInsert->setNextBrother(root);
				root->setPrevBrother(newNodeToInsert);
			}

			else
			{
				newRoot = new Node(root->getKey(), newEventToInsert->getStartTime(), -1, NULL, root, newNodeToInsert, NULL);
				newNodeToInsert->setNextBrother(newNodeToInsert);
				root->setPrevBrother(root);
			}

			newNodeToInsert->setParent(newRoot);
			root->setParent(newRoot);
			root = newRoot;
		}
		else
		{
			tempParent = findParent(newEventToInsert->getStartTime());
			insertHelper(newNodeToInsert, tempParent);
		}

		res = newEventToInsert;
	}

	return res;
}

//Recursive function to insert the the node to its right place in the CalendarTree
void CalendarTree::insertHelper(Node* newNode, Node* newNodeParent)
{
	if (newNodeParent->checkNumOfChildren() == TWO_CHILD)
	{
		newNodeParent->insertToTwoChildNode(newNode);
		return;
	}

	else if (newNodeParent->checkNumOfChildren() == THREE_CHILD)
	{
		Node *temp;

		temp = divideNode(newNodeParent, newNode);
		if (temp != nullptr)
			insertHelper(temp, newNodeParent->getParnet());
	}
}

//When adding a child to a 3 child node, split the node
Node* CalendarTree::divideNode(Node* p, Node *child)
{
	Node *newP = new Node;
	treeKey childMin;

	if (child->isLeaf())
		childMin = child->getKey();
	else
		childMin = child->getmin(1);


	if (childMin < p->getmin(1) || childMin < p->getmin(2))
	{
		if (childMin < p->getmin(1))
		{
			newP->setLeftChild(child);
			newP->setMiddleChild(p->getLeftChild());
			newP->setMin1(childMin);
			newP->setMin2(p->getmin(1));
			if (child->isLeaf())
			{
				newP->getLeftChild()->setNextBrother(newP->getMiddleChild());
				newP->getLeftChild()->setPrevBrother(nullptr);
				newP->getMiddleChild()->setPrevBrother(newP->getLeftChild());
			}

		}
		else if (childMin < p->getmin(2))
		{
			newP->setLeftChild(p->getLeftChild());
			newP->setMiddleChild(child);
			newP->setMin1(p->getmin(1));
			newP->setMin2(childMin);

			if (child->isLeaf())
			{
				newP->getMiddleChild()->setNextBrother(newP->getLeftChild()->getNextBrother());
				newP->getLeftChild()->setNextBrother(newP->getMiddleChild());
				p->getLeftChild()->setPrevBrother(newP->getMiddleChild());
				newP->getMiddleChild()->setPrevBrother(newP->getLeftChild());
			}
		}
		p->setLeftChild(p->getMiddleChild());
		p->setMiddleChild(p->getRightChild());
		p->setRightChild(nullptr);
		p->setMin1(p->getmin(2));
		p->setMin2(p->getmin(3));
		p->setMin3(-1);
	}
	else
	{
		if (childMin < p->getmin(3))
		{
			newP->setLeftChild(child);
			newP->setMin1(childMin);
			newP->setMiddleChild(p->getRightChild());
			newP->setMin2(p->getmin(3));

			if (child->isLeaf())
			{
				newP->getLeftChild()->setNextBrother(newP->getMiddleChild());
				p->getMiddleChild()->setNextBrother(newP->getLeftChild());
				newP->getMiddleChild()->setPrevBrother(newP->getLeftChild());
				newP->getLeftChild()->setPrevBrother(p->getMiddleChild());
			}
		}
		else if (childMin > p->getmin(3))
		{
			newP->setLeftChild(p->getRightChild());
			newP->setMiddleChild(child);
			newP->setMin1(p->getmin(3));
			newP->setMin2(child->getKey());

			if (child->isLeaf())
			{
				newP->getLeftChild()->setNextBrother(newP->getMiddleChild());
				newP->getMiddleChild()->setPrevBrother(newP->getLeftChild());
				newP->getMiddleChild()->setNextBrother(nullptr);
			}
		}
		p->setRightChild(nullptr);
		p->setMin3(-1);
	}
	p->fixParent();
	newP->fixParent();

	if (p->getParnet() == nullptr)
	{
		Node *newRoot;
		if (p->getmin(1) > newP->getmin(1))
			newRoot = new Node(newP->getmin(1), p->getmin(1), -1, nullptr, newP, p, nullptr);
		else
			newRoot = new Node(p->getmin(1), newP->getmin(1), -1, nullptr, p, newP, nullptr);

		this->setRoot(newRoot);
		p->setParent(newRoot);
		newP->setParent(newRoot);

		return nullptr;
	}
	return newP;
}

//Uses FIND to find a key (that it gets as input) and deletes it from the tree
void CalendarTree::Delete(const treeKey &key)
{
	Node *delNode = Find(key);

	if (delNode == nullptr || root == nullptr)
	{
		cout << "key does not exist" << endl;
	}
	else if (root->isLeaf() && delNode == root)
	{
		delete root;
		root = nullptr;
	}
	else
	{
		deleteHelper(delNode->getParnet(), delNode);
	}
}

//Recursive function to delete a node
void CalendarTree::deleteHelper(Node* currParent, Node* currChild)
{
	int whichChild, num;
	Node *temp;

	if (currParent->checkNumOfChildren() == THREE_CHILD)
		currParent->deleteNodeFromThree(currChild);
	else
	{
		currParent->deleteNodeFromTwo(currChild);
		whichChild = currParent->whichAmI();
		switch (whichChild)
		{
		case LEFT:
			temp = currParent->getParnet()->getMiddleChild();
			if (temp->checkNumOfChildren() == THREE_CHILD)
			{
				currParent->setMiddleChild(temp->getLeftChild());
				temp->setLeftChild(temp->getMiddleChild());
				temp->setMiddleChild(temp->getRightChild());
				temp->setRightChild(NULL);
				currParent->setMin2(temp->getmin(1));
				temp->setMin1(temp->getmin(2));
				temp->setMin2(temp->getmin(3));
				temp->setMin3(-1);
			}
			else
			{
				temp->setRightChild(temp->getMiddleChild());
				temp->setMiddleChild(temp->getLeftChild());
				temp->setLeftChild(currParent->getLeftChild());
				temp->setMin1(currParent->getmin(1));
				num = temp->getmin(2);
				temp->setMin2(temp->getmin(3));
				temp->setMin3(num);
				deleteHelper(currParent->getParnet(), currParent);
			}
			break;

		case MIDDLE:
			temp = currParent->getParnet()->getLeftChild();
			if (temp->checkNumOfChildren() == THREE_CHILD)
			{
				currParent->setMiddleChild(currParent->getLeftChild());
				currParent->setLeftChild(temp->getRightChild());
				temp->setRightChild(NULL);
				currParent->setMin2(currParent->getmin(1));
				currParent->setMin1(temp->getmin(3));
				temp->setMin3(-1);
			}
			else
			{
				temp->setRightChild(currParent->getLeftChild());
				temp->setMin3(currParent->getmin(1));
				currParent->setLeftChild(NULL);
				currParent->setMin1(-1);
				deleteHelper(currParent->getParnet(), currParent);
			}
			break;

		case RIGHT:
			temp = currParent->getParnet()->getMiddleChild();
			if (temp->checkNumOfChildren() == THREE_CHILD)
			{
				currParent->setMiddleChild(currParent->getLeftChild());
				currParent->setLeftChild(temp->getRightChild());
				temp->setRightChild(NULL);
				currParent->setMin2(currParent->getmin(1));
				currParent->setMin1(temp->getmin(3));
				temp->setMin3(-1);
			}
			else
			{
				temp->setRightChild(currParent->getLeftChild());
				temp->setMin3(currParent->getmin(1));
				currParent->setLeftChild(NULL);
				currParent->setMin1(-1);
				deleteHelper(currParent->getParnet(), currParent);
			}
			break;

		case ROOT:
			temp = currParent->getLeftChild();
			delete root;
			root = temp;
			root->setParent(NULL);
			fixKeys(temp);
			break;
		}
	}
}

//Gets a key from the input and finds the node that holds it
Node* CalendarTree::Find(const treeKey &key)const
{
	Node* curr;

	curr = root;
	if (curr == nullptr)
		return nullptr;

	while (curr != nullptr)
	{
		if (curr->isLeaf())
			return curr;
		else
		{
			if (curr->checkNumOfChildren() == THREE_CHILD && key >= curr->getmin(THREE_CHILD))
				curr = curr->getRightChild();

			else if (key >= curr->getmin(TWO_CHILD))
				curr = curr->getMiddleChild();

			else
				curr = curr->getLeftChild();
		}
	}

	return nullptr;
}

//Finds and returns the parent of a node
Node* CalendarTree::findParent(const treeKey &ID)const
{
	Node * temp;
	temp = root;

	while (temp->getLeftChild() != NULL)
	{
		if (temp->checkNumOfChildren() == THREE_CHILD && ID > temp->getmin(3))
			temp = temp->getRightChild();

		else if (ID > temp->getmin(2))
			temp = temp->getMiddleChild();

		else
			temp = temp->getLeftChild();
	}
	return temp->getParnet();
}

void CalendarTree::fixKeys(Node *node)
{
	if (node == NULL)
		return;
	else
	{
		node->fixMins();
		fixKeys(node->getParnet());
	}
}

//Print the data sorted
void CalendarTree::printSorted()
{
	Node* curr = root;

	while (curr->getLeftChild() != nullptr)
	{
		curr = curr->getLeftChild();
	}
	
	while (curr != nullptr)
	{
		curr->getData()->print();
		curr = curr->getNextBrother();
	}
	
}

CalendarEvent* CalendarTree::eventAt(time_t newEventStartTime)
{
	Node* requestedLeaf = Find(newEventStartTime);
	CalendarEvent* requestedEvent = nullptr;

	if ((requestedLeaf != nullptr) && (requestedLeaf->getData()->isEventStillHappening(newEventStartTime)))
	{
		requestedEvent = requestedLeaf->getData();
	}

	return requestedEvent;
}

CalendarEvent * CalendarTree::eventAfter(time_t startTime)
{
	Node* requestedLeaf = Find(startTime);
	CalendarEvent* requestedEvent = nullptr;

	if ((requestedLeaf != nullptr) && (requestedLeaf->getKey() >= startTime))
	{
		requestedEvent = requestedLeaf->getData();
	}
	else if ((requestedLeaf != nullptr) && (requestedLeaf->getNextBrother() != nullptr))
	{
		requestedEvent = requestedLeaf->getNextBrother()->getData();
	}

	return requestedEvent;
}

bool CalendarTree::isInsertLegal(CalendarEvent* eventToInsert)
{
	bool isLegal = true;

	treeKey newEventStartTime = eventToInsert->getStartTime();
	treeKey newEventEndTime = newEventStartTime + eventToInsert->getDuration();
	Node* currLeaf = Find(newEventStartTime);
	if (currLeaf != nullptr && currLeaf->getData()->isWithinBounds(newEventStartTime, newEventEndTime))
	{
		isLegal = false;
	}

	return isLegal;
}

	