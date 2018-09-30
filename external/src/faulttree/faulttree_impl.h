// faulttree.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>


#include "faulttree.h"





class FaultTree : public IFaultTree
{

public:
	

	//主控端
	const bool main_1 = 0;
	const bool main_6 = 1;

public:
	FaultTree();
	~FaultTree();

	// presettings
	void setMode(Mode);
	// 主控 0--1车 1--6车
	void setMainControl(bool);

	bool parse(const std::string &);
	bool input(const std::string &, bool);
	//检查所有叶子节点都有值
	bool preCheck();
	//检查所有节点都被计算
	bool postCheck();
	bool calculate(bool check = true, const std::string &id = "1");

	std::string toJson();

	void reset();

	bool esrap(const std::string &);

	bool addNode(const std::string &, FaultNode *);

	bool calculate(FaultNode *);

	std::map<std::string, FaultNode *> &getTreeModel();

	int depth(const std::string &id = "1");

	int range() const;

	std::vector<std::string> maxDepthBranch(const std::string &id = "1");

	std::map<int, std::vector<std::string>> &getStructure();

private:
	std::string getFormula(const rapidjson::Value &);
	void parseChildren(const rapidjson::Value &, std::vector<Child> &child);

	void splitIdAndExpect(const std::string &, std::string &, bool &);

	FaultNode *getNode(const std::string &);

	bool parseStructure(const std::string & id = "1", int level = 0);

	bool toJsonAux(rapidjson::Writer<rapidjson::StringBuffer> &writer, const std::string &id);
	
private:
	std::map<std::string, FaultNode *> m_faultTree;
	Mode m_mode;
	bool m_mainControl;

	std::vector<std::string> m_maxBranch;
	// index --> level  value --> column
	std::map<int, std::vector<std::string>> m_structure;
};

