#include "CalendarTree.h"
//will free the tree
CalendarTree::~CalendarTree()
{
	freeTree(root);
	root = NULL;
}
//goes from treeNode to treeNode and deletes it
void CalendarTree::freeTree(Node *delRoot)
{
	if (delRoot == NULL)
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
	Node* newNodeToInsert, *tempP;
	CalendarEvent* res = nullptr;

	if (isInsertLegal(newEventToInsert))
	{
		newNodeToInsert = new Node;
		newNodeToInsert->setLeaf(newEventToInsert);

		if (root == NULL)
		{
			root = newNodeToInsert;
		}
			
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
			tempP = findParent(ev->getStartTime());
			insertHelper(newNodeToInsert, tempP);
		}

		return ev;
	}
	
}
//recursive function to insert the the node to its right place in the CalendarTree
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
		if (temp != NULL)
			insertHelper(temp, newNodeParent->getParnet());
	}
}
//if a node as more then 3 children the function will make a new node and divide the children between the two
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
		}
		else if (childMin < p->getmin(2))
		{
			newP->setLeftChild(p->getLeftChild());
			newP->setMiddleChild(child);
			newP->setMin1(p->getmin(1));
			newP->setMin2(childMin);
		}
		p->setLeftChild(p->getMiddleChild());
		p->setMiddleChild(p->getRightChild());
		p->setRightChild(NULL);
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
		}
		else if (childMin > p->getmin(3))
		{
			newP->setLeftChild(p->getRightChild());
			newP->setMiddleChild(child);
			newP->setMin1(p->getmin(3));
			newP->setMin2(child->getKey());
		}
		p->setRightChild(NULL);
		p->setMin3(-1);
	}
	p->fixParent();
	newP->fixParent();

	if (p->getParnet() == NULL)
	{
		Node *newRoot;
		if (p->getmin(1) > newP->getmin(1))
			newRoot = new Node(newP->getmin(1), p->getmin(1), -1, NULL, newP, p, NULL);
		else
			newRoot = new Node(p->getmin(1), newP->getmin(1), -1, NULL, p, newP, NULL);

		this->setRoot(newRoot);
		p->setParent(newRoot);
		newP->setParent(newRoot);
		return NULL;
	}
	return newP;
}
//uses FIND to find a key (that it gets as input) and deletes it from the tree
void CalendarTree::Delete(const treeKey &key)
{
	Node *delNode = Find(key);

	if (delNode == NULL || root == NULL)
	{
		cout << "key does not exist" << endl;
	}
	else if (root->isLeaf() && delNode == root)
	{
		delete root;
		root = NULL;
	}
	else
		deleteHelper(delNode->getParnet(), delNode);
}
//recursive function to delete a node
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
//gets a key from the input and finds the node that holds it
Node* CalendarTree::Find(const treeKey &key)const
{
	Node* curr;

	curr = root;
	if (curr == NULL)
		return NULL;

	while (curr != NULL)
	{
		if (curr->isLeaf() && curr->getKey() == key)
			return curr;
		else
		{
			if (curr->checkNumOfChildren() == THREE_CHILD && key >= curr->getmin(3))
				curr = curr->getRightChild();

			else if (key >= curr->getmin(2))
				curr = curr->getMiddleChild();

			else
				curr = curr->getLeftChild();
		}
	}
	return NULL;
}
// finds and returns the perent of a Node bi the ID
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

//goes over the tree and places the leafs in the list
void CalendarTree::PlaceLeafsInList(Node* node)
{

	if (node->isLeaf())
		leafList.push_back(*node);

	else
	{
		this->PlaceLeafsInList(node->getLeftChild());
		this->PlaceLeafsInList(node->getMiddleChild());


		if (node->checkNumOfChildren() == THREE_CHILD)
			this->PlaceLeafsInList(node->getRightChild());
	}

}
//print the data in the leaf list
void CalendarTree::printSorted()
{
	for (auto v : leafList)
	{
		v.getData()->print();
		cout << endl;
	}
}

CalendarEvent* CalendarTree::eventAt(time_t startTime)
{
	Node* leaf = Find(startTime);
	CalendarEvent* ev = nullptr;

	if (leaf != nullptr && leaf->getData()->isEventStillHappening(startTime))
		ev = leaf->getData();
	return ev;
}

CalendarEvent * CalendarTree::eventAfter(time_t startTime)
{
	Node* leaf = Find(startTime);
	CalendarEvent* ev = nullptr;

	if (leaf != nullptr && leaf->getNext() != nullptr)
		ev = leaf->getNext()->getEvent();
	
	return ev;
}

CalendarEvent * CalendarTree::deleteFirst()
{
	CalendarEvent* firstEvent;

	if (root == nullptr)
	{
		return nullptr;
	}
	else if (root->isLeaf())
	{
		firstEvent = root->getData();
		delete root;
		root = nullptr;
	}
	else
	{
		firstEvent = deleteHelper(root, nullptr);
	}
	return firstEvent;
}

bool CalendarTree::isInsertLegal(CalendarEvent* eventToInsert)
{
	bool isLegal = false;






	return isLegal;
}