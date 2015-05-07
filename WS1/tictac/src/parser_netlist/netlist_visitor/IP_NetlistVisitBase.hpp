/*
 * IP_NetlistVisitBase.hpp
 *
 *  Created on: Feb 4, 2015
 *      Author: benk
 */

#ifndef IP_NETLISTVISITBASE_HPP_
#define IP_NETLISTVISITBASE_HPP_

#include "sim_utils.hpp"

class IP_SubcirDef;

class IP_NetlistStructBase;
class IP_ControlBase;
class IP_NetlistStructSequence;

class IP_SubcirDef;
class IP_Element;
class IP_Function;
class IP_Model;
class IP_Param;
class IP_SubcirSettings;
class IP_XCall;
class IP_ConditionedNetlist;
class IP_GlobalSettings;
class IP_Option;
class IP_Output;
class IP_Measurement;
class IP_Assertion;
class IP_AnaysisCard;


/** this is the base class to visit all entities of the parsed (directly by the parser)
 * data structures */
class IP_NetlistVisitBase : public UT_SIMVerboseClass
{
public:

  /** empty Ctor*/
  IP_NetlistVisitBase();

  /** empty Dtor */
  virtual ~IP_NetlistVisitBase() {;}

  /** the main entry point to visit a netlist
   * @param toplevelToVisit the top-lvel subcircuit */
  virtual void   startTopLevelVisit(boost::shared_ptr<IP_SubcirDef> toplevelToVisit);

  virtual void   Visit(IP_NetlistStructBase &node);
  virtual void   Visit(IP_NetlistStructSequence &node);
  virtual void   Visit(IP_ControlBase &node);
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

  void   VisitChildren(IP_NetlistStructBase &node);
  void   VisitChildren(IP_NetlistStructSequence &node);
  void   VisitChildren(IP_ControlBase &node);
  void   VisitChildren(IP_SubcirDef &node);
  void   VisitChildren(IP_Element &node);
  void   VisitChildren(IP_Function &node);
  void   VisitChildren(IP_Model &node);
  void   VisitChildren(IP_Param &node);
  void   VisitChildren(IP_SubcirSettings &node);
  void   VisitChildren(IP_XCall &node);
  void   VisitChildren(IP_ConditionedNetlist &node);
  void   VisitChildren(IP_GlobalSettings &node);
  void   VisitChildren(IP_Option &node);
  void   VisitChildren(IP_Output &node);
  void   VisitChildren(IP_Measurement &node);
  void   VisitChildren(IP_Assertion &node);
  void   VisitChildren(IP_AnaysisCard &node);


protected:

  /**key method for node visiting  */
  virtual void visitNode(IP_NetlistStructBase *node);

  /** the actual top-level subcircuit, which is visited */
  IP_SubcirDef* actTopLevel_;

  /** flag to signal if we are on the top level */
  bool isTopLevel_;
  /** the actual level of subcircuit definition (0 is toplevel)*/
  MYINT level_;

  /// to show if a param is within a subcircuit definition
  bool isParamWithinSubcktDef_;
  /// is parameter within X-call
  bool isParamWithinXCall_;
  /// is parameter within model
  bool isParamWithinModel_;

};

#endif /* IP_NETLISTVISITBASE_HPP_ */
