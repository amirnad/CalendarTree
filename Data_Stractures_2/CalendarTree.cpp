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

CalendarEvent* CalendarTree::insert(CalendarEvent* eventToInsert)
{
	Node *nodeToInsert = nullptr, *sibling = nullptr, *newRoot = nullptr;
	CalendarEvent *returnedEvent = nullptr;

	if (isInsertLegal(eventToInsert))
	{
		nodeToInsert = new Node();
		nodeToInsert->setLeaf(eventToInsert);

		//The tree is empty
		if (root == nullptr)
		{
			root = nodeToInsert;
		}
		//There is already one leaf
		else if (root->isLeaf())
		{
			//check in which order to insert the nodes
			if (root->getKey() < nodeToInsert->getKey())
			{
				newRoot = new Node(root, nodeToInsert);
				root->setNextBrother(nodeToInsert);
				nodeToInsert->setPrevBrother(root);
			}
			else
			{
				newRoot = new Node(nodeToInsert, root);
				nodeToInsert->setNextBrother(root);
				root->setPrevBrother(nodeToInsert);
			}
			//make sure the node values are set properly
			newRoot->fixMins();
			root = newRoot;
		}
		else
		{
			//in this case - the root isn't a leaf so we go the recusive function in order to insert currectly
			insertHelper(root, nodeToInsert, nullptr, &sibling);
		}

		returnedEvent = eventToInsert;
	}
	return returnedEvent;
}

//Recursive function to insert the the node to its right place in the CalendarTree
/*THE IDEA: Traverse the tree until we get the place in which we need to insert the new node.
			We traverse the tree one level at a time until we get to the requested level.
			At the requested level - if it has 2 children, we can insert our node safely and finish.
			Otherwise - we use a division method in order to split the place in which we want to insert to 2 parts.
			In case we did that, we need to connect the new node to the tree in the if statement starting in line 96.*/
void CalendarTree::insertHelper(Node *currentRoot, Node *nodeToInsert, Node *parent, Node **sibling)
{
	Node *temp = nullptr;
	Node *tempParent = nullptr;
	Node *newRoot = nullptr;

	if (currentRoot->getLeftChild()->isLeaf())
	{
		if (currentRoot->checkNumOfChildren() == TWO_CHILD)
			currentRoot->insertToTwoChildNode(nodeToInsert);
		else//no sapce - divide the node into 2 parts in order to insert the new node
			(*sibling) = currentRoot->divideNode(nodeToInsert);
	}
	else
	{
		tempParent = getNextLevel(currentRoot, nodeToInsert->getKey());
		insertHelper(tempParent, nodeToInsert, currentRoot, sibling);
	}

	if (*sibling != nullptr) //in case we created a new node at divideNode method --> we must connect it to the tree
	{
		if (parent == nullptr)
		{//connecting the 2 new nodes to a new parent
			newRoot = new Node((*sibling), currentRoot);
			root = newRoot;
		}
		else
		{//adding the 1 new node to an existing parent
			parent->fixMins();
			if (parent->checkNumOfChildren() == TWO_CHILD)
			{
				parent->insertToTwoChildNode(*sibling);
				*sibling = nullptr;
			}
			else
			{
				*sibling = parent->divideNode(*sibling);
			}

		}
	}
	currentRoot->fixMins(); //make sure min values are set properly before leaving the method
}

//Going down one level in the tree - by a given Key
Node* CalendarTree::getNextLevel(Node *curRoot, const treeKey &ID)const
{
	Node *nextLevel = nullptr;
	if (curRoot->checkNumOfChildren() == THREE_CHILD && ID >= curRoot->getmin(3))
		nextLevel = curRoot->getRightChild();
	else if (ID >= curRoot->getmin(2))
		nextLevel = curRoot->getMiddleChild();
	else
		nextLevel = curRoot->getLeftChild();

	return nextLevel;
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

//Print the data sorted --> Traverse the tree until we get the leftmost leaf, and then start printing the nodes by order (they are connected with pointers to each other)
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

//eventAt -  returns an event that happens in a given time - even if it happens during the time and not at the exact time.
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

//eventAfter - returns an event that happens after the given time . In case the given time is an exact time of an event it returns the event itself.
CalendarEvent * CalendarTree::eventAfter(time_t newEventStartTime)
{
	Node* requestedLeaf = Find(newEventStartTime);
	CalendarEvent* requestedEvent = nullptr;

	if ((requestedLeaf != nullptr) && (requestedLeaf->getKey() >= newEventStartTime))
	{
		requestedEvent = requestedLeaf->getData();
	}
	else if ((requestedLeaf != nullptr) && (requestedLeaf->getNextBrother() != nullptr))
	{
		requestedEvent = requestedLeaf->getNextBrother()->getData();
	}

	return requestedEvent;
}

//Deletes the first instance of an event from the tree, and returns the event itself.
CalendarEvent * CalendarTree::deleteFirst()
{
	CalendarEvent* erasedEvent = nullptr;

	if (root != nullptr)
	{
		//in case only root --> delete it
		if (root->isLeaf())
		{
			erasedEvent = root->getData();
			delete root;
			root = nullptr;
		}
		else
		{//traverse the tree in order to find and remove the requested event
			erasedEvent = removeFirstEvent(root, nullptr);
		}
	}
	return erasedEvent;

}

//A recursive method to traverse the tree , find the first event , delete it and update the values of the tree accordingly
CalendarEvent * CalendarTree::removeFirstEvent(Node * node, Node * parent)
{
	CalendarEvent* erasedEvent = nullptr;

	if (node->getLeftChild()->isLeaf()) //we got to the left most leaf - erase it and fix the values above it
	{
		erasedEvent = node->getLeftChild()->getData();
		node->getLeftChild()->getNextBrother()->setPrevBrother(nullptr);
		delete node->getLeftChild();
		node->orderNodeToLeft();
		node->fixMins();
	}
	else
	{	//going down the tree until we get to the left most leaf (the event that we want to remove)
		erasedEvent = removeFirstEvent(node->getLeftChild(), node);
	}
	if (node->getMiddleChild() == nullptr) // in this case - there is an illegal situation of only one son, so we need to organize the whole tree
	{
		node = organizeTree(node, parent);
	}
	node->fixMins();
	return erasedEvent;
}

//Reordering the tree in case we need to
Node* CalendarTree::organizeTree(Node* node, Node* parent)
{
	if (parent == nullptr)
	{ //only root  -> update its left child (last remaining event actualy)
		root = node->getLeftChild();
		delete node;
		node = root;
	}
	else if (parent->getMiddleChild()->getRightChild() != nullptr)
	{ //if we have children under the middle child --> we lift them up one level
		node->setMiddleChild(parent->getMiddleChild()->getLeftChild());
		parent->getMiddleChild()->orderNodeToLeft();
		parent->getMiddleChild()->fixMins();
	}
	else
	{ //ordering the whole node
		parent->getMiddleChild()->orderNodeToRight();
		parent->getMiddleChild()->setLeftChild(node->getLeftChild());
		parent->getMiddleChild()->fixMins();
		delete node;
		parent->orderNodeToLeft();
		parent->fixMins();
		node = parent->getLeftChild();
	}
	return node;
}

//Checking all possible situations of event insertions in order to determine weather the insertion is legal or not.
bool CalendarTree::isInsertLegal(CalendarEvent* eventToInsert)
{
	bool isLegal = false;

	treeKey newEventStartTime = eventToInsert->getStartTime();
	treeKey newEventEndTime = newEventStartTime + eventToInsert->getDuration();

	Node* existEvent = Find(newEventStartTime);
	if (existEvent == nullptr)
	{
		isLegal = true;
	}
	else
	{
		treeKey existEventStartTime = existEvent->getData()->getStartTime();
		treeKey existEventEndTime = existEvent->getData()->getDuration() + existEventStartTime;

		if (newEventStartTime <= existEventStartTime)
		{
			if (newEventEndTime <= existEventStartTime)
			{
				Node* prevExistEvent = existEvent->getPrevBrother();
				if (prevExistEvent == nullptr)
				{
					isLegal = true;
				}
				else
				{
					treeKey prevExistEventStartTime = prevExistEvent->getData()->getStartTime();
					treeKey prevExistEventEndTime = prevExistEvent->getData()->getDuration() + prevExistEventStartTime;

					if (prevExistEventEndTime <= newEventStartTime)
						isLegal = true;
					else
						isLegal = false;
				}
			}
		}
		else
		{
			if (newEventStartTime >= existEventEndTime)
			{
				Node* nextExistEvent = existEvent->getNextBrother();
				if (nextExistEvent == nullptr)
				{
					isLegal = true;
				}
				else
				{
					treeKey nextExistEventStartTime = nextExistEvent->getData()->getStartTime();
					treeKey nextExistEventEndTime = nextExistEvent->getData()->getDuration() + nextExistEventStartTime;

					if (nextExistEventStartTime >= newEventEndTime)
						isLegal = true;
					else
						isLegal = false;
				}
			}
		}
	}
	return isLegal;
}

