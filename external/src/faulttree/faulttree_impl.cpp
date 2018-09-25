// faulttree.cpp : Defines the entry point for the application.
//

#include "faulttree_impl.h"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

using namespace rapidjson;

FaultTree::FaultTree()
	: m_mode(ATB)
	, m_mainControl(main_1)
{
}


void FaultTree::setMode(Mode mode)
{
	m_mode = mode;
}

void FaultTree::setMainControl(bool main)
{
	m_mainControl = main;
}

bool FaultTree::parse(const std::string &fileName)
{
	Document document;
	std::ifstream config(fileName, std::ios::in);
	if (!config.is_open())
	{
		std::cout << "Can not Open " << fileName << std::endl;
		return false;
	}
	IStreamWrapper isw(config);

	if (document.ParseStream(isw).HasParseError())
	{
		std::cout << "Json File has ERROR!!" << std::endl;
		return false;
	}


	assert(document.IsArray());

	for (Value::ConstValueIterator itr = document.Begin();
		itr != document.End(); ++itr)
	{
		FaultNode *node = new FaultNode;
		auto object = itr->GetObject();
		std::string id(object["id"].GetString());

		std::string formula;
		std::vector<std::string> children;
		std::vector<bool> expectValue;
		
		if (object.HasMember("formula"))
		{
			node->formula = getFormula(object["formula"]);
		}
		if (object.HasMember("children"))
		{
			parseChildren(object["children"], node->children, node->expectValue);
		}

		if (m_faultTree.find(id) != m_faultTree.end())
		{
			std::cout << "duplicate key ..." << id << "\n";
		}
		m_faultTree[id] = node;

		
	}
	return true;
}

bool FaultTree::input(const std::string &id, bool value)
{
	if (m_faultTree.find(id) == m_faultTree.end()) return false;
	m_faultTree[id]->caled = true;
	m_faultTree[id]->value = value;
	return true;
}

bool FaultTree::preCheck()
{
	for (auto itr = m_faultTree.begin(); itr != m_faultTree.end(); ++itr)
	{
		auto id = itr->first;
		if (id.find("DT") != std::string::npos)
		{
			if (itr->second->caled == false)
			{
				std::cout << "Not import DATA ..." << id << "\n";
				return false;
			}
		}
	}
	return true;
}

bool FaultTree::postCheck()
{
	for (auto itr = m_faultTree.begin(); itr != m_faultTree.end(); ++itr)
	{
		
		if (itr->second->caled == false)
		{
			if (itr->first.find("comment") == std::string::npos)
			{
				std::cout << "DATA ..." << itr->first << "... Not be calculated." << "\n";
				return false;
			}
		}
	}
	return true;
}

bool FaultTree::calculate(const std::string &id)
{
	auto node = getNode(id);
	if (node == nullptr) return false;
	calculate(node);
}

bool FaultTree::calculate(FaultNode *node)
{
	if (node->caled) return true;

	for (const auto& child : node->children)
	{
		auto childNode = getNode(child);
		if (childNode != nullptr && !childNode->caled) calculate(childNode);
	}

	//需要推导的默认为true
	assert(node->children.size() == node->expectValue.size());

	if (node->formula == "all")
	{
		auto itr_id = node->children.begin();
		auto itr_expect = node->expectValue.begin();

		for (; itr_id != node->children.end(); ++itr_id, ++itr_expect)
		{
			auto childNode = getNode(*itr_id);
			if (childNode == nullptr) continue;
			assert(childNode->caled);

			if (childNode->value != *itr_expect)
			{
				break;
			}
		}
		if (itr_id != node->children.end())
		{
			node->value = false;
		}
		else
		{
			node->value = true;
		}

		node->caled = true;
	}
	else if (node->formula == "least")
	{
		auto itr_id = node->children.begin();
		auto itr_expect = node->expectValue.begin();

		for (; itr_id != node->children.end(); ++itr_id, ++itr_expect)
		{
			auto childNode = getNode(*itr_id);
			if (childNode == nullptr) continue;
			assert(childNode->caled);

			if (childNode->value == *itr_expect)
			{
				break;
			}
		}

		if (itr_id != node->children.end())
		{
			node->value = true;
		}
		else
		{
			node->value = false;
		}

		node->caled = true;

	}
	else if (node->formula == "only")
	{
		auto itr_id = node->children.begin();
		auto itr_expect = node->expectValue.begin();

		auto cnt = 0;
		for (; itr_id != node->children.end(); ++itr_id, ++itr_expect)
		{
			auto childNode = getNode(*itr_id);
			if (childNode == nullptr) continue;
			assert(childNode->caled);

			if (childNode->value == *itr_expect)
			{
				++cnt;
			}
		}

		if (cnt == 1)
		{
			node->value = true;
		}
		else
		{
			node->value = false;
		}

		node->caled = true;

	}
	else {
		std::cout << "Unsupport operation ..." << node->formula << "\n";
		assert(false);
		return false;
	}
	return true;
}

std::map<std::string, FaultNode *> &FaultTree::getTreeModel()
{
	return m_faultTree;
}


std::string FaultTree::getFormula(const rapidjson::Value &val)
{
	if (val.IsString())
	{
		return val.GetString();
	}
	else if (val.IsObject())
	{
		Value::ConstMemberIterator itr;

		switch (m_mode)
		{
		case ATB:
			itr = val.FindMember("3");
			return itr->value.GetString();
		default:
			itr = val.FindMember("else");
			return itr->value.GetString();
			break;
		}
	}
	else 
	{
		assert(false);
	}
	return "";
}

void FaultTree::parseChildren(const rapidjson::Value &val, std::vector<std::string> &children, std::vector<bool> &expectValue)
{
	if (val.IsArray())
	{
		for (Value::ConstValueIterator itr = val.Begin();
			itr != val.End(); ++itr)
		{
			std::string id;
			bool expect;

			if (itr->IsString())
			{
				splitIdAndExpect(itr->GetString(), id, expect);
			}
			else if(itr->IsArray())
			{
				auto selectControl = itr->GetArray();

				if (m_mainControl == main_1)
				{
					splitIdAndExpect(selectControl[0].GetString(), id, expect);
				}
				else if (m_mainControl == main_6)
				{
					splitIdAndExpect(selectControl[1].GetString(), id, expect);
				}

			}
			else
			{
				assert(false);
			}
			children.push_back(id);
			expectValue.push_back(expect);
		}
	}
	else if (val.IsObject())
	{
		Value::ConstMemberIterator itr;

		switch (m_mode)
		{
		case FaultTree::ATB:
			itr = val.FindMember("3");
			assert(itr->value.IsArray());
			parseChildren(itr->value, children, expectValue);
			break;
		default:
			itr = val.FindMember("else");
			assert(itr->value.IsArray());
			parseChildren(itr->value, children, expectValue);
			break;
		}
	}
	else
	{
		assert(false);
	}
}


void FaultTree::splitIdAndExpect(const std::string &raw, std::string &id, bool &expect)
{
	size_t id_end = raw.find(":");
	if (id_end == std::string::npos)
	{
		id = raw;
		expect = true;
	}
	else
	{
		id = raw.substr(0, id_end);
		std::istringstream(raw.substr(id_end + 1)) >> expect;
	}
}

FaultNode *FaultTree::getNode(const std::string &id)
{

	if (m_faultTree.find(id) == m_faultTree.end())
	{
		if (id.find("NONE") != std::string::npos)
		{
			return nullptr;
		}
		std::cout << "KEYERROR cannot find key..." + id;
        return 0;
	}
	return m_faultTree[id];
}


FaultTree::~FaultTree()
{
}


extern "C" IFaultTree *getFaultTree()
{
	return new FaultTree();
}
