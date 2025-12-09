#pragma once
#include "CError.h"
#include "CTree.h"
#include <fstream>

template<typename T>
class CResultSaver
{
public:
    static void vSaveToFile(const std::string& sFileName, const CResult<T, CError>& cResult)
    {
        std::ofstream fFile(sFileName.c_str());
        if (!fFile.is_open()) return;

        fFile << "Type = Unknown" << "\n";

        if (!cResult.bIsSuccess())
        {
            const std::vector<CError*> vErrors = cResult.vGetErrors();

            for (size_t i = 0; i < vErrors.size(); i++)
            {
                if (vErrors[i] != NULL)
                {
                    fFile << vErrors[i]->sGetMessage() << "\n";
                }
            }
        }

        fFile.close();
    }
};

template <>
inline void CResultSaver<CTree>::vSaveToFile(const std::string& sFileName, const CResult<CTree, CError>& cResult)
{
    std::ofstream fFile(sFileName.c_str());
    if (!fFile.is_open()) return;

    fFile << "Type = CTree" << "\n";

    if (cResult.bIsSuccess())
    {
        fFile << cResult.cGetValue().sGetFormulaString();
    }
    else
    {
        const std::vector<CError*>& vErrors = cResult.vGetErrors();
        for (size_t i = 0; i < vErrors.size(); i++)
        {
            if (vErrors[i] != NULL)
            {
                fFile << vErrors[i]->sGetMessage() << "\n";
            }
        }
    }
    fFile.close();
}
