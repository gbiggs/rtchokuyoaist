/* RTC:HokuyoAIST
 *
 * Header file for the Ranger interface implementation.
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

#if !defined(RANGER_IMPL__)
#define RANGER_IMPL__

#include "idl/ranger.hh"

class RTCHokuyoAIST;

namespace RTCHokuyoAist
{

class RangerProvider
    : public virtual POA_RTC::Ranger,
    public virtual PortableServer::RefCountServantBase
{
    public:
        RangerProvider();
        virtual ~RangerProvider();

        void setup(RTCHokuyoAIST* owner) { owner_ = owner; }

        RTC::RangerGeometry* GetGeometry() throw (CORBA::SystemException);
        void Power(bool enable) throw (CORBA::SystemException);
        void EnableIntensities(bool enable) throw (CORBA::SystemException);
        RTC::RangerConfig GetConfig() throw (CORBA::SystemException);
        void SetConfig(RTC::RangerConfig const& new_config)
            throw (CORBA::SystemException);

    private:
        RTCHokuyoAIST* owner_;
}; // class RangerProvider

}; // namespace RTCHokuyoAist

#endif // RANGER_IMPL__

