/*
 * IP_SubcktCopyVisitor.hpp
 *
 *  Created on: Mar 16, 2015
 *      Author: benk
 */

#ifndef IP_SUBCKTCOPYVISITOR_HPP_
#define IP_SUBCKTCOPYVISITOR_HPP_

#include "IP_NetlistVisitBase.hpp"

class IP_Token;

/** class to copy a subcircuit definition
 *
 * This class could later alse be used for "filtering" a given netlist */
class IP_SubcktCopyVisitor : public IP_NetlistVisitBase
{
public:

  IP_SubcktCopyVisitor();

  virtual ~IP_SubcktCopyVisitor() {;}

  virtual void   startTopLevelVisit(boost::shared_ptr<IP_SubcirDef> toplevelToVisit);

  virtual void   Visit(IP_NetlistStructSequence &node);
  virtual void   Visit(IP_SubcirDef &node);
  virtual void   Visit(IP_Element &node);
  virtual void   Visit(IP_Function &node);
  virtual void   Visit(IP_Model &node);
  virtual void   Visit(IP_Param &node);
  virtual void   Visit(IP_SubcirSettings &node);
  virtual void   Visit(IP_XCall &node);
  virtual void   Visit(IP_ConditionedNetlist &node);
  virtual void   Visit(IP_GlobalSettings &node);
  virtual void   Visit(IP_Option &node);
  virtual void   Visit(IP_Output &node);
  virtual void   Visit(IP_Measurement &node);
  virtual void   Visit(IP_Assertion &node);
  virtual void   Visit(IP_AnaysisCard &node);

  /// returns the copied subcircuit
  boost::shared_ptr<IP_SubcirDef> getResultingSubcircuitDef() { return resultedSubckt_; }

protected:

  /// method to create "tokens_" based on an existent element
  void getTokenSeqence(IP_NetlistStructBase* elem);

  /// the list of tokens to copy one element
  std::vector<IP_Token> tokens_;

  /// the returned objects
  boost::shared_ptr<IP_NetlistStructBase> ret_;
  boost::shared_ptr<IP_NetlistStructSequence> seq_;

  /// the resulted subcircuit definition
  boost::shared_ptr<IP_SubcirDef> resultedSubckt_;

};

#endif /* IP_SUBCKTCOPYVISITOR_HPP_ */
