#include "CTree.h"
#include <sstream>
#include <cctype>

CTree::CTree(): cRoot(NULL){ }

CTree::~CTree()
{
    if (cRoot != NULL)
    {
        // on wywoluje kaskadowe usuwanie swoich dzieci
        delete cRoot;
    }
}

CTree::CTree(const CTree& cOther)
{
    if (cOther.cRoot != NULL)
    {
        // gleboka kopia roota innego drzewa
        cRoot = cOther.cRoot->pClone();
    }
    else
    {
        cRoot = NULL;
    }
}

CTree& CTree::operator=(const CTree& cOther)
{
    if (this == &cOther)
    {
        return *this;
    }

    delete cRoot;

    if (cOther.cRoot != NULL)
    {
        cRoot = cOther.cRoot->pClone();
    }
    else
    {
        cRoot = NULL;
    }

    return *this;
}

CTree CTree::operator+(const CTree& cOther) const
{
    CTree thisTreeCopy(*this);
    CNode* pCNodeLeaf = thisTreeCopy.cRoot->cNodeFindFirstLeaf(); // wskaznik na pierwszy lisc na lewo w dol
    CNode* pCNodeLeafParent = pCNodeLeaf->cNodeGetParent(); // wskaznik na rodzica tego liscia

    CNode* pClonedOtherRoot = NULL;
    if (cOther.cRoot != NULL)
    {
        pClonedOtherRoot = cOther.cRoot->pClone();
    }

    pCNodeLeafParent->bReplaceChild(pCNodeLeaf, pClonedOtherRoot);

	return thisTreeCopy;
}

void CTree::vJoin(std::string sFormula, CError& cError)
{
    CTree cOtherTree;
    cOtherTree.vBuildTree(sFormula, cError);

    *this = *this + cOtherTree;

    vPopulateVariables();
}

void CTree::vBuildTree(std::string sFormula, CError& cError)
{
	std::vector<std::string>vecStr_tokens = tokenize(sFormula, cError);
	if (vecStr_tokens.empty())
	{
		cError.vSetError("Empty formula");
		return;
	}
	int offset = 0;

	cRoot = new CNode();
	cRoot->vSetParent(NULL);
	cRoot->vLoad(vecStr_tokens, offset, cError);

    if (offset < vecStr_tokens.size())
    {
        std::string sExtraToken = vecStr_tokens[offset];
        cError.vSetError("Redundant arguments ignored (starting with: '" + sExtraToken + "').");
    }

    vPopulateVariables();
}

void CTree::vPopulateVariables()
{
    setStr_variables.clear();

    if (cRoot != NULL)
    {
        cRoot->vFindVariables(setStr_variables);
    }
}


double CTree::dCalculate(const std::vector<double>& vecValues, CError& cError)
{
    if (vecValues.size() == setStr_variables.size())
    {
        vInitializeMap(vecValues);
        return cRoot->dCalculate(cVariableMap, cError);
    }

    cError.vSetError("Wrong number of constants");
	return -1.0;
}

void CTree::vInitializeMap(const std::vector<double>& vecValues)
{
    cVariableMap.clear();

    // iterator wskaznik do chodzenia po secie
    std::set<std::string>::iterator it = setStr_variables.begin();

    for (size_t i = 0; i < vecValues.size(); i++)
    {
        cVariableMap[*it] = vecValues[i];
        it++;
    }
}

std::string CTree::sGetFormulaString() const
{
    if (cRoot != NULL)
    {
        return cRoot->sTostring();
    }

}

std::vector<std::string> CTree::tokenize(std::string formula, CError& cError)
{
    std::vector<std::string> vecResult;
    std::stringstream ss(formula);
    std::string sToken;
    bool bWasCleaned = false;

    while (ss >> sToken)
    {
        if (CNode::bIsOperator(sToken))
        {
            vecResult.push_back(sToken);
        }
        else if (CNode::bIsConstant(sToken))
        {
            vecResult.push_back(sToken);
        }
        else
        {
            // czyszczenie zmiennych
            std::string sCleanedToken = "";
            bool bHasLetter = false;

            for (int i = 0; i < sToken.length(); i++)
            {
                unsigned char c = static_cast<unsigned char>(sToken[i]);

                if (isalpha(c))
                {
                    bHasLetter = true;
                    sCleanedToken += c;
                }
                else if (isdigit(c))
                {
                    // Nie dodawaj cyfry, jesli nie bylo wczesniej litery
                    if (bHasLetter)
                    {
                        sCleanedToken += c;
                    }
                    else
                    {
                        bWasCleaned = true;
                    }
                }
                else
                {
                    bWasCleaned = true;
                }
            }

            if (sToken != sCleanedToken)
            {
                cError.vSetError("Ignored or fixed invalid token: '" + sToken + "'");
            }

            if (bHasLetter && !sCleanedToken.empty())
            {
                vecResult.push_back(sCleanedToken);
            }
            else if (!sToken.empty())
            {
                bWasCleaned = true; // ignorowanie calego tokenu
            }
        }

    }

    if (bWasCleaned)
    {
        cError.vSetError("The formula contained invalid characters or names that were ignored/fixed.");
    }

    return vecResult;
}
