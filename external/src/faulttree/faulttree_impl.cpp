// faulttree.cpp : Defines the entry point for the application.
//

#include "faulttree_impl.h"


#include <string>

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
	//std::string line, jsonText;
	//while (config >> line)
	//{
	//	jsonText.append(line);
	//}
	IStreamWrapper isw(config);

	if (document.ParseStream(isw).HasParseError())
	{
		std::cout << "Json File has ERROR!!" << std::endl;
		return false;
	}

	//if (document.Parse(jsonText.c_str()).HasParseError())
	//{
	//	std::cout << "Json File has ERROR!!" << std::endl;
	//	return false;
	//}

	assert(document.IsArray());

	for (Value::ConstValueIterator itr = document.Begin();
		itr != document.End(); ++itr)
	{
		FaultNode *node = new FaultNode;
		auto object = itr->GetObject();
		std::string id(object["id"].GetString());
		bool a = id == "101"? true : false;
		std::string formula;

		if (object.HasMember("name"))
		{
			node->name = object["name"].GetString();
		}

		if (object.HasMember("desc"))
		{
			node->desc = object["desc"].GetString();
		}
		
		if (object.HasMember("formula"))
		{
			node->formula = getFormula(object["formula"]);
		}
		if (object.HasMember("children"))
		{
			parseChildren(object["children"], node->children);
		}

		if (m_faultTree.find(id) != m_faultTree.end())
		{
			std::cout << "duplicate key ..." << id << "\n";
		}
		m_faultTree[id] = node;

		
	}
	parseStructure();
	return true;
}

bool FaultTree::parseStructure(const std::string &id, int level)
{
	m_structure[level].push_back(id);
	auto node = getNode(id);
	if (node == nullptr) return true;

	for (const auto &child : node->children)
	{
		parseStructure(child.id, level + 1);
	}
}

bool FaultTree::input(const std::string &id, bool value)
{
	if (m_faultTree.find(id) == m_faultTree.end()) return false;
	m_faultTree[id]->caled = true;
	m_faultTree[id]->value = value;
	return true;
}

void FaultTree::reset()
{
	for (auto itr = m_faultTree.begin();
		itr != m_faultTree.end(); ++itr)
	{
		FaultNode *node = itr->second;
		node->caled = false;
	}
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

bool FaultTree::calculate(bool check, const std::string &id)
{
	if (check && !preCheck()) return false;
	auto node = getNode(id);
	if (node == nullptr) return false;
	if(!calculate(node)) return false;
	if (check && !postCheck()) return false;
	return true;
}

bool FaultTree::calculate(FaultNode *node)
{
	if (node->caled) return true;

	for (const auto& child : node->children)
	{
		auto childNode = getNode(child.id);
		if (childNode != nullptr && !childNode->caled) calculate(childNode);
	}

	//需要推导的默认为true
//	assert(node->children.size() == node->expectValue.size());

	if (node->formula == "all")
	{
		auto itr = node->children.begin();

		for (; itr != node->children.end(); ++itr)
		{
			auto childNode = getNode(itr->id);
			if (childNode == nullptr) continue;
			assert(childNode->caled);

			if (childNode->value != itr->expectValue)
			{
				break;
			}
		}
		if (itr != node->children.end())
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
		auto itr = node->children.begin();

		for (; itr != node->children.end(); ++itr)
		{
			auto childNode = getNode(itr->id);
			if (childNode == nullptr) continue;
			assert(childNode->caled);

			if (childNode->value == itr->expectValue)
			{
				break;
			}
		}

		if (itr != node->children.end())
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
		auto itr = node->children.begin();

		auto cnt = 0;
		for (; itr != node->children.end(); ++itr)
		{
			auto childNode = getNode(itr->id);
			if (childNode == nullptr) continue;
			assert(childNode->caled);

			if (childNode->value == itr->expectValue)
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

void FaultTree::parseChildren(const rapidjson::Value &val, std::vector<Child> &children)
{
	if (val.IsArray())
	{
		for (Value::ConstValueIterator itr = val.Begin();
			itr != val.End(); ++itr)
		{
			Child child;
			
			if (itr->IsString())
			{
				splitIdAndExpect(itr->GetString(), child.id, child.expectValue);
			}
			else if(itr->IsArray())
			{
				auto selectControl = itr->GetArray();

				if (m_mainControl == main_1)
				{
					splitIdAndExpect(selectControl[0].GetString(), child.id, child.expectValue);
				}
				else if (m_mainControl == main_6)
				{
					splitIdAndExpect(selectControl[1].GetString(), child.id, child.expectValue);
				}

			}
			else
			{
				assert(false);
			}
			children.push_back(child);
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
			parseChildren(itr->value, children);
			break;
		default:
			itr = val.FindMember("else");
			assert(itr->value.IsArray());
			parseChildren(itr->value, children);
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

int FaultTree::depth(const std::string &id)
{
	auto node = getNode(id);
	if (node == nullptr) return 1;
	
	int maxChildDepth = 0;

	for (const auto &child : node->children)
	{
		int childDepth = depth(child.id);
		if (maxChildDepth < childDepth)
		{
			maxChildDepth = childDepth;
		}
	}
	return maxChildDepth + 1;
}

int FaultTree::range() const
{
	int range = 0;
	for (auto itr = m_faultTree.begin(); itr != m_faultTree.end(); ++itr)
	{
		auto id = itr->first;
		if (id.find("DT") != std::string::npos)
		{
			++range;
		}
	}
	return range;
}

std::vector<std::string> FaultTree::maxDepthBranch(const std::string &id)
{
	std::vector<std::string> maxBranch;
	auto node = getNode(id);
	if (node == nullptr) return maxBranch;

	for (const auto &child : node->children)
	{
		auto branch = maxDepthBranch(child.id);
		if (maxBranch.size() < branch.size())
		{
			maxBranch = branch;
		}
	}
	maxBranch.push_back(id);

	return maxBranch;
}

std::string FaultTree::toJson()
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	toJsonAux(writer, "1");
	
	return std::string(s.GetString());
}

bool FaultTree::toJsonAux(rapidjson::Writer<StringBuffer> &writer, const std::string &id)
{
	if (m_faultTree.find(id) == m_faultTree.end())
	{
		std::cout << "TO JSON ERROR , CAN NOT FIND id " << id << std::endl;
		return false;
	}
	FaultNode *node = m_faultTree[id];
	
	writer.StartObject();

	writer.Key("id");
	writer.String(id.c_str());
	writer.Key("name");
	writer.String(node->name.c_str());
	writer.Key("value");
	writer.Bool(node->value);
	writer.Key("formula");
	writer.String(node->formula.c_str());
	writer.Key("desc");
	writer.String(node->desc.c_str());
	
	writer.Key("children");
	writer.StartArray();
	for (const auto &child : node->children)
	{
		toJsonAux(writer, child.id);
	}
	writer.EndArray();

	writer.EndObject();
	return true;
}

std::map<int, std::vector<std::string>> &FaultTree::getStructure()
{
	return m_structure;
}

bool FaultTree::addNode(const std::string &id, FaultNode *node)
{
	m_faultTree[id] = node;
}

bool FaultTree::esrap(const std::string &file)
{
	StringBuffer s;
	PrettyWriter<StringBuffer> writer(s);

	writer.StartArray();
	
	for (auto itr = m_faultTree.begin();
		itr != m_faultTree.end(); ++itr)
	{
		writer.StartObject();
		
		auto id = itr->first;
		auto node = itr->second;

		writer.Key("id");
		writer.String(id.c_str());
		writer.Key("name");
		writer.String(node->name.c_str());
		writer.Key("desc");
		writer.String(node->desc.c_str());
		writer.Key("formula");
		writer.String(node->formula.c_str());
		writer.Key("children");

		writer.StartArray();
		for (const auto &child : node->children)
		{
			std::string tmp = child.id;
			tmp += ":";
			tmp += std::to_string(child.expectValue);
			writer.Key(tmp.c_str());
		}
		writer.EndArray();

		writer.EndObject();
	}
	writer.EndArray();
	

	std::ofstream out(file);
	out << s.GetString();
	std::cout << s.GetString();
	return true;
}


extern "C" IFaultTree *getFaultTree()
{
	return new FaultTree();
}