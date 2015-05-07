/*
 * IndependentVoltageSource.cpp
 *
 *  Created on: Sep 12, 2013
 *      Author: benk
 */

#include "IndependentVoltageSource.hpp"
#include "AN_Analysis.hpp"
#include "AN_SimulationControllers.hpp"

IndependentVoltageSource::IndependentVoltageSource(MYINT p1, MYINT p2, SINSource* sinsource,
    std::string name, MYREAL mFact)
:Device(p1,p2,name), sourceType_(IndepVolt_SIN_Type), dcValue_(0.0)
{
  this->setMFactor(mFact);
  sinsource_ = *sinsource;
  sinsource_.sinfreq_ = sinsource->sinfreq_;
  sinsource_.sinphase_ = sinsource->sinphase_;
  sinsource_.sintd_ = sinsource->sintd_;
  sinsource_.sintheta_ = sinsource->sintheta_;
  sinsource_.sinva_ = sinsource->sinva_;
  sinsource_.sinvo_ = sinsource->sinvo_;
  pwlIndex_ = 0;
  currentID_.resize(1,-1);
}

IndependentVoltageSource::IndependentVoltageSource(MYINT p1, MYINT p2, PWLSource* pwlSource,
    std::string name, MYREAL mFact)
:Device(p1,p2,name), sourceType_(IndepVolt_PWL_Type), dcValue_(0.0)
{
  this->setMFactor(mFact);
  pwlsource_ = *pwlSource;
  pwlsource_.nrPoints_ = pwlSource->nrPoints_;
  pwlsource_.repeatTime_ = pwlSource->repeatTime_;
  pwlsource_.startTime_ = pwlSource->startTime_;
  pwlsource_.timeStamps_ = pwlSource->timeStamps_;
  pwlsource_.voltageValues_ = pwlSource->voltageValues_;
  pwlsource_.updateConstruct();
  pwlIndex_ = 0;
  currentID_.resize(1,-1);
}

IndependentVoltageSource::IndependentVoltageSource(MYINT p1, MYINT p2, MYREAL dcValue,
    std::string name, MYREAL mFact)
:Device(p1,p2,name), sourceType_(IndepVolt_DC_Type), dcValue_(dcValue)
{
  this->setMFactor(mFact);
  pwlIndex_ = 0;
  currentID_.resize(1,-1);
}

void IndependentVoltageSource::getAnalysisStamp(
    std::vector<MYINT>& stampColumn,
    std::vector<MYINT>& stampRow,
    std::vector<MatrixEntryType>& entryType,
    std::vector<MYREAL>& potentialEntryValues,
    MYINT &nrStampsElements,
    const AN_Analysis *analysis) const
{
  // return the local element stamp
  nrStampsElements = 4;
  if (nrStampsElements > (MYINT)stampColumn.size()) stampColumn.resize(nrStampsElements, -1);
  if (nrStampsElements > (MYINT)stampRow.size()) stampRow.resize(nrStampsElements, -1);
  if (nrStampsElements > (MYINT)entryType.size()) entryType.resize(nrStampsElements);
  if (nrStampsElements > (MYINT)potentialEntryValues.size()) potentialEntryValues.resize(nrStampsElements, 0.0);
  stampColumn[0] = 0; stampRow[0] = 2; entryType[0] = STATIC_ONLY; potentialEntryValues[0] = 1.0;
  stampColumn[1] = 1; stampRow[1] = 2; entryType[1] = STATIC_ONLY; potentialEntryValues[1] = 1.0;
  stampColumn[2] = 2; stampRow[2] = 0; entryType[2] = STATIC_ONLY; potentialEntryValues[2] = 1.0;
  stampColumn[3] = 2; stampRow[3] = 1; entryType[3] = STATIC_ONLY; potentialEntryValues[3] = 1.0;
}


void IndependentVoltageSource::getAnalysisStampRHS(
     std::vector<MYINT>& stampColumn,
     std::vector<MatrixEntryType>& entryType,
     std::vector<MYREAL>& potentialEntryValues,
     MYINT &nrRHSStampsElements,
     const AN_Analysis *analysis) const
{
  // return the local element stamp
  nrRHSStampsElements = 3;
  if (nrRHSStampsElements > (MYINT)stampColumn.size()) stampColumn.resize(nrRHSStampsElements, -1);
  if (nrRHSStampsElements > (MYINT)entryType.size()) entryType.resize(nrRHSStampsElements);
  if (nrRHSStampsElements > (MYINT)potentialEntryValues.size()) potentialEntryValues.resize(nrRHSStampsElements, 0.0);
  stampColumn[0] = 0; entryType[0] = STATIC_ONLY; potentialEntryValues[0] = 1.0;
  stampColumn[1] = 1; entryType[1] = STATIC_ONLY; potentialEntryValues[1] = 1.0;
  stampColumn[2] = 2; entryType[2] = STATIC_ONLY; potentialEntryValues[2] = 1.0;
}


void IndependentVoltageSource::evalDevice(
    // ---------- INPUTS --------
    const AN_Analysis *analysis,
    AN_SimulationControllers* simulationController,
    std::vector<MYREAL> &localVars,
    MYINT partiID,
    // ----------- OUTPUTS ----------
    MYINT &nrMatrixStamp,
    MYINT &nrRHSStamp,
    std::vector<MYREAL> &matrixValues,
    std::vector<MYREAL> &rhsValues,
    MYINT &flags
    )
{
  // this is the same stamp for DC and for TRAN analysis
  MYREAL mfact = (MYREAL)this->getMFactor();
  MYREAL pwlDt = 0.0;
  nrRHSStamp = 3;
  nrMatrixStamp = 4;
  matrixValues[0] =  1.0 * mfact;
  matrixValues[1] = -1.0 * mfact;
  matrixValues[2] =  1.0 * mfact;
  matrixValues[3] = -1.0 * mfact;
  rhsValues[0] =  localVars[2] * mfact;
  rhsValues[1] = -localVars[2] * mfact;
  rhsValues[2] = (localVars[0] - localVars[1]) * mfact;

  //  get the actual DC voltage
  if (sourceType_ == IndepVolt_DC_Type)
    { // add to the right hand side vector
    }
  else if (sourceType_ == IndepVolt_SIN_Type)
    {
      MYREAL atime = simulationController->getTime();
      dcValue_ =  sinsource_.evalSINSource(atime);
    }
  else { //sourceType_ == IndepVolt_PWL_Type
      MYREAL atime = simulationController->getTime();
      dcValue_ =  pwlsource_.evalPWLSource(atime, this->pwlIndex_, pwlDt);
    }
  rhsValues[2] += -dcValue_ * mfact;
}

void IndependentVoltageSource::tranPostProcess(
    AN_SimulationControllers* simulationController,
    MYREAL actualTime, MYREAL &proposedDT,
    MYINT &flags,MYREAL &startDT)
{
  // we have breakpoints with PWLs
  if (this->sourceType_ == IndepVolt_PWL_Type)
  {
 
 /* === HERE STARTS THE CODE OF ASSIGNMENT: 3 ==== */ 
 
 /* === HERE ENDS THE CODE OF ASSIGNMENT: 3 ==== */ 
  }

  // there could be only one potential break point with SIN VSources
  if (this->sourceType_ == IndepVolt_SIN_Type)
  {
      MYINT  verb = 0;
      SIM_PRINT_L5( verb , "tranPostProcess d=" << this->sinsource_.sintd_ << " t=" << actualTime);
      if ((this->sinsource_.sintd_ > (actualTime+SIM_ABS_TIME_STEP_LIMIT)) && (this->sinsource_.sintd_ < actualTime+proposedDT)){
          // this will be the next time step after the break point
          startDT = SIMMIN( startDT , 1.0/(1000.0*this->sinsource_.sinfreq_) );
          // this is the time step until the breakpoint
          proposedDT = SIMMIN( proposedDT , this->sinsource_.sintd_ - actualTime);
          // this means breakpoint
          DeviceInterfaceFlags flag = DEVICE_BREAKPOINT;
          flags = flags | flag;
          //
          SIM_PRINT_L5( verb , "tranPostProcess  proposedDT="<<proposedDT << " startDT=" << startDT);
      }
  }
}
