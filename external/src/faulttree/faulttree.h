#pragma once

#include <map>

struct FaultNode
{
	std::string formula;
	std::vector<std::string> children;
	std::vector<bool> expectValue;
	bool value;
	bool caled;
	bool ignored;

	FaultNode()
		:value(false),
		caled(false),
		ignored(false)
	{

	}
};


class IFaultTree
{
public:
	enum Mode
	{
		Emegency = 1,
		ATO = 2,
		ATB = 3,
		ATP = 4,
		RMR = 5,
		RMF = 6,
		Zero = 7,
		Clean = 8,
		Auto = 9
	};
public:
	// 设置模式
	virtual void setMode(Mode) = 0;
	// 设置主从车
	virtual void setMainControl(bool) = 0;

	virtual std::map<std::string, FaultNode *> &getTreeModel() = 0;

	// 输入叶节点ID， 值
	virtual bool input(const std::string &, bool) = 0;
	// 解析的配置文件路径
	virtual bool parse(const std::string &) = 0;
	// 开始计算
	virtual bool calculate(const std::string &id = "1") = 0;

	// 检查所有叶子节点都有值
	virtual bool preCheck() = 0;
	// 检查所有节点都被计算
	virtual bool postCheck() = 0;

};

extern "C" IFaultTree *getFaultTree();