#pragma once

#include <map>

struct Child
{
	std::string id;
	bool expectValue;
};

struct FaultNode
{
	std::string name;
	std::string desc;
	std::string formula;
	std::vector<Child> children;

	bool value;
	bool caled;
	bool ignored;

	FaultNode()
		:value(true),
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
	virtual bool calculate(bool check = true, const std::string &id = "1") = 0;
	// 返回JSON
	virtual std::string toJson() = 0;
	// add Node
	virtual bool addNode(const std::string &, FaultNode *) = 0;
	//reset tree
	virtual void reset() = 0;
	//write to json (esrap)
	virtual bool esrap(const std::string &) = 0;
	// 结点深度
	virtual int depth(const std::string &id = "1") = 0;
	// 结点宽度
	virtual int range() const = 0;

	virtual std::vector<std::string> maxDepthBranch(const std::string &id = "1") = 0;

	virtual std::map<int, std::vector<std::string>>& getStructure() = 0;

};

extern "C" IFaultTree *getFaultTree();