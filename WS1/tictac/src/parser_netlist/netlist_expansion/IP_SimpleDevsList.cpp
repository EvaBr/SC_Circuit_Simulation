/*
 * IP_SimpleDevsList.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: benk
 */

#include "IP_SimpleDevsList.hpp"


#include "Resistor.hpp"
#include "Capacitor.hpp"
#include "Inductor.hpp"

#include "IndependentVoltageSource.hpp"


// ================ RESISTOR =================
IP_MatchRes::IP_MatchRes():IP_DeviceMatchinRule(){
  devType_ = RES_TYPE;   nrPinns_ = 2;  deviceName_="Simple Resistor";

  optionalPinNames_.push_back("np");  optionalPinNames_.push_back("nn");
  pinFlags_.push_back(0);             pinFlags_.push_back(0);
  // TODO: at the moment no nodel is allowed
  // default value
  ConstExpressionValue def; def.valueType_ = REAL;
  paramNames_.push_back("r");    paramTypes_.push_back(REAL); def.dval_ = 1E+3; paramDefaultValue_.push_back(def);

}


Device* IP_MatchRes::createDevice( std::string &instanceName, std::vector< MYINT > &pinns,
      std::vector< std::string > &paramNames, std::vector< ConstExpressionValue > &paramValues,
      std::vector< MYINT > &paramFlags,
      std::vector< boost::shared_ptr<CS_ExpressionBasis> > &MNAExprs, MYINT mfact ) const
{
  // simply create a resistor
  return (new Resistor(pinns[0], pinns[1], paramValues[0].dval_, (MYREAL)mfact, instanceName ));
}


// ==================== CAPACITOR ================
IP_MatchCap::IP_MatchCap():IP_DeviceMatchinRule(){
  devType_ = CAP_TYPE;   nrPinns_ = 2; deviceName_="Simple Capacitor";

  optionalPinNames_.push_back("np");  optionalPinNames_.push_back("nn");
  pinFlags_.push_back(0);             pinFlags_.push_back(0);
  // TODO: at the moment no nodel is allowed
  // default value
  ConstExpressionValue def; def.valueType_ = REAL;
  paramNames_.push_back("c");     paramTypes_.push_back(REAL); def.dval_ = 1E-9; paramDefaultValue_.push_back(def);
}


Device* IP_MatchCap::createDevice( std::string &instanceName, std::vector< MYINT > &pinns,
    std::vector< std::string > &paramNames, std::vector< ConstExpressionValue > &paramValues,
    std::vector< MYINT > &paramFlags,
    std::vector< boost::shared_ptr<CS_ExpressionBasis> > &MNAExprs, MYINT mfact ) const
{
  // simply create a capacitor
  return (new Capacitor(pinns[0], pinns[1], paramValues[0].dval_, (MYREAL)mfact, instanceName ));
}


// ==================== INDUCTOR ================
IP_MatchInd::IP_MatchInd():IP_DeviceMatchinRule(){
  devType_ = IND_TYPE;   nrPinns_ = 2; deviceName_="Simple Inductor";

  optionalPinNames_.push_back("np");  optionalPinNames_.push_back("nn");
  pinFlags_.push_back(0);             pinFlags_.push_back(0);
  // TODO: at the moment no nodel is allowed
  // default value
  ConstExpressionValue def; def.valueType_ = REAL;
  paramNames_.push_back("l");    paramTypes_.push_back(REAL); def.dval_ = 1E-9; paramDefaultValue_.push_back(def);
}

Device* IP_MatchInd::createDevice( std::string &instanceName, std::vector< MYINT > &pinns,
    std::vector< std::string > &paramNames, std::vector< ConstExpressionValue > &paramValues,
    std::vector< MYINT > &paramFlags,
    std::vector< boost::shared_ptr<CS_ExpressionBasis> > &MNAExprs, MYINT mfact ) const
{
  // simply create an inductor
  return (new Inductor(pinns[0], pinns[1], paramValues[0].dval_, (MYREAL)mfact, instanceName ));
}


// ==================== DC source  ================
IP_MatchVDC::IP_MatchVDC():IP_DeviceMatchinRule(){
  devType_ = VELEM_TYPE;   nrPinns_ = 2; deviceName_="V DC";

  optionalPinNames_.push_back("np");  optionalPinNames_.push_back("nn");
  pinFlags_.push_back(0);             pinFlags_.push_back(0);
  // default value
  ConstExpressionValue def; def.valueType_ = REAL;
  paramNames_.push_back("dc");   paramTypes_.push_back(REAL); def.dval_ = 0.0; paramDefaultValue_.push_back(def);
}

Device* IP_MatchVDC::createDevice( std::string &instanceName, std::vector< MYINT > &pinns,
    std::vector< std::string > &paramNames, std::vector< ConstExpressionValue > &paramValues, std::vector< MYINT > &paramFlags,
    std::vector< boost::shared_ptr<CS_ExpressionBasis>  > &MNAExprs, MYINT mfact) const
{
  return (new IndependentVoltageSource(pinns[0], pinns[1], paramValues[0].dval_, instanceName , (MYREAL)mfact));
}

// ============= SIN Source ================
IP_MatchVSIN::IP_MatchVSIN():IP_DeviceMatchinRule(){
  devType_ = VELEM_TYPE;   nrPinns_ = 2; deviceName_="V SIN";

  optionalPinNames_.push_back("np");  optionalPinNames_.push_back("nn");
  pinFlags_.push_back(0);             pinFlags_.push_back(0);

  moduleName_ = "sin";
  // default value
  ConstExpressionValue def; def.valueType_ = REAL;
  paramNames_.push_back("sinvo");       paramTypes_.push_back(REAL); def.dval_ = 0.0;  paramDefaultValue_.push_back(def);
  paramNames_.push_back("sinva");       paramTypes_.push_back(REAL); def.dval_ = 1.0;  paramDefaultValue_.push_back(def);
  paramNames_.push_back("sinfreq");     paramTypes_.push_back(REAL); def.dval_ = 1E-3; paramDefaultValue_.push_back(def);
  paramNames_.push_back("sintd");       paramTypes_.push_back(REAL); def.dval_ = 0.0;  paramDefaultValue_.push_back(def);
  paramNames_.push_back("sintheta");    paramTypes_.push_back(REAL); def.dval_ = 0.0;  paramDefaultValue_.push_back(def);
  paramNames_.push_back("sinphase");    paramTypes_.push_back(REAL); def.dval_ = 0.0;  paramDefaultValue_.push_back(def);
}
Device* IP_MatchVSIN::createDevice(std::string &instanceName,
    std::vector< MYINT > &pinns, std::vector< std::string > &paramNames,
    std::vector< ConstExpressionValue > &paramValues, std::vector< MYINT > &paramFlags,
    std::vector< boost::shared_ptr<CS_ExpressionBasis>  > &MNAExprs, MYINT mfact ) const
{
  SINSource sins;
  sins.getDefaultValues();
  sins.sinvo_ = paramValues[0].dval_;
  sins.sinva_ = paramValues[1].dval_;
  sins.sinfreq_ = paramValues[2].dval_;
  sins.sintd_ = paramValues[3].dval_;
  sins.sintheta_ = paramValues[4].dval_;
  sins.sinphase_ = paramValues[5].dval_;

  return (new IndependentVoltageSource(pinns[0], pinns[1], &sins, instanceName , (MYREAL)mfact));
}

// ================= PULSE source =================
IP_MatchVPULSE::IP_MatchVPULSE():IP_DeviceMatchinRule(){
  devType_ = VELEM_TYPE;   nrPinns_ = 2; deviceName_="V PULSE";

  optionalPinNames_.push_back("np");  optionalPinNames_.push_back("nn");
  pinFlags_.push_back(0);             pinFlags_.push_back(0);

  moduleName_ = "pulse";
  // default value
  ConstExpressionValue def; def.valueType_ = REAL; // TODO: here we could already use information from the .TRAN record !!!
  paramNames_.push_back("pulsev1");   paramTypes_.push_back(REAL); def.dval_ = 0.0;   paramDefaultValue_.push_back(def);
  paramNames_.push_back("pulsev2");   paramTypes_.push_back(REAL); def.dval_ = 1.0;   paramDefaultValue_.push_back(def);
  paramNames_.push_back("pulsetd");   paramTypes_.push_back(REAL); def.dval_ = 0.0;   paramDefaultValue_.push_back(def);
  paramNames_.push_back("pulsetr");   paramTypes_.push_back(REAL); def.dval_ = 1E-12; paramDefaultValue_.push_back(def);
  paramNames_.push_back("pulsetf");   paramTypes_.push_back(REAL); def.dval_ = 1E-12;   paramDefaultValue_.push_back(def);
  paramNames_.push_back("pulsepw");   paramTypes_.push_back(REAL); def.dval_ = 1E-6;   paramDefaultValue_.push_back(def);
  paramNames_.push_back("pulseper");   paramTypes_.push_back(REAL); def.dval_ = 1E-6+2*1E-12;   paramDefaultValue_.push_back(def);
}
Device* IP_MatchVPULSE::createDevice( std::string &instanceName,
    std::vector< MYINT > &pinns, std::vector< std::string > &paramNames,
    std::vector< ConstExpressionValue > &paramValues, std::vector< MYINT > &paramFlags,
    std::vector< boost::shared_ptr<CS_ExpressionBasis> > &MNAExprs, MYINT mfact ) const
{
  PWLSource pwls;
  pwls.reserve(5);
  MYREAL pulseV, initV, delayTime, riseTime, fallTime, widthTime, repeatTime;
  MYINT ii = 0;
  initV = paramValues[0].dval_;
  pulseV = paramValues[1].dval_;
  delayTime = paramValues[2].dval_;
  riseTime = paramValues[3].dval_;
  fallTime = paramValues[4].dval_;
  widthTime = paramValues[5].dval_;
  repeatTime = paramValues[6].dval_;

  pwls.delayTime_ = delayTime;
  pwls.timeStamps_[ii] = 0.0;
  pwls.voltageValues_[ii] = initV; ii++;
  pwls.timeStamps_[ii] = riseTime;
  pwls.voltageValues_[ii] = pulseV; ii++;
  pwls.timeStamps_[ii] = riseTime+widthTime;
  pwls.voltageValues_[ii] = pulseV; ii++;
  pwls.timeStamps_[ii] = riseTime+widthTime+fallTime;
  pwls.voltageValues_[ii] = initV; ii++;
  pwls.timeStamps_[ii] = repeatTime;
  pwls.voltageValues_[ii] = initV; ii++;
  pwls.repeatTime_ = 0.0; // we repeat from time zero
  return (new IndependentVoltageSource(pinns[0], pinns[1], &pwls, instanceName , (MYREAL)mfact));
}


// ============= PWL Source ================
IP_MatchVPWL::IP_MatchVPWL()
{
  devType_ = VELEM_TYPE;   nrPinns_ = 2;  deviceName_="V PWL";

  optionalPinNames_.push_back("np");  optionalPinNames_.push_back("nn");
  pinFlags_.push_back(0);             pinFlags_.push_back(0);
  moduleName_ = "pwl";
  // default value
  ConstExpressionValue def; def.valueType_ = REAL_ARRAY;
  paramNames_.push_back("pwlsign");   paramTypes_.push_back(REAL_ARRAY); def.aval_ = std::vector<MYREAL>(0);   paramDefaultValue_.push_back(def);
  def.valueType_ = REAL;
  paramNames_.push_back("r");         paramTypes_.push_back(REAL); def.dval_ = 0.0;   paramDefaultValue_.push_back(def);
  paramNames_.push_back("td");   paramTypes_.push_back(REAL); def.dval_ = -1.0;   paramDefaultValue_.push_back(def); // BY DEFAULT NO REPEAT!
}

Device* IP_MatchVPWL::createDevice( std::string &instanceName,
    std::vector< MYINT > &pinns, std::vector< std::string > &paramNames,
    std::vector< ConstExpressionValue > &paramValues, std::vector< MYINT > &paramFlags,
    std::vector< boost::shared_ptr<CS_ExpressionBasis> > &MNAExprs, MYINT mfact ) const
{
  PWLSource pwls;
  pwls.reserve(paramValues[0].aval_.size()/2);
  MYINT ii = 0;
  for (ii = 0; ii < (MYINT)paramValues[0].aval_.size(); ii = ii + 2)
    {
      pwls.timeStamps_[ii/2] = paramValues[0].aval_[ii];
      pwls.voltageValues_[ii/2] = paramValues[0].aval_[ii+1];
    }

  pwls.delayTime_ = paramValues[1].dval_;
  pwls.repeatTime_ = paramValues[2].dval_;
  return (new IndependentVoltageSource(pinns[0], pinns[1], &pwls, instanceName , (MYREAL)mfact));
}
