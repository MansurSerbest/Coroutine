#pragma once
#define SAFE_DELETE(a) if(a) delete a

struct Node
{
	Node *pLeft, *pRight;
	int value;

	//Create Leaf
	Node(int v)
		:pLeft{ nullptr }, value{ v }, pRight{ nullptr }
	{

	}

	//Create Branch
	Node(Node *pL, int v, Node *pR)
		:pLeft{ pL }, value{ v }, pRight{ pR }
	{

	}

	~Node()
	{
		SAFE_DELETE(pLeft);
		SAFE_DELETE(pRight);
	}

	static Node* Create(const int v)
	{
		return new Node{ v };
	}

	static Node* Create(Node* pL, const int v, Node* pR)
	{
		return new Node{ pL, v, pR };
	}
};

Node* CreateLeftree(int value);

Node* CreateRightTree(int value);

void PrintFringes(const Node* pNode);
