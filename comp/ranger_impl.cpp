/* RTC:HokuyoAist
 *
 * Copyright (C) 2010
 *     Geoffrey Biggs
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 * Licensed under the Eclipse Public License -v 1.0 (EPL)
 * http://www.opensource.org/licenses/eclipse-1.0.txt
 *
 * Service implementation class.
 */


#include "comp/ranger_impl.h"
#include "comp/rtc.h"

using namespace RTCHokuyoAist;

//////////////////////////////////////////////////////////////////////////////
// Ranger provider class

RangerProvider::RangerProvider()
    : owner_(0)
{
}


RangerProvider::~RangerProvider()
{
}


RTC::RangerGeometry* RangerProvider::GetGeometry()
    throw(CORBA::SystemException)
{
    RTC::RangerGeometry* result = new RTC::RangerGeometry;

    *result = owner_->get_geometry();
    return result;
}


void RangerProvider::Power(bool enable)
    throw(CORBA::SystemException)
{
    owner_->set_power(enable);
}


void RangerProvider::EnableIntensities(bool enable)
    throw(CORBA::SystemException)
{
    owner_->enable_intensity_data(enable);
}


RTC::RangerConfig RangerProvider::GetConfig()
    throw(CORBA::SystemException)
{
    RTC::RangerConfig result;
    result = owner_->get_config();
    return result;
}


void RangerProvider::SetConfig(RTC::RangerConfig const& new_config)
    throw(CORBA::SystemException)
{
    owner_->set_config(new_config);
}

