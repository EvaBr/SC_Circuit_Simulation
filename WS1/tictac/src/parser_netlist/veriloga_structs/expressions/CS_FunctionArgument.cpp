/*
 * CS_FunctionArgument.cpp
 *
 *  Created on: Feb 21, 2013
 *      Author: benk
 */

#include "CS_FunctionArgument.hpp"

CS_FunctionArgument::CS_FunctionArgument(
    std::string name, CS_AnalogFunctionDeclaration *function, int argIndex,
    const CS_ModuleDeclaration* module, int startLine, int startColumn)
: CS_ExpressionBasis(module, startLine, startColumn),
  argumentExpr_(0),name_(name), argIndex_(argIndex), functionDecr_(function)
{
}


