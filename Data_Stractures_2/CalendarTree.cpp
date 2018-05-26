#include "CalendarTree.h"

//will free the tree
CalendarTree::~CalendarTree()
{
	freeTree(m_Root);
	m_Root = nullptr;
}

//goes from treeNode to treeNode and deletes it
void CalendarTree::freeTree(Node* i_RootToDelete)
{
	if (i_RootToDelete == nullptr)
		return;
	else if (i_RootToDelete->isLeaf())
		delete i_RootToDelete;
	else
	{
		freeTree(i_RootToDelete->getLeftChild());
		freeTree(i_RootToDelete->getMiddleChild());
		freeTree(i_RootToDelete->getRightChild());
	}
}

CalendarEvent* CalendarTree::insert(CalendarEvent* i_EventToInsert)
{
	Node* nodeToInsert = nullptr, *sibling = nullptr, *newRoot = nullptr;
	CalendarEvent* returnedEvent = nullptr;

	if (isInsertLegal(i_EventToInsert))
	{
		nodeToInsert = new Node();
		nodeToInsert->setLeaf(i_EventToInsert);

		//The tree is empty
		if (m_Root == nullptr)
		{
			m_Root = nodeToInsert;
		}
		//There is already one leaf
		else if (m_Root->isLeaf())
		{
			//check in which order to insert the nodes
			if (m_Root->getKey() < nodeToInsert->getKey())
			{
				newRoot = new Node(m_Root, nodeToInsert);
				m_Root->setNextBrother(nodeToInsert);
				nodeToInsert->setPrevBrother(m_Root);
			}
			else
			{
				newRoot = new Node(nodeToInsert, m_Root);
				nodeToInsert->setNextBrother(m_Root);
				m_Root->setPrevBrother(nodeToInsert);
			}
			//make sure the node values are set properly
			newRoot->fixMins();
			m_Root = newRoot;
		}
		else
		{
			//in this case - the root isn't a leaf so we go the recusive function in order to insert currectly
			insertHelper(&sibling, m_Root, nodeToInsert, nullptr);
		}

		returnedEvent = i_EventToInsert;
	}

	return returnedEvent;
}

/*Recursive function to insert the the node to its right place in the CalendarTree
  THE IDEA: Traverse the tree until we get the place in which we need to insert the new node.
			We traverse the tree one level at a time until we get to the requested level.
			At the requested level - if it has 2 children, we can insert our node safely and finish.
			Otherwise - we use a division method in order to split the place in which we want to insert to 2 parts.
			In case we did that, we need to connect the new node to the tree in the if statement starting in line 96.*/
void CalendarTree::insertHelper(Node** i_Sibling, Node* i_CurrRoot, Node* i_EventToInsert, Node* i_Parent)
{
	Node *temp = nullptr;
	Node *tempParent = nullptr;
	Node *newRoot = nullptr;

	if (i_CurrRoot->getLeftChild()->isLeaf())
	{
		if (i_CurrRoot->checkNumOfChildren() == TWO_CHILD)
		{
			i_CurrRoot->insertToTwoChildNode(i_EventToInsert);
		}
		else//no sapce - divide the node into 2 parts in order to insert the new node
		{
			(*i_Sibling) = i_CurrRoot->divideNode(i_EventToInsert);
		}
	}
	else
	{
		tempParent = getNextLevel(i_CurrRoot, i_EventToInsert->getKey());
		insertHelper(i_Sibling, tempParent, i_EventToInsert, i_CurrRoot);
	}

	if (*i_Sibling != nullptr) //in case we created a new node at divideNode method --> we must connect it to the tree
	{
		if (i_Parent == nullptr)
		{//connecting the 2 new nodes to a new parent
			newRoot = new Node((*i_Sibling), i_CurrRoot);
			m_Root = newRoot;
		}
		else
		{//adding the 1 new node to an existing parent
			i_Parent->fixMins();
			if (i_Parent->checkNumOfChildren() == TWO_CHILD)
			{
				i_Parent->insertToTwoChildNode(*i_Sibling);
				*i_Sibling = nullptr;
			}
			else
			{
				*i_Sibling = i_Parent->divideNode(*i_Sibling);
			}

		}
	}

	i_CurrRoot->fixMins(); //make sure min values are set properly before leaving the method
}

//Going down one level in the tree - by a given Key
Node* CalendarTree::getNextLevel(Node* i_CurrRoot, treeKey i_Key)
{
	Node *nextLevel = nullptr;

	if ((i_CurrRoot->checkNumOfChildren() == THREE_CHILD) && (i_Key >= i_CurrRoot->getMin(THREE_CHILD)))
	{
		nextLevel = i_CurrRoot->getRightChild();
	}
	else if (i_Key >= i_CurrRoot->getMin(TWO_CHILD))
	{
		nextLevel = i_CurrRoot->getMiddleChild();
	}
	else
	{
		nextLevel = i_CurrRoot->getLeftChild();
	}

	return nextLevel;
}

//Recursive function to delete a node
void CalendarTree::deleteHelper(Node* i_CurrParent, Node* i_CurrChild)
{
	int whichChild, num;
	Node* temp;

	if (i_CurrParent->checkNumOfChildren() == THREE_CHILD)
		i_CurrParent->deleteNodeFromThree(i_CurrChild);
	else
	{
		i_CurrParent->deleteNodeFromTwo(i_CurrChild);
		whichChild = i_CurrParent->findChildPlace();
		switch (whichChild)
		{
		case LEFT:
			temp = i_CurrParent->getParnet()->getMiddleChild();
			if (temp->checkNumOfChildren() == THREE_CHILD)
			{
				i_CurrParent->setMiddleChild(temp->getLeftChild());
				temp->setLeftChild(temp->getMiddleChild());
				temp->setMiddleChild(temp->getRightChild());
				temp->setRightChild(nullptr);
				i_CurrParent->setMin2(temp->getMin(1));
				temp->setMin1(temp->getMin(2));
				temp->setMin2(temp->getMin(3));
				temp->setMin3(-1);
			}
			else
			{
				temp->setRightChild(temp->getMiddleChild());
				temp->setMiddleChild(temp->getLeftChild());
				temp->setLeftChild(i_CurrParent->getLeftChild());
				temp->setMin1(i_CurrParent->getMin(1));
				num = temp->getMin(2);
				temp->setMin2(temp->getMin(3));
				temp->setMin3(num);
				deleteHelper(i_CurrParent->getParnet(), i_CurrParent);
			}
			break;

		case MIDDLE:
			temp = i_CurrParent->getParnet()->getLeftChild();
			if (temp->checkNumOfChildren() == THREE_CHILD)
			{
				i_CurrParent->setMiddleChild(i_CurrParent->getLeftChild());
				i_CurrParent->setLeftChild(temp->getRightChild());
				temp->setRightChild(nullptr);
				i_CurrParent->setMin2(i_CurrParent->getMin(1));
				i_CurrParent->setMin1(temp->getMin(3));
				temp->setMin3(-1);
			}
			else
			{
				temp->setRightChild(i_CurrParent->getLeftChild());
				temp->setMin3(i_CurrParent->getMin(1));
				i_CurrParent->setLeftChild(nullptr);
				i_CurrParent->setMin1(-1);
				deleteHelper(i_CurrParent->getParnet(), i_CurrParent);
			}
			break;

		case RIGHT:
			temp = i_CurrParent->getParnet()->getMiddleChild();
			if (temp->checkNumOfChildren() == THREE_CHILD)
			{
				i_CurrParent->setMiddleChild(i_CurrParent->getLeftChild());
				i_CurrParent->setLeftChild(temp->getRightChild());
				temp->setRightChild(nullptr);
				i_CurrParent->setMin2(i_CurrParent->getMin(1));
				i_CurrParent->setMin1(temp->getMin(3));
				temp->setMin3(-1);
			}
			else
			{
				temp->setRightChild(i_CurrParent->getLeftChild());
				temp->setMin3(i_CurrParent->getMin(1));
				i_CurrParent->setLeftChild(nullptr);
				i_CurrParent->setMin1(-1);
				deleteHelper(i_CurrParent->getParnet(), i_CurrParent);
			}
			break;

		case ROOT:
			temp = i_CurrParent->getLeftChild();
			delete m_Root;
			m_Root = temp;
			m_Root->setParent(nullptr);
			fixKeys(temp);
			break;
		}
	}
}

void CalendarTree::fixKeys(Node* i_EventNode)
{
	if (i_EventNode == NULL)
	{
		return;
	}
	else
	{
		i_EventNode->fixMins();
		fixKeys(i_EventNode->getParnet());
	}
}

//Gets a key from the input and finds the node that holds it
Node* CalendarTree::findEvent(treeKey i_TimeToFind)
{
	Node* curr;

	curr = m_Root;
	if (curr == nullptr)
	{
		return nullptr;
	}

	while (curr != nullptr)
	{
		if (curr->isLeaf())
		{
			return curr;
		}
		else
		{
			if ((curr->checkNumOfChildren() == THREE_CHILD) && (i_TimeToFind >= curr->getMin(3)))
			{
				curr = curr->getRightChild();
			}
			else if (i_TimeToFind >= curr->getMin(2))
			{
				curr = curr->getMiddleChild();
			}
			else
			{
				curr = curr->getLeftChild();
			}
		}
	}

	return nullptr;
}

//Print the data sorted --> Traverse the tree until we get the leftmost leaf, and then start printing the nodes by order (they are connected with pointers to each other)
void CalendarTree::printSorted()
{
	Node* curr = m_Root;

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
CalendarEvent* CalendarTree::eventAt(time_t i_TimeToFind)
{
	Node* requestedLeaf = findEvent(i_TimeToFind);
	CalendarEvent* requestedEvent = nullptr;

	if ((requestedLeaf != nullptr) && (requestedLeaf->getData()->isEventStillHappening(i_TimeToFind)))
	{
		requestedEvent = requestedLeaf->getData();
	}

	return requestedEvent;
}

//eventAfter - returns an event that happens after the given time . In case the given time is an exact time of an event it returns the event itself.
CalendarEvent* CalendarTree::eventAfter(time_t i_TimeToFind)
{
	Node* requestedLeaf = findEvent(i_TimeToFind);
	CalendarEvent* requestedEvent = nullptr;

	if ((requestedLeaf != nullptr) && (requestedLeaf->getKey() >= i_TimeToFind))
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
CalendarEvent* CalendarTree::deleteFirst()
{
	CalendarEvent* erasedEvent = nullptr;

	if (m_Root != nullptr)
	{
		//in case only root --> delete it
		if (m_Root->isLeaf())
		{
			erasedEvent = m_Root->getData();
			delete m_Root;
			m_Root = nullptr;
		}
		else
		{//traverse the tree in order to find and remove the requested event
			erasedEvent = removeFirstEvent(m_Root, nullptr);
		}
	}

	return erasedEvent;
}

//A recursive method to traverse the tree , find the first event , delete it and update the values of the tree accordingly
CalendarEvent* CalendarTree::removeFirstEvent(Node* i_EventNode, Node* i_Parent)
{
	CalendarEvent* erasedEvent = nullptr;

	if (i_EventNode->getLeftChild()->isLeaf()) //we got to the left most leaf - erase it and fix the values above it
	{
		erasedEvent = i_EventNode->getLeftChild()->getData();
		i_EventNode->getLeftChild()->getNextBrother()->setPrevBrother(nullptr);
		delete i_EventNode->getLeftChild();
		i_EventNode->shiftNodeToLeft();
		i_EventNode->fixMins();
	}
	else
	{	//going down the tree until we get to the left most leaf (the event that we want to remove)
		erasedEvent = removeFirstEvent(i_EventNode->getLeftChild(), i_EventNode);
	}
	if (i_EventNode->getMiddleChild() == nullptr) // in this case - there is an illegal situation of only one son, so we need to organize the whole tree
	{
		i_EventNode = organizeTree(i_EventNode, i_Parent);
	}

	i_EventNode->fixMins();
	return erasedEvent;
}

//Reordering the tree in case we need to
Node* CalendarTree::organizeTree(Node* i_EventNode, Node* i_Parent)
{
	if (i_Parent == nullptr)
	{ //only root  -> update its left child (last remaining event actualy)
		m_Root = i_EventNode->getLeftChild();
		delete i_EventNode;
		i_EventNode= m_Root;
	}
	else if (i_Parent->getMiddleChild()->getRightChild() != nullptr)
	{ //if we have children under the middle child --> we lift them up one level
		i_EventNode->setMiddleChild(i_Parent->getMiddleChild()->getLeftChild());
		i_Parent->getMiddleChild()->shiftNodeToLeft();
		i_Parent->getMiddleChild()->fixMins();
	}
	else
	{ //ordering the whole node
		i_Parent->getMiddleChild()->shiftNodeToRight();
		i_Parent->getMiddleChild()->setLeftChild(i_EventNode->getLeftChild());
		i_Parent->getMiddleChild()->fixMins();
		delete i_EventNode;
		i_Parent->shiftNodeToLeft();
		i_Parent->fixMins();
		i_EventNode = i_Parent->getLeftChild();
	}

	return i_EventNode;
}

//Checking all possible situations of event insertions in order to determine weather the insertion is legal or not.
bool CalendarTree::isInsertLegal(CalendarEvent* i_EventToInsert)
{
	bool isLegal = false;

	treeKey newEventStartTime = i_EventToInsert->getStartTime();
	treeKey newEventEndTime = newEventStartTime + i_EventToInsert->getDuration();

	Node* existEvent = findEvent(newEventStartTime);
	if (existEvent == nullptr)
	{
		isLegal = true;
	}
	else
	{
		treeKey existEventStartTime = existEvent->getData()->getStartTime();
		treeKey existEventEndTime = existEvent->getData()->getDuration() + existEventStartTime;

		//if we are below
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
		{//if we are above
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

