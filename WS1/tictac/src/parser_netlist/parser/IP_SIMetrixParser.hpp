/*
 * IP_SIMetrixParser.hpp
 *
 *  Created on: Mar 14, 2015
 *      Author: benk
 */

#ifndef IP_SIMETRIXPARSER_HPP_
#define IP_SIMETRIXPARSER_HPP_

#include "IP_ParserBase.hpp"

/** parser class with special extentions for the SIMetrix netlists */
class IP_SIMetrixParser : public IP_ParserBase
{
public:

  IP_SIMetrixParser();

  virtual ~IP_SIMetrixParser() {;}

  /// for the output we also have special ".GRAPH"
  virtual bool matchOutput(const std::vector<IP_Token> &tokens);

  /// for X-calls we have a special handling for SIMetrix
  virtual boost::shared_ptr<IP_XCall>                 buildXCall();
};

#endif /* IP_SIMETRIXPARSER_HPP_ */
