/*
 * IP_Params.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: benk
 */

#include "IP_Param.hpp"
#include "IP_ScannerBase.hpp"

IP_Param::IP_Param(std::vector<IP_Token> &tokens) : IP_NetlistStructBase(tokens)
{
  // get the name ,and the initialization expression
  name_ = IP_ScannerBase::toLower( tokens[0].getValue() );
  exprToken_ = tokens[1];
  exprToken_.setTokenType(TK_EXPRESSION);
}

