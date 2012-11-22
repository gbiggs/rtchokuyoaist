/* RTC:HokuyoAIST
 *
 * Ranger interface implementation.
 *
 * Copyright 2010-2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of HokuyoAIST.
 *
 * HokuyoAIST is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License,
 * or (at your option) any later version.
 *
 * HokuyoAIST is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with HokuyoAIST. If not, see
 * <http://www.gnu.org/licenses/>.
 */


#include <rtchokuyoaist/ranger_impl.h>
#include <rtchokuyoaist/rtc.h>

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


void RangerProvider::SetGeometry(RTC::RangerGeometry const& geom)
    throw(CORBA::SystemException)
{
    owner_->set_geometry(geom);
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

