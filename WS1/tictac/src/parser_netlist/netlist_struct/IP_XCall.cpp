/*
 * IP_XCall.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: benk
 */

#include "IP_XCall.hpp"

IP_XCall::IP_XCall(std::vector<IP_Token> &tokens,
    int nrPinns, std::string &sucktName,
    std::vector < boost::shared_ptr<IP_Param> > &callParams )
: IP_NetlistStructBase(tokens), nrPinns_(nrPinns), sucktName_(sucktName), callParams_(callParams)
{
  // all fields should be set
}

