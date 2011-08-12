/* RTC:HokuyoAIST
 *
 * Header file for the HokuyoAIST interface implementation.
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

#if !defined(HOKUYOAIST_IMPL__)
#define HOKUYOAIST_IMPL__

#include "idl/hokuyoaist.hh"

class RTCHokuyoAIST;

namespace RTCHokuyoAist
{

class HokuyoAistProvider
    : public virtual POA_Hokuyo_Aist::HokuyoAist,
    public virtual PortableServer::RefCountServantBase
{
    public:
        HokuyoAistProvider();
        virtual ~HokuyoAistProvider();

        void setup(RTCHokuyoAIST* owner) { owner_ = owner; }

        void request_scan() throw (CORBA::SystemException);

    private:
        RTCHokuyoAIST* owner_;
}; // class HokuyoAistProvider

}; // namespace RTCHokuyoAist

#endif // HOKUYOAIST_IMPL__

