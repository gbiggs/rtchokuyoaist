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

#if !defined(RANGER_IMPL__)
#define RANGER_IMPL__

#include "idl/ranger.hh"

class RTC_HokuyoAist;

namespace RTCHokuyoAist
{

class RangerProvider
    : public virtual POA_RTC::Ranger,
    public virtual PortableServer::RefCountServantBase
{
    public:
        RangerProvider();
        virtual ~RangerProvider();

        void setup(RTC_HokuyoAist* owner) { owner_ = owner; }

        RTC::RangerGeometry* GetGeometry() throw (CORBA::SystemException);
        void Power(bool enable) throw (CORBA::SystemException);
        void EnableIntensities(bool enable) throw (CORBA::SystemException);
        RTC::RangerConfig GetConfig() throw (CORBA::SystemException);
        void SetConfig(RTC::RangerConfig const& new_config)
            throw (CORBA::SystemException);

    private:
        RTC_HokuyoAist* owner_;
}; // class RangerProvider

}; // namespace RTCHokuyoAist

#endif // RANGER_IMPL__

