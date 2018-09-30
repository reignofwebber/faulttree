#include <iostream>
#include "faulttree_impl.h"
#include "test.h"

#include <locale>

using namespace rapidjson;

int main()
{
	//std::ifstream in("test.txt");

	//char buf[1000];
	//in.getline(buf, 1000);
	//std::cout << buf;

	FaultTree tree;
	tree.parse("config.json");
	pushInput(tree);
	tree.calculate();
	std::cout << "success";
    std::cout <<  tree.toJson();


	//pushInput(tree);

	//tree.preCheck();
	//tree.calculate();
	//tree.postCheck();
	
	getchar();

	return 0;
}