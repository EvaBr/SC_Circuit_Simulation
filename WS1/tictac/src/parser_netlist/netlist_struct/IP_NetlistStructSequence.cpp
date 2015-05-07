/*
 * IP_NetlistStructSequence.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: benk
 */

#include "IP_NetlistStructSequence.hpp"

IP_NetlistStructSequence::IP_NetlistStructSequence(std::vector<IP_Token> &tokens):
IP_NetlistStructBase(tokens)
{
  //
  sequence_.clear();
}

