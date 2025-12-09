#include "CError.h"
#include <sstream>

CError::CError() : bErrorOccurred(false), sErrorMessage(""), iErrorCount(0) { }

CError::CError(const std::string& sMessage)
{
    bErrorOccurred = true;
    sErrorMessage = sMessage;
    iErrorCount = 1;
}

CError::CError(const CError& cOther)
{
    bErrorOccurred = cOther.bErrorOccurred;
    sErrorMessage = cOther.sErrorMessage;
    iErrorCount = cOther.iErrorCount;
}


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
