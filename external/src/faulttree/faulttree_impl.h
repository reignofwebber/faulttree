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
	bool calculate(const std::string &id = "1");

	bool calculate(FaultNode *);

	std::map<std::string, FaultNode *> &getTreeModel();

private:
	std::string getFormula(const rapidjson::Value &);
	void parseChildren(const rapidjson::Value &, std::vector<std::string> &, std::vector<bool> &);

	void splitIdAndExpect(const std::string &, std::string &, bool &);

	FaultNode *getNode(const std::string &);
private:
	std::map<std::string, FaultNode *> m_faultTree;
	Mode m_mode;
	bool m_mainControl;
};

