/*
 * IP_SubcirDef.hpp
 *
 *  Created on: Jan 19, 2015
 *      Author: benk
 */

#ifndef IP_SUBCIRDEF_HPP_
#define IP_SUBCIRDEF_HPP_

#include "IP_NetlistStructBase.hpp"
#include "IP_NetlistStructSequence.hpp"
#include "IP_Param.hpp"

/** this class represents a subcircuit definition */
class IP_SubcirDef  : public IP_NetlistStructBase
{
public:

  /** Ctor only with the tokens representing the header of the subcircuit */
  IP_SubcirDef(std::vector<IP_Token> &tokens,
      std::string &name,
      std::vector < boost::shared_ptr<IP_Param> > &subCirParams,
      std::vector < IP_Token > &subCircPorts);

  /** empty Dtor */
  virtual ~IP_SubcirDef() {;}

  /** returns true if this subcricuit is on the top level */
  bool isSubcirTopLevel() const { return isTopLevel_; }

  /** sets the flag for top level, default is false */
  void setTopLevelFlag(bool b) { isTopLevel_ = b; }

  /** returns the body of the subcircuit */
  boost::shared_ptr<IP_NetlistStructSequence>& getSubcktBody() { return subCirBody_; }
  const boost::shared_ptr<IP_NetlistStructSequence>& getSubcktBody() const { return subCirBody_; }

  /** sets the body of the subcircuit */
  void setSubcktBody(boost::shared_ptr<IP_NetlistStructSequence>& subCirBody) {  subCirBody_ = subCirBody; }

  /** returns a parameter of the subcircuit */
  boost::shared_ptr<IP_Param>& getSubCircuitParams(MYINT i) { return subCirParams_[i]; }
  const boost::shared_ptr<IP_Param>& getSubCircuitParams(MYINT i) const{ return subCirParams_[i]; }

  //// returns the name of the subcircuit call parameter
  const std::string& getParamName(MYINT i) const { return subCirParams_[i]->getName(); }

  /** returns the name of the subcircuit */
  const std::string& getName() const { return  subCirName_; }

  /** returns the number of pinns/ports*/
  MYINT getNrPorts() const { return (MYINT)subCircPorts_.size(); }

  /** returns the instantiated pinn names */
  const std::string& getPortInstName(MYINT i) const{ return subCircPorts_[i].getValue(); }

  /** returns the port token */
  IP_Token& getPortToken(int i) { return subCircPorts_[i]; }
  const IP_Token& getPortToken(int i) const { return subCircPorts_[i]; }

  /** get the number of parameters */
  MYINT getNrParams() const { return (MYINT)subCirParams_.size(); }

  boost::shared_ptr<IP_Param>& getParam(MYINT i) { return subCirParams_[i]; }

  /** @returns true if this param is found, false otherwise */
  bool hasParam(const std::string &instParamName) {
    for (MYINT i = 0; i < (MYINT)subCirParams_.size(); i++)
      if (subCirParams_[i]->getName() == instParamName)
        return true;
    return false;
  }

  /** method for the data structure traversal*/
  virtual void Accept(IP_NetlistVisitBase &visitor) { visitor.Visit( (IP_SubcirDef &)(*this)); }

  /*
  std::unordered_map< std::string , IP_Param*>&      getLoalParamsMap() { return localParams_; }
  std::unordered_map< std::string , IP_Model*>&      getLocalModelMap() { return localModels_; }
  std::unordered_map< std::string , IP_Function*>&   getLocalFunctionsMap() { return localFunctions_;}
  std::unordered_map< std::string , IP_SubcirDef*>&  getLocalSubcktMap()    { return subcircuitDefs_; }*/

private:

  /** flag to signal if this subcircuit is top level */
  bool isTopLevel_;

  /** the name of the defined subcircuit */
  std::string subCirName_;

  /** this is the body of the subcircuit */
  boost::shared_ptr<IP_NetlistStructSequence> subCirBody_;

  /** array with the parameters of the subcircuit definition */
  std::vector < boost::shared_ptr<IP_Param> > subCirParams_;

  /** these tokens were recognized as ports by the parser */
  std::vector < IP_Token > subCircPorts_;

/* DUE TO CONDITIONED NETLISTING THESE CANNOT BE HERE !!!!
  // ==== these informations are set after the initial netlist parser ====
  /// local parameter (not those from the subcircuit header)
  std::unordered_map< std::string , IP_Param*> localParams_;
  /// list of local models
  std::unordered_map< std::string , IP_Model*> localModels_;
  /// list of local functions
  std::unordered_map< std::string , IP_Function*> localFunctions_;
  /// on this level pointers to all subcircuit definitions
  std::unordered_map< std::string , IP_SubcirDef*> subcircuitDefs_;*/
};

#endif /* IP_SUBCIRDEF_HPP_ */
