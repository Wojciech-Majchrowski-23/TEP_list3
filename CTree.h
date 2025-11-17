#pragma once
#include "CError.h"
#include "CNode.h"
#include <set>
#include <vector>
#include <string>
#include <map>

class CTree
{
private:
	CNode* cRoot;
	std::set<std::string> setStr_variables;
	std::map<std::string, double> cVariableMap;

	void vInitializeMap(const std::vector<double>& vecValues);
	void vPopulateVariables();
	static std::vector<std::string> tokenize(std::string formula, CError& cError);

	CTree operator+(const CTree& cOther) const;
	CTree& operator=(const CTree& cOther);

public:
	CTree();
	~CTree();
	CTree(const CTree& cOther);

	//join
	void vJoin(std::string sFormula, CError& cError);

	//enter
	void vBuildTree(std::string formula, CError& cError);

	//comp
	double dCalculate(const std::vector<double>& vecValues, CError& cError);

	//print
	std::string sGetFormulaString() const;

	//vars
	const std::set<std::string>& sGetVariables() const { return setStr_variables; }

};

