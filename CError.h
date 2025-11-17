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

    void vSetError(const std::string& sMessage);

    bool bHasError() const;

    std::string sGetMessage() const;

    void vClear();
};