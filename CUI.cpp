#include "CUI.h"
#include <iostream>
#include <sstream>

const std::string CUI::sEnterCommand = "enter";
const std::string CUI::sJoinCommand = "join";
const std::string CUI::sCompCommand = "comp";
const std::string CUI::sVarsCommand = "vars";
const std::string CUI::sPrintCommand = "print";
const std::string CUI::sHelpCommand = "help";
const std::string CUI::sExitCommand = "exit";

CUI::CUI(): bRunning(true) { }

CUI::~CUI() { }

void CUI::vRun()
{
	std::string sInputLine;

	while (bRunning)
	{
		std::cout << "> ";
		std::getline(std::cin, sInputLine);

		if (!sInputLine.empty())
		{
			vParseCommand(sInputLine);
		}
	}
}

void CUI::vHandleEnter(const std::string& sArguments)
{
	CError cErrorEnter;
	cTree.vBuildTree(sArguments, cErrorEnter);
	vPrintError(cErrorEnter);
	if (cErrorEnter.bHasError()) { std::cout << "Repaired: ";  vHandlePrint(); }
}

void CUI::vHandleJoin(const std::string& sArguments)
{
	CError cErrorJoin;
	cTree.vJoin(sArguments, cErrorJoin);
	vPrintError(cErrorJoin);
}

void CUI::vHandleComp(const std::string& sArguments)
{
	CError cError;
	std::vector<double> vValues;
	std::stringstream ss(sArguments);
	std::string sToken;

	while (ss >> sToken)
	{
		if(CNode::bIsConstant(sToken)) vValues.push_back(std::atof(sToken.c_str()));
		else
		{
			cError.vSetError("Inapropriate input of comp");
		}
	}

	if (cError.bHasError())
	{
		vPrintError(cError);
	}
	else
	{
		double dResult = cTree.dCalculate(vValues, cError);
		std::cout << "Result: " << dResult << std::endl;
	}
}

void CUI::vHandleVars() const
{
	const std::set<std::string>& sVars = cTree.sGetVariables();

	if (sVars.empty())
	{
		std::cout << "No variables found in the current formula." << std::endl;
		return;
	}

	std::cout << "Variables: ";

	std::set<std::string>::const_iterator it;
	for (it = sVars.begin(); it != sVars.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

void CUI::vHandlePrint() const
{
	std::cout << cTree.sGetFormulaString() << std::endl;
	std::cout << "Number Of Leafs: " << cTree.iGetNumberOfLeafs() << std::endl;
}

void CUI::vHandleExit()
{
	bRunning = false;
	std::cout << "Exiting." << std::endl;
}

void CUI::vHandleHelp() const
{
	std::cout << "Available commands:" << std::endl;
	std::cout << "  enter <formula>  - Enters a new formula (e.g., enter + 1 x)" << std::endl;
	std::cout << "  join <formula>   - Joins the current formula with a new one" << std::endl;
	std::cout << "  comp <values...> - Computes the formula (e.g., comp 1 2.5)" << std::endl;
	std::cout << "  vars             - Lists all variables in the formula" << std::endl;
	std::cout << "  print            - Prints the current formula in prefix notation" << std::endl;
	std::cout << "  exit             - Quits the application" << std::endl;
	std::cout << "  help             - Shows this help message" << std::endl;
}

void CUI::vParseCommand(std::string sInput)
{
	CError cError;
	std::stringstream ss(sInput);
	std::string sCommand;
	std::string sArguments;

	ss >> sCommand;

	std::getline(ss, sArguments);

	// usuwanie wiodacej spacji po komendzie, zaczyna się ona od " + a b"
	if (!sArguments.empty() && sArguments[0] == ' ')
	{
		sArguments = sArguments.substr(1);
	}

	if (sCommand == sEnterCommand)
	{
		vHandleEnter(sArguments);
	}
	else if (sCommand == sJoinCommand)
	{
		vHandleJoin(sArguments);
	}
	else if (sCommand == sCompCommand)
	{
		vHandleComp(sArguments);
	}
	else if (sCommand == sVarsCommand)
	{
		vHandleVars();
	}
	else if (sCommand == sPrintCommand)
	{
		vHandlePrint();
	}
	else if(sCommand == sHelpCommand)
	{
		vHandleHelp();
	}
	else if (sCommand == sExitCommand)
	{
		vHandleExit();
	}
	else
	{
		cError.vSetError("Incorrect command");
		vPrintError(cError);
		vHandleHelp();
	}
}

void CUI::vPrintError(const CError& cError) const
{
	if (cError.bHasError())
	{
		std::cout << "Error Log:\n" << cError.sGetMessage() << std::endl;
	}
}
