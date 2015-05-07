/*
 * CS_Parameter.cpp
 *
 *  Created on: Nov 29, 2012
 *      Author: benk
 */

#include "CS_Parameter.hpp"
#include "CS_ModuleDeclaration.hpp"

CS_Parameter::CS_Parameter(std::string name, const CS_ModuleDeclaration* module,
    int startLine,  int startColumn,
    int paramIndex, VarType type,
    bool isLocal, CS_Range* range):
name_(name), module_(module), startLine_(startLine), startColumn_(startColumn),
index_(paramIndex), type_(type), isLocalParameter_(isLocal)
{
  size_ = 1;
  offset_ = 0;
  isReverse_ = false;
  range_ = range;

  // if range is given means this must be a localparam and and array
  if (range_)
    {
      range_ = range;
      int leftV = verilog_nearestInt( range_->getLeftExpr()->evalConst());
      int rightV = verilog_nearestInt( range_->getRightExpr()->evalConst());
      // swap values if left is grater
      if ( leftV > rightV ){
          isReverse_ = true;
          int tmpVal = leftV; leftV = rightV; rightV = tmpVal;
      }
      // left index can not be greater
      VERILOG_ERROR_COND_STOP(leftV > rightV, " In index range of the parameter '" << name_
          << "' left index can not be greater than the right limit", this);
      size_ = rightV - leftV + 1; // set this value correctly
      VERILOG_ERROR_COND_STOP(size_ <= 0, " In index range of the parameter '" << name_
          << "' size of the variable must be at least one but it is " << size_ , this);
      offset_ = leftV;
    }
}

