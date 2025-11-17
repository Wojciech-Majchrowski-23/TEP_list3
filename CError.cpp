#include "CError.h"
#include <sstream>

CError::CError() : bErrorOccurred(false), sErrorMessage(""), iErrorCount(0) { }


void CError::vSetError(const std::string& sMessage)
{
    bErrorOccurred = true;
    iErrorCount++;

    std::stringstream ss;
    ss << iErrorCount;
    std::string sCounter = ss.str();

    sErrorMessage += sCounter + ". " + sMessage + "\n";
}

bool CError::bHasError() const
{
    return bErrorOccurred;
}

std::string CError::sGetMessage() const
{
    return sErrorMessage;
}

void CError::vClear()
{
    bErrorOccurred = false;
    sErrorMessage = "";
    iErrorCount = 0;
}
