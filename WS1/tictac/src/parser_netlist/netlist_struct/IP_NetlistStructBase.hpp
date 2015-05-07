/*
 * IP_NetlistStructBase.hpp
 *
 *  Created on: Jan 29, 2015
 *      Author: benk
 */

#ifndef IP_NETLISTSTRUCTBASE_HPP_
#define IP_NETLISTSTRUCTBASE_HPP_

#include "IP_Token.hpp"
#include "sim_utils.hpp"
#include "IP_NetlistVisitBase.hpp"

#include "IP_NetlistStructInfo.hpp"

/** the base class for ALL components which can appear in the netlist */
class IP_NetlistStructBase
{
public:

  /** the basic class Ctor with a list of tokens */
  IP_NetlistStructBase(std::vector<IP_Token> &tokens);

  /** empty Dtor */
  virtual ~IP_NetlistStructBase() {;}

  /** method for the data structure traversal*/
  virtual void Accept(IP_NetlistVisitBase &visitor) { visitor.Visit( (IP_NetlistStructBase &)(*this)); }

  /** returns the number of tokens */
  MYINT getNrTokens() const { return (MYINT)tokens_.size(); }

  /** returns the token */
  IP_Token& getToken(int i) { return tokens_[i]; }
  const IP_Token& getToken(int i) const { return tokens_[i]; }

  /** returns reference to the general information object */
  IP_NetlistStructInfo& getInfo() { return info_; }
  const IP_NetlistStructInfo& getInfo() const { return info_; }

protected:

  /** the tokens representing the statement, or at least the first record (e.g. .subckt)*/
  std::vector<IP_Token> tokens_;
  /// each node will hold this class to store general infoemation in each node
  IP_NetlistStructInfo info_;
};

#endif /* IP_NETLISTSTRUCTBASE_HPP_ */
