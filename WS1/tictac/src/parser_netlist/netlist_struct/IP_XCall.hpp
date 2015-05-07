/*
 * IP_XCall.hpp
 *
 *  Created on: Jan 19, 2015
 *      Author: benk
 */

#ifndef IP_XCALL_HPP_
#define IP_XCALL_HPP_

#include "IP_NetlistStructBase.hpp"
#include "IP_Param.hpp"

/** this class represents the instantioation of a subcircuit */
class IP_XCall : public IP_NetlistStructBase
{
public:

  /** Ctor with the original tokens */
  IP_XCall(std::vector<IP_Token> &tokens,
      int nrPinns, std::string &sucktName,
      std::vector < boost::shared_ptr<IP_Param> > &callParams );

  /** empty Dtor */
  virtual ~IP_XCall() {;}

  /** method for the data structure traversal*/
  virtual void Accept(IP_NetlistVisitBase &visitor) { visitor.Visit( (IP_XCall &)(*this)); }

  /** returns the number of pinns */
  int getNrPinns() const { return nrPinns_; }

  /// returns the instantion name of the pinns
  const std::string& getInstantionationPinn(MYINT i) const { return getToken(i+1).getValue(); }
  /// returns the X-call instantion
  const std::string& getInstantionationXname() const { return getToken(0).getValue(); }

  /** returnes the called subcircuit name */
  const std::string& getSubcktName() const { return sucktName_; }

  /** returns the number of parameters on the X-call*/
  int getNrParams() const { (int)callParams_.size(); }

  /** returns the parameter on the X-call */
  boost::shared_ptr<IP_Param>& getCalledParam(int i) { return callParams_[i]; }
  const boost::shared_ptr<IP_Param>& getCalledParam(int i) const { return callParams_[i]; }

  /** @returns positive number if the instantiation contains a given parameter, -1 otherwise*/
  MYINT hasInstParam(const std::string &paramName)
  {
    for (MYINT i = 0; i < (MYINT)callParams_.size(); i++)
      if (paramName == callParams_[i]->getName()) return i;
    return -1;
  }

private:

  /** nr pinns of the call */
  int nrPinns_;

  /** name of the called subcircuit */
  std::string sucktName_;

  /** all the parametres on the X-call */
  std::vector < boost::shared_ptr<IP_Param> > callParams_;

};

#endif /* IP_XCALL_HPP_ */
