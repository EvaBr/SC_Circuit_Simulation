/*
 * IP_ExpansionStack.hpp
 *
 *  Created on: Feb 28, 2015
 *      Author: benk
 */

#ifndef IP_EXPANSIONSTACK_HPP_
#define IP_EXPANSIONSTACK_HPP_

#include "sim_utils.hpp"
#include "Netlist.hpp"
#include "IP_SubcirDef.hpp"
#include "IP_NetlistVisitBase.hpp"
#include "CS_ExpressionBasis.hpp"
#include "CS_Parameter.hpp"

#include "IP_Param.hpp"
#include "IP_Function.hpp"
#include "IP_Model.hpp"

#include "IP_ExprConstEvaluator.hpp"


#define IP_HIERACHY_DELIMITER_STRING "."


/** class which should only contain the information of one X-call, the stack
 * of such information is used in the expansion process */
class ExpansionLevelInformation{
public:
  /// empty Ctor
  ExpansionLevelInformation(IP_SubcirDef *actSubCir, std::string xcallName);
  // empty Dtor
  virtual ~ExpansionLevelInformation() {;}

  /** the name of the X-call */
  std::string xcallName_;

  /// the actual subcircuit definition which gets expanded
  IP_SubcirDef *actSubCir_;

  const IP_SubcirDef* getActualSubcir() const { return actSubCir_; }
  IP_SubcirDef* getActualSubcir() { return actSubCir_; }

  // THESE FIELDS MUST BE HERE DUE TO CONDITIONED NETLISTING !!!

  /// the list of pins with this subcircuit was intantiated with
  std::unordered_map< std::string , MYINT> instantiationPinns_;
  /// the list of all internal pins which were created (subcircuit internal pinns)
  std::unordered_map< std::string , MYINT> internalPinns_;

  void addInstantPinn( const std::string &pinn , MYINT id);
  void addInternalPinn( const std::string &pinn , MYINT id);
  /** returns a posistive number (which is the pinn in the netlist)
   * if the given name is an instantiation pin, else negative < 0*/
  MYINT hasInstantiationPin(const std::string &pinName);
  /** returns a posistive number (which is the pinn in the netlist)
   * if the given name is a subcircuit internal pin, else negative < 0*/
  MYINT hasInternalPin(const std::string &pinName);

  /// the parameters with the subcircuit is instaniated with
  std::unordered_map< std::string , IP_Param*> instantiationParams_;
  /// we create for each parameter a reference, which can be used by building the expressions
  std::unordered_map< std::string , boost::shared_ptr<CS_Parameter> > instantParamsRefs_;
  /// the values of the instantiated
  std::unordered_map< std::string , ConstExpressionValue > instantiationParamValues_;
  void addInstantParam( IP_Param* param );
  void addInstantParamValue( std::string &param, ConstExpressionValue& epr );


  /// local parameter (not those from the subcircuit header)
  std::unordered_map< std::string , IP_Param*> localParams_;
  /// we create for each parameter a reference, which can be used by building the expressions
  std::unordered_map< std::string, boost::shared_ptr<CS_Parameter> > localParamsRefs_;
  /// the value of each local parameter
  std::unordered_map< std::string , boost::shared_ptr<CS_ExpressionBasis> > localParamValues_;

  void addLocalParam( IP_Param* param );
  void addLocalParamValue( std::string &param, boost::shared_ptr<CS_ExpressionBasis>& epr );

  /// list of local models
  std::unordered_map< std::string , IP_Model*> localModels_;
  /// here we store for each found model the values of theyr parameters
  std::unordered_map< std::string , std::vector<ConstExpressionValue> > modelParamValues_;

  void addModel(IP_Model* model);
  void addModelParamValues(IP_Model* model, std::vector< ConstExpressionValue >& values);
  /** looks for a model within the subcircuit, if not found than NULL*/
  IP_Model* getModel(std::string &modelName);
  /** returns the value of the parameters for the model (the model must be existent within the subcircuit )*/
  std::vector< ConstExpressionValue >& getModelParams(std::string &modelName, bool &error) ;
  // TODO: for models we should be evaluate their parameters staright-ahead after the
  // parameters are ready
  /// get the model according to its name, we should also integrate a type check as well !!! ???


  /// list of local functions
  std::unordered_map< std::string , IP_Function*> localFunctions_;
  void addFunctionDef(IP_Function* function) {
    localFunctions_.insert(std::pair<std::string,IP_Function*>(function->getName(), function));
  }

  /// on this level pointers to all subcircuit definitions
  // TODO: we want also to considet the number of pinns
  std::unordered_map< std::string , IP_SubcirDef*> subcircuitDefs_;
  void addSubcircuitDef(IP_SubcirDef* subcir) {
    subcircuitDefs_.insert(std::pair<std::string,IP_SubcirDef*>(subcir->getName(),subcir));
  }


  /// list of the elements which should be parsed and added to the netlist
  std::vector< IP_Element* > elementsToExpand_;
  void addElem(IP_Element* elem) { elementsToExpand_.push_back(elem); }
  std::vector< IP_Element* >& getElements() { return elementsToExpand_; }

  /// list of all X-calls (for which expansion should be applied recursively)
  std::vector< IP_XCall* > xCallsToExpand_;
  void addXCall(IP_XCall* xelem) { xCallsToExpand_.push_back(xelem); }
  std::vector< IP_XCall* >& getXCalls() { return xCallsToExpand_; }

  /** returns a subcircuit if it conatins, otherwise NULL
   * Number of pinns are not considered yet, now cound be any value */
  IP_SubcirDef* getSubcircuit(std::string &subcktName, MYINT nrPinns);
};


/** class containing all the expansion stack informations (multiple levels)*/
class ExpansionStack : public UT_SIMVerboseClass{
public:
  /// Ctor
  ExpansionStack();
  /// empty Dtor
  virtual ~ExpansionStack() {;}

  /// returns true if a parameter if found, and also sais if this parameter has already an initial value
  bool hasParameter(std::string &paramName, bool &hasInitValue,
      bool ignoreActualLevel=false) const;

  /// returns the parameter reference (needed in expressions)
  boost::shared_ptr<CS_Parameter> getParameterReference(std::string &paramName,
      bool ignoreActualLevel=false) const ;

  /// returns the initial value of a parameter (if there is none than ERROR)
  boost::shared_ptr<CS_ExpressionBasis> getParameterInitialValue(std::string &paramName,
      bool ignoreActualLevel=false) const;

  /// adds a global pinn name
  void addGlobalPinn(std::string globalPinName, MYINT ind);

  ExpansionLevelInformation& getExpansionLevel(int i) { return expansionStack_[i]; }
  const ExpansionLevelInformation& getExpansionLevel(int i) const { return expansionStack_[i]; }

  ExpansionLevelInformation& getExpansionLastLevel() { return expansionStack_.back(); }
  const ExpansionLevelInformation& getExpansionLastLevel() const { return expansionStack_.back(); }

  /// adds a new level
  void addNewLevel(IP_SubcirDef *actSubCir, const std::string &xcallName );

  /// removes last level
  void removeLastLevel();

  /// clear all the stack
  void clearAllStack() { expansionStack_.clear(); globalPinns_.clear(); listOfGndNames_.clear(); }


  bool isGnd(std::string pinName) const;
  /** the global pinss are initiated with -10 until they are referred somewhere in the circuit.
   * If a global pin is simply not found than this returns -1, otherwise -10 (or similar) when is found but is not
   * assigned yet. If found and is assigned, then returns the ID of the pin in the netlist */
  MYINT isGlobalPinn(std::string pinName) const;
  /** once a global pin is instantiated it should be assedned to a ID in the real netlist, this method does this. */
  void setGlobalPinnID(std::string pinName, MYINT id);

  /** returns the found subcircuit in the expansion stack, if not found than returns NULL*/
  IP_SubcirDef* getSubcircuit(std::string &subcktName, MYINT nrPinns);

  /** returns the hierarchical name in the current expanded context */
  std::string getHierarchicalName(std::string posStr);

private:

  /// returns true if a parameter if found, and also sais if this parameter has already an initial value
  bool foundParameter(std::string &paramName, bool &hasInitValue,
      boost::shared_ptr<CS_ExpressionBasis> &internalFoundParamValue,
      boost::shared_ptr<CS_Parameter>       &internalFoundParamRef,
      IP_Param*                             &internalFoundParam,
      bool                                  &internalFoundParamIsLocal,
      bool ignoreActualLevel=false ) const;

  /// vector representing the stack
  std::vector< ExpansionLevelInformation > expansionStack_;

  /** list of all global signals defined on the toplevel */
  std::unordered_map< std::string , MYINT> globalPinns_;
  /** list of all names eqvivalent to ground */
  std::unordered_map< std::string , MYINT> listOfGndNames_;

};

#endif /* IP_EXPANSIONSTACK_HPP_ */
