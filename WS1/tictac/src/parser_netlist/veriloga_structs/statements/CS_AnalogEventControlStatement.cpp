/*
 * CS_AnalogEventControlStatement.cpp
 *
 *  Created on: Dec 20, 2012
 *      Author: benk
 */

#include "CS_AnalogEventControlStatement.hpp"

CS_AnalogEventControlStatement::CS_AnalogEventControlStatement( CS_AnalogStatement* condInstr,
    const CS_ModuleDeclaration* module, int startLine, int startColumn):
CS_AnalogStatement( module, startLine, startColumn), eventConditions_(), condInstr_(condInstr), nrEvents_(0)
{
  // nothing to do here more
}
