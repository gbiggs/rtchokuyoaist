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


#include <rtchokuyoaist/hokuyoaist_impl.h>
#include <rtchokuyoaist/rtc.h>

using namespace RTCHokuyoAist;

//////////////////////////////////////////////////////////////////////////////
// HokuyoAist provider class

HokuyoAistProvider::HokuyoAistProvider()
    : owner_(0)
{
}


HokuyoAistProvider::~HokuyoAistProvider()
{
}


void HokuyoAistProvider::request_scan()
    throw(CORBA::SystemException)
{
    owner_->request_scan();
}

