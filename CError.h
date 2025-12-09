#pragma once
#include <string>

class CError
{
private:
    bool bErrorOccurred;
    std::string sErrorMessage;
    int iErrorCount;

public:

    CError();

    CError(const std::string& sMessage);

    CError(const CError& cOther);

    void vSetError(const std::string& sMessage);

    bool bHasError() const;

    std::string sGetMessage() const;

    void vClear();
};