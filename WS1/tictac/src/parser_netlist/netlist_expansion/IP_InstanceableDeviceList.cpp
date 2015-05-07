/*
 * IP_InstanceableDeviceList.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: benk
 */

#include "IP_InstanceableDeviceList.hpp"

#include "IP_SimpleDevsList.hpp"

IP_InstanceableDeviceList::IP_InstanceableDeviceList()
{
  // TODO Auto-generated constructor stub

  // each device MUST REGISTER itself !!!
  MYINT i = 0;
  devType_.push_back(R_ELEM);
  devMatchRule_.resize(++i);
  devMatchRule_[i-1].push_back(boost::shared_ptr< IP_DeviceMatchinRule >(new IP_MatchRes() ));

  devType_.push_back(C_ELEM);
  devMatchRule_.resize(++i);
  devMatchRule_[i-1].push_back(boost::shared_ptr< IP_DeviceMatchinRule >( new IP_MatchCap() ));

  devType_.push_back(L_ELEM);
  devMatchRule_.resize(++i);
  devMatchRule_[i-1].push_back(boost::shared_ptr< IP_DeviceMatchinRule >( new IP_MatchInd() ));

  devType_.push_back(V_ELEM);
  devMatchRule_.resize(++i);
  devMatchRule_[i-1].push_back(boost::shared_ptr< IP_DeviceMatchinRule >( new IP_MatchVDC() ));
  devMatchRule_[i-1].push_back(boost::shared_ptr< IP_DeviceMatchinRule >( new IP_MatchVSIN() ));
  devMatchRule_[i-1].push_back(boost::shared_ptr< IP_DeviceMatchinRule >( new IP_MatchVPULSE() ));
  devMatchRule_[i-1].push_back(boost::shared_ptr< IP_DeviceMatchinRule >( new IP_MatchVPWL() ));
}

const std::vector< boost::shared_ptr<IP_DeviceMatchinRule> >&
IP_InstanceableDeviceList::getListOfDeviceMatchings(IP_ElemPreType elemType)
{
  // look for an array
  for (MYINT i = 0; i < (MYINT)devType_.size(); i++)
    {
      if (devType_[i] == elemType)
        return devMatchRule_[i];
    }
  // empty list when there is o match !!!
  return emptyList_;
}
