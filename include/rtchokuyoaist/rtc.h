/* RTC:HokuyoAIST
 *
 * Copyright (C) 2010-2011
 *     Geoffrey Biggs
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 * Licensed under the Eclipse Public License -v 1.0 (EPL)
 * http://www.opensource.org/licenses/eclipse-1.0.txt
 *
 * Component header file.
 */


#ifndef RTC_H__
#define RTC_H__

#include <hokuyoaist/hokuyoaist.h>
#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/OutPort.h>
#include <rtm/CorbaPort.h>
#include <time.h>

#include "ranger_impl.h"
#include "hokuyoaist_impl.h"

using namespace RTC;


class RTCHokuyoAIST
: public RTC::DataFlowComponentBase
{
    public:
        RTCHokuyoAIST(RTC::Manager* manager);
        ~RTCHokuyoAIST();

        virtual RTC::ReturnCode_t onInitialize();
        virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);
        virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);
        virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

        // Functions used by the services
        RTC::RangerGeometry get_geometry() const { return sensor_geom_; }
        void set_power(bool enable);
        void enable_intensity_data(bool enable);
        RTC::RangerConfig get_config() const { return sensor_config_; }
        void set_config(RTC::RangerConfig const& config);
        void request_scan();

    protected:
        // Ports
        RTC::RangeData ranges_;
        RTC::OutPort<RTC::RangeData> ranges_port_;
        RTC::IntensityData intensities_;
        RTC::OutPort<RTC::IntensityData> intensities_port_;
        RTCHokuyoAist::RangerProvider svc_prov_;
        RTCHokuyoAist::HokuyoAistProvider specialist_prov_;
        RTC::CorbaPort svc_port_;

        // Configurable settings
        std::string port_opts_;
        double start_angle_, end_angle_;
        unsigned int cluster_count_;
        bool enable_intns_;
        bool high_sens_;
        bool pull_mode_;
        bool new_data_mode_;
        time_t error_time_;
        double x_, y_, z_, roll_, pitch_, yaw_;

        hokuyoaist::Sensor laser_;
        hokuyoaist::ScanData scan_data_;
        RTC::RangerGeometry sensor_geom_;
        RTC::RangerConfig sensor_config_;

        coil::Mutex mutex_;
        double base_ang_res_;
        time_t last_error_time_;

        void open_laser();
        void close_laser();
        void reset_laser();
        void get_scan();
        void write_scan();
};


extern "C"
{
    DLL_EXPORT void rtc_init(RTC::Manager* manager);
};

#endif // RTC_H__

