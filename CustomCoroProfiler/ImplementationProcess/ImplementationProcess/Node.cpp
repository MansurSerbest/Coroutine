#include "Node.h"
#include <iostream>

Node* CreateLeftree(int value)
{
	//return the root node
	return Node::Create(Node::Create(Node::Create(1), 2, Node::Create(3)), 4, Node::Create(5));
}

Node* CreateRightTree(int value)
{
	return Node::Create(Node::Create(1), 2, Node::Create(Node::Create(3), 4, Node::Create(5)));
}

void PrintFringes(const Node* pNode)
{
	//Traverse Left
	if (pNode->pLeft)
		PrintFringes(pNode->pLeft);

	//Traverse Right
	if (pNode->pRight)
		PrintFringes(pNode->pRight);

	//Only print when the reached node is a leaf! (no left or right branches/leaves)
	if (!pNode->pLeft && !pNode->pRight)
		std::cout << pNode->value << " ";
}