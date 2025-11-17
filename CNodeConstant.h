#pragma once
#include "CNode.h"

class CNodeConstant : public CNode
{
private:
    double dValue;
public:
    // takie funkcje z klamerkami to funkcje inline,
    //ktore sa juz tutaj zdefiniowane. Nie definiujemy ich znowu w pliku .cpp
    CNodeConstant(double dVal) : dValue(dVal) {}
    virtual double dCalculate(const CVariableMap& mVariables) const { return dValue; }
    virtual std::string sToString() const;
    virtual void vGetVariables(std::set<std::string>& sVariables) const { }
    virtual CNode* pClone() const { return new CNodeConstant(*this); }
    virtual void vParse(const std::vector<std::string>& vTokens, int& iOffset, bool& bError) { }
    virtual void vAddDefaultChildren() { }
    // puste klamerki - nie rob nic
};

