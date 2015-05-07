/*
 * IP_SubcirDef.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: benk
 */

#include "IP_SubcirDef.hpp"

IP_SubcirDef::IP_SubcirDef(std::vector<IP_Token> &tokens,
    std::string &name,
    std::vector < boost::shared_ptr<IP_Param> > &subCirParams,
    std::vector < IP_Token > &subCircPorts)
: IP_NetlistStructBase(tokens)
{
  // default value is false
  isTopLevel_ = false;
  // initialize also the subcircuit body
  // subcktBody_ = boost::shared_ptr<IP_NetlistStructSequence>(new IP_NetlistStructSequence(tokens));

  subCirName_   = name;
  subCirParams_ = subCirParams;
  subCircPorts_ = subCircPorts;

}

