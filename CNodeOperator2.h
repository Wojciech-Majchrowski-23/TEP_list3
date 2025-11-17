#pragma once
#include "CNode.h"

class CNodeOperator2 : public CNode
{
private:
    CNode* pChildLeft;
    CNode* pChildRight;
    std::string sOperator; // Przechowuje znak, np. "+"
public:
    CNodeOperator2(std::string sOp) : pChildLeft(NULL), pChildRight(NULL), sOperator(sOp) {}
    virtual ~CNodeOperator2(); // Musi wywołać delete na pChildLeft i pChildRight

    virtual double dCalculate(const CVariableMap& mVariables) const; // Obliczy dzieci i doda wyniki
    virtual std::string sToString() const; // Zwróci np. "+ 1 2"
    virtual void vGetVariables(std::set<std::string>& sVariables) const; // Poprosi dzieci o zmienne
    virtual CNode* pClone() const; // Stworzy kopię siebie i sklonuje dzieci

    // Ta funkcja jest kluczowa dla budowania drzewa z PDF [cite: 145]
    virtual void vParse(const std::vector<std::string>& vTokens, int& iOffset, bool& bError);
    virtual void vAddDefaultChildren(); // Doda "1" jeśli brakuje argumentów
};

