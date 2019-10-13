#include "Coroutine.h"
#include<iostream>
#include <vld.h>
#include <thread>
#include "Node.h"

void ProfileSameFringeProblem();
void ProfileGenerator();
void ProfileGeneratorWithoutCoroutine();

int main()
{
	//std::this_thread::sleep_for(std::chrono::seconds(5));

//#define SameFringe
//#define Generator
#define GeneratorWithoutCoro

#ifdef SameFringe
	auto start = std::chrono::high_resolution_clock::now();
	ProfileSameFringeProblem();
	auto end = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;
#endif // SameFringe

#ifdef Generator
	auto start = std::chrono::high_resolution_clock::now();
	ProfileGenerator();
	auto end = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
#endif // Generator

#ifdef GeneratorWithoutCoro
	auto start = std::chrono::high_resolution_clock::now();
	ProfileGeneratorWithoutCoroutine();
	auto end = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
#endif // GeneratorWithoutCoro


	std::cin.get();
	return 0;
}

template<typename T>
void TraverseTree(const Node* pNode, Caller<T>& c)
{
	//Traverse Left
	if (pNode->pLeft)
		TraverseTree(pNode->pLeft, c);

	//Traverse Right
	if (pNode->pRight)
		TraverseTree(pNode->pRight, c);

	//Only yield when the reached node is a leaf!
	if (!pNode->pLeft && !pNode->pRight)
		c(pNode->value);
}

void ProfileSameFringeProblem()
{
	//Create Trees
	Node *pLeftTree{ CreateLeftree(4) };
	Node *pRightTree{ CreateRightTree(2) };

	bool isSameFringe{ true };

	//Start Coroutine for left tree
	Coroutine<int> coro_left_tree{ [pLeftTree](Caller<int>& c)
	{
		TraverseTree(pLeftTree, c);
	}};

	//Start Coroutine for right tree
	Coroutine<int> coro_right_tree{ [pRightTree](Caller<int>& c)
	{
		TraverseTree(pRightTree, c);
	}};

	//Loop Until Same Fringe is found
	while (true)
	{
		int leftTreeValue{ coro_left_tree.GetValue()};
		int rightTreeValue{ coro_right_tree.GetValue() };

		//If fringe difference is found, jump out
		if (leftTreeValue != rightTreeValue)
		{
			isSameFringe = false;
			//std::cout << "Fringe difference found!" << std::endl;
			break;
		}

		//if no fringe difference is found and coroutines are done searching the whole fringe, jump out
		//or else, resuming the coroutine will crash the program
		if (!coro_right_tree.IsValid() || !coro_left_tree.IsValid())
			break;

		//if searching still valid, resume the coroutines
		coro_left_tree.Resume();
		coro_right_tree.Resume();
	}

	//Destroy Nodes
	SAFE_DELETE (pLeftTree);
	SAFE_DELETE (pRightTree);
}

void ProfileGenerator()
{
	//Start Coroutine for left tree
	Coroutine<int> generator{ [](Caller<int>& c)
	{
		const unsigned int size{1000000};
		for (unsigned int i{ 0 }; i < size; ++i)
		{
			c(i);
		}
	} };

	//loop through the coroutine
	int sum{};
	while (generator.IsValid())
	{
		generator.Resume();
		sum += generator.GetValue();
	}
}

void ProfileGeneratorWithoutCoroutine()
{
	int value{};
	int maxValue{ 1000000 };

	auto generateValue = [&value, maxValue]()
	{
		for (int i{}; i <= maxValue; ++i)
		{
			value = i;
		}
	};

	//loop through the coroutine
	int sum{};
	while (value < maxValue)
	{
		generateValue();
		sum += value;
	}
}