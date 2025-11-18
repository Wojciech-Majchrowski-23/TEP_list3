#include "CNode.h"
#include <cmath>

const std::string CNode::opAdd = "+";
const std::string CNode::opSub = "-";
const std::string CNode::opMult = "*";
const std::string CNode::opDiv = "/";
const std::string CNode::opSin = "sin";
const std::string CNode::opCos = "cos";

const std::string CNode::S_DEFAULT_TOKEN = "1";
const double CNode::D_DEFAULT_VALUE = 1.0;

CNode::CNode() : pCNode_parent(NULL), enType(CNODE_UNKNOWN), iNumberOfChildren(0), sToken(""), dValue(0.0) {}

CNode::CNode(const CNode& cOther)
{
    sToken = cOther.sToken;
    enType = cOther.enType;
    dValue = cOther.dValue;
    iNumberOfChildren = cOther.iNumberOfChildren;

    pCNode_parent = NULL;
}

CNode::~CNode()
{
    for (size_t i = 0; i < pVecCNode_children.size(); i++)
    {
        if (pVecCNode_children[i] != NULL)
        {
            delete pVecCNode_children[i];
        }
    }
}

CNode* CNode::pClone() const
{
    CNode* thisCopy = new CNode(*this);

    for (int i = 0; i < iNumberOfChildren; i++)
    {
        CNode* pClonedChild = pVecCNode_children[i]->pClone();
        pClonedChild->vSetParent(thisCopy);
        thisCopy->pVecCNode_children.push_back(pClonedChild);
    }
    return thisCopy;
}

void CNode::vLoad(std::vector<std::string>& vecStr_tokens, int& offset, CError& cError)
{
    if (offset >= vecStr_tokens.size())
    {
        std::string sOperator = pCNode_parent->sToken;
        cError.vSetError("No argument for the operator '" + sOperator + "'. Filling with '1'.");

        sToken = S_DEFAULT_TOKEN;
        setType();
        dValue = D_DEFAULT_VALUE;

        return;
    }

    sToken = vecStr_tokens[offset];
    offset++;
    setType();
    if (enType == CNODE_CONSTANT) dValue = atof(sToken.c_str());


    for (int i = 0; i < iNumberOfChildren; i++)
    {
        CNode* pNewChild = new CNode();
        pNewChild->vSetParent(this);

        pVecCNode_children.push_back(pNewChild);
        pNewChild->vLoad(vecStr_tokens, offset, cError);
    }
}

double CNode::dCalculate(const std::map<std::string, double>& cVariableMap, CError& cError) const
{
    switch (enType)
    {
    case CNODE_CONSTANT:
        return dValue;
    case CNODE_VARIABLE:
        return getValueFromMap(cVariableMap, cError);
    case CNODE_OPERATOR1:
        return getValueOperator1(cVariableMap, cError);
    case CNODE_OPERATOR2:
        return getValueOperator2(cVariableMap, cError);
    }
}

double CNode::getValueFromMap(const std::map<std::string, double>& cVariableMap, CError& cError) const
{
    std::map<std::string, double>::const_iterator it = cVariableMap.find(sToken);

    if (it != cVariableMap.end())
    {
        return it->second;
    }

    cError.vSetError("Internal Logic Error: Variable '" + sToken + "' not found in map during calculation.");
    return 0.0;
}

double CNode::getValueOperator1(const std::map<std::string, double>& cVariableMap, CError& cError) const
{
    if (sToken == opSin)

    {
        return std::sin(pVecCNode_children[0]->dCalculate(cVariableMap, cError));
    }
    return std::cos(pVecCNode_children[0]->dCalculate(cVariableMap, cError));
}

double CNode::getValueOperator2(const std::map<std::string, double>& cVariableMap, CError& cError) const
{
    double dLeft = pVecCNode_children[0]->dCalculate(cVariableMap, cError);
    double dRight = pVecCNode_children[1]->dCalculate(cVariableMap, cError);

    if (sToken == opAdd) return dLeft + dRight;
    if (sToken == opSub) return dLeft - dRight;
    if (sToken == opMult) return dLeft * dRight;
    if (sToken == opDiv)
    {
        if (dRight == 0.0)
        {
            cError.vSetError("Runtime Error: Division by zero detected.");
            return 0.0;
        }
        return dLeft / dRight;
    }
    cError.vSetError("Internal Logic Error: Operator2 token '" + sToken + "' is not a valid operator.");
    return 0.0;
}



void CNode::vFindVariables(std::set<std::string>& setStr_variables)
{
    if (enType == CNODE_VARIABLE)
    {
        setStr_variables.insert(sToken);
    }

    for (size_t i = 0; i < iNumberOfChildren; i++)
    {
        if (pVecCNode_children[i] != NULL)
        {
            pVecCNode_children[i]->vFindVariables(setStr_variables);
        }
    }
}

std::string CNode::sTostring()
{
    switch (enType)
    {
    case CNODE_CONSTANT:
    case CNODE_VARIABLE:
        return sToken;

    case CNODE_OPERATOR1:
        return sToken + " " + pVecCNode_children[0]->sTostring();

    case CNODE_OPERATOR2:
        return sToken + " " + pVecCNode_children[0]->sTostring() + " " + pVecCNode_children[1]->sTostring();
    }
}

CNode* CNode::cNodeFindFirstLeaf()
{
    if (enType == CNODE_CONSTANT || enType == CNODE_VARIABLE) return this;
    return pVecCNode_children[0]->cNodeFindFirstLeaf();
}

bool CNode::bReplaceChild(CNode* cNodeOldChild, CNode* cNodeNewChild)
{
    for (int i = 0; i < iNumberOfChildren; i++)
    {
        if (pVecCNode_children[i] == cNodeOldChild)
        {
            if (cNodeOldChild != NULL)
            {
                delete cNodeOldChild;
            }

            pVecCNode_children[i] = cNodeNewChild;

            if (cNodeNewChild != NULL)
            {
                cNodeNewChild->vSetParent(this);
            }

            return true;
        }
    }
    return false;
}

void CNode::vIncreaseLeafCount(int& iNumberOfLeafs)
{
    if (enType == CNODE_VARIABLE || enType == CNODE_CONSTANT)
    {
        iNumberOfLeafs++;
    }
    else 
    {
        for (int i = 0; i < iNumberOfChildren; i++)
        {
            pVecCNode_children[i]->vIncreaseLeafCount(iNumberOfLeafs);
        }
    }
}

void CNode::setType()
{
    if (bIsOperator(sToken))
    {
        if (sToken == opAdd || sToken == opSub || sToken == opMult || sToken == opDiv) 
        {
            enType = CNODE_OPERATOR2; 
            iNumberOfChildren = 2;
        }
        else if (sToken == opSin || sToken == opCos)
        {
         enType = CNODE_OPERATOR1; 
         iNumberOfChildren = 1;
        }
    }
    else if (bIsConstant(sToken)) enType = CNODE_CONSTANT;
    else enType = CNODE_VARIABLE;   // Logika tworzenia variable w tokenize dba o to, zeby to byl poprawny variable
}

bool CNode::bIsConstant(const std::string& sToken)
{
    if (sToken.empty())
    {
        return false;
    }

    for (size_t i = 0; i < sToken.length(); i++)
    {
        if (!isdigit(static_cast<unsigned char>(sToken[i])))
        {
            return false;
        }
    }
    return true;
}

bool CNode::bIsOperator(const std::string& sToken)
{
    return (sToken == opAdd || sToken == opSub || sToken == opMult ||
        sToken == opDiv || sToken == opSin || sToken == opCos);
}
