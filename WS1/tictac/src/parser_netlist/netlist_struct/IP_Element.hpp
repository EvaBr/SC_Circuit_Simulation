/*
 * IP_Element.hpp
 *
 *  Created on: Jan 19, 2015
 *      Author: benk
 */

#ifndef IP_ELEMENT_HPP_
#define IP_ELEMENT_HPP_

#include "IP_NetlistStructBase.hpp"

/** enum type for the rough type calssification of the elements */
typedef enum {
  R_ELEM, ///< resistor
  C_ELEM, ///< capacitor
  L_ELEM, ///< inductance
  V_ELEM,  ///< independent voltage source
  E_ELEM,  ///< Behavioral controlled voltage sources
  G_ELEM,  ///< Behavioral controlled current sources
  D_ELEM,  ///< diodes
  M_ELEM,  ///< MOSFETs
  Q_ELEM,  ///< bipolar transitors
  J_ELEM,  ///< JFET

  K_ELEM,  ///< K-elem magnetic core ...
  U_ELEM,  ///< U elements for digitial (SIMetrix stuff), ot others

  ERR_ELEM ///< error element, just to signal error
} IP_ElemPreType;

/** class to model the basis of all elements which might represent a device,
 * or subcircuit declaration/call in the netlist */
class IP_Element : public IP_NetlistStructBase
{
public:

  /** Ctor */
  IP_Element(std::vector<IP_Token> &tokens, IP_ElemPreType elemType);

  /** empty Dtor */
  virtual ~IP_Element() {;}

  /** return the instantiation name */
  const std::string& getInstName() const { return this->getToken(0).getValue(); }

  /** @returns the type of the element */
  IP_ElemPreType getElemType() const { return elemType_; }

  /** method for the data structure traversal*/
  virtual void Accept(IP_NetlistVisitBase &visitor) { visitor.Visit( (IP_Element &)(*this)); }

private:

  /** the element type , this is just the rough type*/
  IP_ElemPreType elemType_;

};

#endif /* IP_ELEMENT_HPP_ */
