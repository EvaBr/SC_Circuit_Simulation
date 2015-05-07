/*
 * CS_BranchDeclaration.cpp
 *
 *  Created on: Dec 18, 2012
 *      Author: benk
 */

#include "CS_BranchDeclaration.hpp"

CS_BranchDeclaration::CS_BranchDeclaration(const CS_ModuleDeclaration* module, int startLine, int startColumn, std::string name,
    CS_AnalogPortReference *pin1, CS_AnalogPortReference *pin2):
name_(name), module_(module), startLine_(startLine), startColumn_(startColumn), pin1_(pin1), pin2_(pin2)
{
  // nothing to do more here yet
  // todo: make checks that the expressions are OK
}
