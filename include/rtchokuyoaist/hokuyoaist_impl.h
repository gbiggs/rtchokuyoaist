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

