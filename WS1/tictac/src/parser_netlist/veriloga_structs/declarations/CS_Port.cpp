/*
 * CS_PORT_HPP_.cpp
 *
 *  Created on: Nov 30, 2012
 *      Author: benk
 */

#include "CS_Port.hpp"

#include <math.h>

CS_Port::CS_Port(const CS_ModuleDeclaration* module,
    int startLine, int startColumn,
    std::string name,
    int portIndex, CS_Range* range, PortType ty, bool isExternal):
    module_(module), startLine_(startLine), startColumn_(startColumn),
    isReverseNumbered_(false),
    name_(name), portIndex_(portIndex), range_(range),
    portType_(ty), isExternal_(isExternal)
{
  // nothing to do here
  VERILOG_PRINT_L3( verb(), "PN:" << name_ );
  if (range_) {
      int leftV = verilog_nearestInt( range_->getLeftExpr()->evalConst() );
      int rightV = verilog_nearestInt( range_->getRightExpr()->evalConst() );
      VERILOG_PRINT_L3( verb(), "leftV = " << range_->getLeftExpr()->evalConst() << " rightV=" << range_->getRightExpr()->evalConst());
      // swap values if left is grater
      if ( leftV > rightV ){
          isReverseNumbered_ = true;
          int tmpVal = leftV; leftV = rightV; rightV = tmpVal;
      }
      // left index can not be greater
      VERILOG_ERROR_COND_STOP(leftV > rightV, "In index range of the port '" << name_
          << "' left index can not be greater than the right limit", this);
      size_ = rightV - leftV + 1; // set this value correctly
      VERILOG_ERROR_COND_STOP(size_ <= 0, "In index range of the port '" << name_
          << "' size of the port must be at least one but it is " << size_ , this);
      offset_ = leftV;
      VERILOG_PRINT_L3( verb(), "size_ = " << size_ );
      VERILOG_PRINT_L3( verb(), "offset_ = " << offset_ );
      isArray_ = true;
  }
  else {
      size_ = 1;
      offset_ = 0;
      isArray_ = false;
  }
}


void CS_Port::setRange(CS_Range* range)
{
  range_ = range;
  // update the size of the port
  if (range_)
    {
      int leftV = (int) verilog_nearestInt( range_->getLeftExpr()->evalConst() );
      int rightV = (int)verilog_nearestInt( range_->getRightExpr()->evalConst() );
      // swap values if left is grater
      if ( leftV > rightV ){
          isReverseNumbered_ = true;
          int tmpVal = leftV; leftV = rightV; rightV = tmpVal;
      }
      // left index can not be greater
      VERILOG_ERROR_COND_STOP(leftV > rightV, "In index range of the port '" << name_
          << "' left index can not be greater than the right limit", this);
      size_ = rightV - leftV + 1; // set this value correctly
      VERILOG_ERROR_COND_STOP(size_ <= 0, "In index range of the port '" << name_
          << "' size of the port must be at least one but it is " << size_, this);
      offset_ = leftV;
      isArray_ = true;
    }
}
