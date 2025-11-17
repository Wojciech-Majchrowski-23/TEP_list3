#include "CNodeOperator2.h"

CNodeOperator2::~CNodeOperator2()
{
    if (pChildLeft != NULL){ delete pChildLeft; }
    if (pChildRight != NULL){ delete pChildRight; }

}