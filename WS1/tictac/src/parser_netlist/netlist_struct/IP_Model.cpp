/*
 * IP_Model.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: benk
 */

#include "IP_Model.hpp"

IP_Model::IP_Model(std::vector<IP_Token> &tokens, std::string &name,
    std::string &type, std::vector < boost::shared_ptr<IP_Param> > &modelParams)
: IP_NetlistStructBase(tokens), name_(name), type_(type), modelParams_(modelParams)
{
  // all is set
}
