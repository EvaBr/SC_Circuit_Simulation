/*
 * IP_ExpansionStack.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: benk
 */

#include "IP_ExpansionStack.hpp"
#include "IP_ExpressionParser.hpp"

// =========================================================
// ======================== ExpansionStack =================
// =========================================================

/// Ctor
ExpansionStack::ExpansionStack() : UT_SIMVerboseClass("ExpansionStack")
{
  // the default names for ground nodes
  listOfGndNames_.insert( std::pair<std::string,MYINT>("0",0));
  listOfGndNames_.insert( std::pair<std::string,MYINT>("gnd",0));
  this->setVerb(0);
}

void ExpansionStack::addGlobalPinn(std::string globalPinName, MYINT ind)
{
  globalPinns_.insert( std::pair<std::string,MYINT>(globalPinName,ind));
}

bool ExpansionStack::foundParameter(std::string &paramName, bool &hasInitValue,
    boost::shared_ptr<CS_ExpressionBasis> &internalFoundParamValue,
    boost::shared_ptr<CS_Parameter>       &internalFoundParamRef,
    IP_Param*                             &internalFoundParam,
    bool                                  &internalFoundParamIsLocal,
    bool ignoreActualLevel ) const {
  MYINT startInd = (MYINT)expansionStack_.size() - 1, i;
  hasInitValue = false;
  // consider if the actual level should be ignored
  if (ignoreActualLevel)
    startInd--;
  // we go backwards in the stack (we want to pupport only this variant !!!)
  for (i = startInd; i >= 0; i--)
    {
      SIM_PRINT_L5(verb(), "ExpansionStack::foundParameter stack ind=" << i << " paramName=" << paramName );
      // ============ LOCAL PARAMS ===========
      // look first in the local params then in the X-call parameters
      auto initParamIt = expansionStack_[i].localParams_.find(paramName);
      if (initParamIt != expansionStack_[i].localParams_.end())
        { // local param found
          SIM_PRINT_L5(verb(), "ExpansionStack::foundParameter local param found");
          auto localParamValIt = expansionStack_[i].localParamValues_.find(paramName);
          auto localParamRefIt = expansionStack_[i].localParamsRefs_.find(paramName);
          internalFoundParam = initParamIt->second;
          internalFoundParamRef = localParamRefIt->second;
          internalFoundParamIsLocal = true;
          if (localParamValIt != expansionStack_[i].localParamValues_.end() )
            {
              internalFoundParamValue = localParamValIt->second;
              hasInitValue = true;
              SIM_PRINT_L5(verb(), "ExpansionStack::foundParameter local param value found " << internalFoundParamValue.get());
            }
          else
            SIM_PRINT_L5(verb(), "ExpansionStack::foundParameter local param value NOT found ");
          return true;
        }

      // ============ X-CALL PARAMS ===========
      auto localParamIt = expansionStack_[i].instantiationParams_.find(paramName);
      if (localParamIt != expansionStack_[i].instantiationParams_.end())
        { // instant param found
          SIM_PRINT_L5(verb(), "ExpansionStack::foundParameter instant param found");
          auto localParamValIt = expansionStack_[i].instantiationParamValues_.find(paramName);
          auto localParamRefIt = expansionStack_[i].instantParamsRefs_.find(paramName);
          internalFoundParam = localParamIt->second;
          internalFoundParamRef = localParamRefIt->second;
          internalFoundParamIsLocal = false;
          if (localParamValIt != expansionStack_[i].instantiationParamValues_.end() )
            {
              internalFoundParamValue = IP_ExpressionParser::getExprForConstantValue( localParamValIt->second );
              hasInitValue = true;
            }
          return true;
        }
    }
  return false;
}

bool ExpansionStack::hasParameter(std::string &paramName, bool &hasInitValue, bool ignoreActualLevel) const
{
  boost::shared_ptr<CS_ExpressionBasis> internalFoundParamValue;
  boost::shared_ptr<CS_Parameter>       internalFoundParamRef;
  IP_Param*                             internalFoundParam;
  bool                                  internalFoundParamIsLocal;
  return foundParameter(paramName, hasInitValue,
      internalFoundParamValue, internalFoundParamRef, internalFoundParam, internalFoundParamIsLocal);
}

boost::shared_ptr<CS_Parameter> ExpansionStack::getParameterReference(std::string &paramName, bool ignoreActualLevel) const
{
  boost::shared_ptr<CS_ExpressionBasis> internalFoundParamValue;
  boost::shared_ptr<CS_Parameter>       internalFoundParamRef;
  IP_Param*                             internalFoundParam;
  bool                                  internalFoundParamIsLocal;
  bool hasInitValue;
  bool hasParam = foundParameter(paramName, hasInitValue,
      internalFoundParamValue, internalFoundParamRef, internalFoundParam, internalFoundParamIsLocal);

  SIM_ERROR_COND_STOP( !hasParam, "Parameter not found " << paramName);
  return internalFoundParamRef;
}

boost::shared_ptr<CS_ExpressionBasis> ExpansionStack::getParameterInitialValue(std::string &paramName, bool ignoreActualLevel) const
{
  boost::shared_ptr<CS_ExpressionBasis> internalFoundParamValue;
  boost::shared_ptr<CS_Parameter>       internalFoundParamRef;
  IP_Param*                             internalFoundParam;
  bool                                  internalFoundParamIsLocal;
  bool hasInitValue;
  bool hasParam = foundParameter(paramName, hasInitValue,
      internalFoundParamValue, internalFoundParamRef, internalFoundParam, internalFoundParamIsLocal);
  SIM_PRINT_L5(verb(), "ExpansionStack::getParameterInitialValue " << hasParam << " " << hasInitValue << " p=" << internalFoundParamValue.get() );
  SIM_ERROR_COND_STOP( !hasParam || !hasInitValue || (internalFoundParamValue.get() == NULL),
      "Parameter's initial value not found " << paramName);
  return internalFoundParamValue;
}

bool ExpansionStack::isGnd(std::string pinName) const
{
  std::unordered_map< std::string , MYINT>::const_iterator
                 subIt = listOfGndNames_.find(pinName);
  if (subIt != listOfGndNames_.end())
    return true;
  else
    return false;
}
MYINT ExpansionStack::isGlobalPinn(std::string pinName) const {
  std::unordered_map< std::string , MYINT>::const_iterator
                 subIt = globalPinns_.find(pinName);
  if (subIt != globalPinns_.end())
    return subIt->second;
  else
    return -1;
}
void ExpansionStack::setGlobalPinnID(std::string pinName, MYINT id)
{
  std::unordered_map< std::string , MYINT>::const_iterator
                 subIt = globalPinns_.find(pinName);
  // if this is found then set the new value for this global pin
  if (subIt != globalPinns_.end())
    globalPinns_[pinName] = id;
}

void ExpansionStack::addNewLevel(IP_SubcirDef *actSubCir, const std::string &xcallName )
{
  // add a new level
  ExpansionLevelInformation newElem(actSubCir, xcallName);
  expansionStack_.push_back(newElem);
}

void ExpansionStack::removeLastLevel()
{
  // just remove the last element
  expansionStack_.pop_back();
}

IP_SubcirDef* ExpansionStack::getSubcircuit(std::string &subcktName, MYINT nrPinns)
{
  IP_SubcirDef* ret = NULL;
  for (MYINT i = (MYINT)expansionStack_.size()-1; i >= 0; i-- ) {
      ret = expansionStack_[i].getSubcircuit( subcktName, nrPinns);
      if (ret != NULL)
        return ret;
  }
  // return null
  return ret;
}
std::string ExpansionStack::getHierarchicalName(std::string posStr)
{
  std::string ret = "";
  // IMPORTANT, we start from "1" since "TOPLEVEL" should not be there
  for (MYINT i = 1; i < (MYINT)expansionStack_.size(); ++i ) {
      ret += expansionStack_[i].xcallName_ + IP_HIERACHY_DELIMITER_STRING;
  }
  if (posStr.size() > 0)
    ret += posStr;
  else
    ret.pop_back();
  // return the modified string
  return ret;
}

// =========================================================
// ======================== ExpansionLevelInformation=======
// =========================================================

ExpansionLevelInformation::ExpansionLevelInformation(
    IP_SubcirDef *actSubCir, std::string xcallName) {
  actSubCir_ = actSubCir;
  xcallName_ = xcallName;
}

IP_SubcirDef* ExpansionLevelInformation::getSubcircuit(std::string &subcktName, MYINT nrPinns) {
  // TODO: the number of pinns are not yet considered
  auto  subIt = subcircuitDefs_.find(subcktName);
  if (subIt != subcircuitDefs_.end())
    return subIt->second;
  else
    return NULL;
}

MYINT ExpansionLevelInformation::hasInstantiationPin(const std::string &pinName){
  auto subIt = instantiationPinns_.find(pinName);
  if (subIt == instantiationPinns_.end())
    return -2;
  else
    return subIt->second;
}

MYINT ExpansionLevelInformation::hasInternalPin(const std::string &pinName){
  auto  subIt = internalPinns_.find(pinName);
  if (subIt == internalPinns_.end())
    return -2;
  else
    return subIt->second;
}

IP_Model* ExpansionLevelInformation::getModel(std::string &modelName) {
  // TODO: the type of the model is not considered yet
  auto subIt = localModels_.find(modelName);
  if (subIt != localModels_.end())
    return subIt->second;
  else
    return NULL;
}

void ExpansionLevelInformation::addModel(IP_Model* model)
{
  localModels_.insert(std::pair<std::string, IP_Model*>(model->getName(), model));
}
void ExpansionLevelInformation::addModelParamValues(IP_Model* model, std::vector< ConstExpressionValue >& values)
{
  modelParamValues_.insert(std::pair<std::string, std::vector< ConstExpressionValue > >(model->getName(), values));
}

std::vector< ConstExpressionValue >& ExpansionLevelInformation::getModelParams(std::string &modelName, bool &error) {
  // TODO: the type of the model is not considered yet
  auto subIt = modelParamValues_.find(modelName);
  error = false;
  if (subIt == modelParamValues_.end())
    error = true;
  // return always this
  return subIt->second;
}

void ExpansionLevelInformation::addLocalParam( IP_Param* param )
{
  localParams_.insert(std::pair<std::string,IP_Param*>(param->getName(), param));
  localParamsRefs_.insert(std::pair<std::string, boost::shared_ptr<CS_Parameter>>(
      param->getName(), boost::shared_ptr<CS_Parameter>(new
          CS_Parameter(
            param->getName(), (const CS_ModuleDeclaration*)param->getToken(1).getFileNameDummyMod(),
            param->getToken(1).getColumnIndex(), param->getToken(1).getRowIndex(),
            localParamsRefs_.size() )
      )));
}

void ExpansionLevelInformation::addLocalParamValue( std::string &param, boost::shared_ptr<CS_ExpressionBasis>& epr )
{
  localParamValues_.insert( std::pair<std::string, boost::shared_ptr<CS_ExpressionBasis>>( param, epr) );
}

void ExpansionLevelInformation::addInstantParam( IP_Param* param )
{
  instantiationParams_.insert(std::pair<std::string,IP_Param*>(param->getName(), param));
  instantParamsRefs_.insert(std::pair<std::string, boost::shared_ptr<CS_Parameter>>(
      param->getName(), boost::shared_ptr<CS_Parameter>(new
          CS_Parameter(
            param->getName(), (const CS_ModuleDeclaration*)param->getToken(1).getFileNameDummyMod(),
            param->getToken(1).getColumnIndex(), param->getToken(1).getRowIndex(),
            localParamsRefs_.size() )
      )));
}
void ExpansionLevelInformation::addInstantParamValue( std::string &param, ConstExpressionValue& epr )
{
  instantiationParamValues_.insert( std::pair<std::string, ConstExpressionValue>( param, epr) );
}

void ExpansionLevelInformation::addInstantPinn( const std::string &pinn, MYINT id){
  instantiationPinns_.insert( std::pair<std::string,MYINT>(pinn,id) );
}
void ExpansionLevelInformation::addInternalPinn( const std::string &pinn, MYINT id){
  internalPinns_.insert( std::pair<std::string,MYINT>(pinn,id) );
}
