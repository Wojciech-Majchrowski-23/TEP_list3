#pragma once
#include "CTree.h"
#include <string>
#include <vector>

class CUI
{
private:

    CTree cTree;
    bool bRunning;

    static const std::string sEnterCommand;
    static const std::string sJoinCommand;
    static const std::string sCompCommand;
    static const std::string sVarsCommand;
    static const std::string sPrintCommand;
    static const std::string sHelpCommand;
    static const std::string sExitCommand;

    void vHandleEnter(const std::string& sArguments);
    void vHandleJoin(const std::string& sArguments);
    void vHandleComp(const std::string& sArguments);
    void vHandleVars() const;
    void vHandlePrint() const;
    void vHandleExit();
    void vHandleHelp() const;
    void vParseCommand(std::string sInput);

    void vPrintError(const CError& cError) const;

public:
    CUI();
    ~CUI();
    void vRun();
};

