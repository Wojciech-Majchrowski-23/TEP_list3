#pragma once
#include "CError.h"
#include <vector>
#include <map>
#include <set>

class CTree;

enum Type {
    CNODE_UNKNOWN,
    CNODE_CONSTANT,
    CNODE_VARIABLE,
    CNODE_OPERATOR1,
    CNODE_OPERATOR2
};

class CNode
{
private:

    std::vector<CNode*>pVecCNode_children;
    int iNumberOfChildren;
    CNode* pCNode_parent;

    std::string sToken;
    Type enType;
    double dValue;

    static const std::string opAdd;
    static const std::string opSub;
    static const std::string opMult;
    static const std::string opDiv;
    static const std::string opSin;
    static const std::string opCos;

    static const std::string S_DEFAULT_TOKEN;
    static const double D_DEFAULT_VALUE;

    double getValueFromMap(const std::map<std::string, double>& cVariableMap, CError& cError) const;
    double getValueOperator1(const std::map<std::string, double>& cVariableMap, CError& cError) const;
    double getValueOperator2(const std::map<std::string, double>& cVariableMap, CError& cError) const;
    void setType();


public:
    CNode();
    CNode(const CNode& cOther);
    ~CNode();
    CNode* pClone() const;

    void vLoad(std::vector<std::string>& vecStr_tokens, int& offset, CError& cError);
    double dCalculate(const std::map<std::string, double>& cVariableMap, CError& cError) const;
    void vFindVariables(std::set<std::string>& setStr_variables);
    std::string sTostring();
    CNode* cNodeFindFirstLeaf();
    bool bReplaceChild(CNode* cNodeOldChild, CNode* cNodeNewChild);

    void vIncreaseLeafCount(int& iNumberOfLeafs);

    static bool bIsConstant(const std::string& sToken);
    static bool bIsOperator(const std::string& sToken);

    void vSetParent(CNode* pParent) { pCNode_parent = pParent; }
    CNode* cNodeGetParent() { return  pCNode_parent; }
};