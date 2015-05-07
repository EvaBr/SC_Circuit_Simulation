/*
 * IP_NetlistStructSequence.hpp
 *
 *  Created on: Jan 29, 2015
 *      Author: benk
 */

#ifndef IP_NETLISTSTRUCTSEQUENCE_HPP_
#define IP_NETLISTSTRUCTSEQUENCE_HPP_

#include "IP_NetlistStructBase.hpp"

/** this a construct to represent a sequence of netlist constructs
 * This is just a "helping" construct to represent sequences of constructs */
class IP_NetlistStructSequence : public IP_NetlistStructBase
{
public:

  /** Ctor with tokens,
   * IMPORTANT, the tokens play here only a dummy role */
  IP_NetlistStructSequence(std::vector<IP_Token> &tokens);

  /** emtpy Dtor */
  virtual ~IP_NetlistStructSequence() {;}

  /** adds a new struct to the sequence */
  void addNetlistStruct(IP_NetlistStructBase *newStruct) {
    sequence_.push_back( boost::shared_ptr<IP_NetlistStructBase>(newStruct) );
  }

  /** adds a new struct to the sequence (adding a smart pointer)*/
  void addNetlistStruct(boost::shared_ptr<IP_NetlistStructBase> newStruct) {
    sequence_.push_back( newStruct );
  }

  /** @returns the length of the sequence */
  MYINT getSequenceLength() const { return (MYINT)sequence_.size(); }

  /** @returns the smart pointer reference to the struct element  */
  boost::shared_ptr<IP_NetlistStructBase>& getNetlistStruct(int i) { return sequence_[i]; }
  const boost::shared_ptr<IP_NetlistStructBase>& getNetlistStruct(int i) const { return sequence_[i]; }

  /** method for the data structure traversal*/
  virtual void Accept(IP_NetlistVisitBase &visitor) { visitor.Visit( (IP_NetlistStructSequence &)(*this)); }

private:

  /** the sequence of constructs */
  std::vector< boost::shared_ptr<IP_NetlistStructBase> > sequence_;

};

#endif /* IP_NETLISTSTRUCTSEQUENCE_HPP_ */
