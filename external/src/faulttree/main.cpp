#include <iostream>
#include "faulttree_impl.h"
#include "test.h"


using namespace rapidjson;

int main()
{
	FaultTree tree;
	tree.parse("config.json");

	pushInput(tree);

	tree.preCheck();
	tree.calculate();
	tree.postCheck();
	
	getchar();

	return 0;
}