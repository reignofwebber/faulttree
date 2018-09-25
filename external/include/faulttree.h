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
	// ����ģʽ
	virtual void setMode(Mode) = 0;
	// �������ӳ�
	virtual void setMainControl(bool) = 0;

	virtual std::map<std::string, FaultNode *> &getTreeModel() = 0;

	// ����Ҷ�ڵ�ID�� ֵ
	virtual bool input(const std::string &, bool) = 0;
	// �����������ļ�·��
	virtual bool parse(const std::string &) = 0;
	// ��ʼ����
	virtual bool calculate(const std::string &id = "1") = 0;

	// �������Ҷ�ӽڵ㶼��ֵ
	virtual bool preCheck() = 0;
	// ������нڵ㶼������
	virtual bool postCheck() = 0;

};

extern "C" IFaultTree *getFaultTree();