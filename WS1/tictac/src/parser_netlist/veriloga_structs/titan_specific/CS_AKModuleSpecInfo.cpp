/*
 * CSAKModuleSpecInfo.cpp
 *
 *  Created on: Jan 14, 2013
 *      Author: benk
 */

#include "CS_AKModuleSpecInfo.hpp"

#include "CS_AnalogEventFunction.hpp"
#include "CS_ExpressionBasis.hpp"

CS_AKModuleSpecInfo::CS_AKModuleSpecInfo():
akVariables_(0), nrVars_(0), akBranches_(0), nrBranches_(0),
akEvaluations_(0), nrEvaluations_(0), akEventFunctions_(0),
nrEventFnuctions_(0), nrLimExpFuncs_(0), nrRealVars_(0), nrIntVars_(0),
moduleInitializations_(), coorespondingVariable_(), nrStaticFunctions_(0),
nrWrtCtrlCalls_(0), nrMonitors_(0), nrTmpStrinVars_(0),
nrArrayVars_For_Const_Arrays_(0), nrTables_(0), nrLaplaceOperators_(0)
{
  // init the lists
}

void CS_AKModuleSpecInfo::addNewVar()
{
  akVariables_.push_back(new AKVar());
  akVariables_[nrVars_]->globalVarIndex_ = nrVars_;
  akVariables_[nrVars_]->kindInString_ = "";
  akVariables_[nrVars_]->restAKGenerate_ = "";
  nrVars_++;
}

int CS_AKModuleSpecInfo::hasVariable(std::string varName) const
{
  int ret_val = -1;
  for (int i = 0 ; i < this->nrVars_ ; i++ )
    // if the names maths then return the index
    if (varName == this->akVariables_[i]->name_)
      ret_val = i;
  // return the index, -1 means not found >=0 if found
  return ret_val;
}

void CS_AKModuleSpecInfo::addNewBranch(int p1, int p2)
{
   akBranches_.push_back(new AKBranch());
   akBranches_[nrBranches_]->brI1_ = p1;
   akBranches_[nrBranches_]->brI2_ = p2;
   akBranches_[nrBranches_]->evalIndx_ = -1;
   akBranches_[nrBranches_]->evalIdxRhs_ = -1;
   akBranches_[nrBranches_]->branchContributionsTypes_ = 0;
   akBranches_[nrBranches_]->branchType_ = VAMSCONTRIB_NONE;
   akBranches_[nrBranches_]->branchTypeString_ = "VAMSCONTRIB_NONE";
   akBranches_[nrBranches_]->indirExprKind_ = "VAMSEXPR_GENERAL";
   nrBranches_++;
}


void CS_AKModuleSpecInfo::addNewEval()
{
   akEvaluations_.push_back(new AKEvaluation());
   akEvaluations_[nrEvaluations_]->initStruct();
   nrEvaluations_++;
}


bool CS_AKModuleSpecInfo::hasBranch(int p1, int p2, int &index, bool &reversed)
{
  AKBranch tmp;
  bool rev = false;
  tmp.brI1_ = p1;
  tmp.brI2_ = p2;
  // for all existing branches
  for (int i = 0; i < nrBranches_; i++) {
      // test for this branch if this is equal
      if ( akBranches_[i]->isEqual(&tmp, rev) ) {
          // match found
          index = i;
          reversed = rev;
          return true;
      }
  }
  // no match found return false
  return false;
}


int CS_AKModuleSpecInfo::hasSFCorrespondent(const std::string& corr )
{
  int ret_tmp = -1;
  for (int i = 0 ; i < nrStaticFunctions_; i++)
    { // test if we have already a match
      if (corr == coorespondingVariable_[i])
        ret_tmp = i;
    }
  return ret_tmp;
}

void CS_AKModuleSpecInfo::addSF( std::string corr, std::string initCode){
  int found_Int = hasSFCorrespondent(corr);
  if (found_Int < 0)
    { // not existent in the list, so add it
      nrStaticFunctions_++;
      coorespondingVariable_.push_back(corr);
      moduleInitializations_.push_back(initCode);
    }
  else
    { // existent in the list, issue warning, update the init code
      VERILOG_WARNING(" SF already existent '" << corr <<"', updating with code '"
          << initCode << "'", 0);
      moduleInitializations_[found_Int] = initCode;
    }
}
