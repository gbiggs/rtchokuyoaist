/* RTC:HokuyoAIST
 *
 * Component implementation.
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


#include <rtchokuyoaist/rtc.h>

#include <flexiport/flexiport.h>


RTCHokuyoAIST::RTCHokuyoAIST(RTC::Manager* manager)
    : RTC::DataFlowComponentBase(manager),
    ranges_port_("ranges", ranges_),
    intensities_port_("intensities", intensities_),
    svc_prov_(),
    svc_port_("ranger"),
    port_opts_("type=serial,device=/dev/ttyACM0,timeout=1"),
    start_angle_(0.0), end_angle_(0.0), cluster_count_(1),
    enable_intns_(false), high_sens_(false), pull_mode_(false),
    new_data_mode_(false), calibrate_time_(true), verbose_(false),
    error_time_(5),
    x_(0.0), y_(0.0), z_(0.0), roll_(0.0), pitch_(0.0), yaw_(0.0),
    base_ang_res_(0.0), last_error_time_(0)
{
}


RTCHokuyoAIST::~RTCHokuyoAIST()
{
}


RTC::ReturnCode_t RTCHokuyoAIST::onInitialize()
{
    bindParameter("port_opts", port_opts_,
            "type=serial,device=/dev/ttyACM0,timeout=1");
    bindParameter("start_angle", start_angle_, "0.0");
    bindParameter("end_angle", end_angle_, "0.0");
    bindParameter("cluster_count", cluster_count_, "1");
    bindParameter("enable_intensity", enable_intns_, "0");
    bindParameter("high_sensitivity", high_sens_, "0");
    bindParameter("pull_mode", pull_mode_, "0");
    bindParameter("new_data_mode", new_data_mode_, "0");
    bindParameter("calibrate_time", calibrate_time_, "1");
    bindParameter("verbose", verbose_, "0");
    bindParameter("error_time", error_time_, "5");

    bindParameter("x", x_, "0.0");
    bindParameter("y", y_, "0.0");
    bindParameter("z", z_, "0.0");
    bindParameter("roll", roll_, "0.0");
    bindParameter("pitch", pitch_, "0.0");
    bindParameter("yaw", yaw_, "0.0");

    addOutPort(ranges_port_.getName(), ranges_port_);
    addOutPort(intensities_port_.getName(), intensities_port_);
    svc_port_.registerProvider("ranger", "Ranger", svc_prov_);
    svc_port_.registerProvider("hokuyoaist", "HokuyoAist",
            specialist_prov_);
    addPort(svc_port_);
    svc_prov_.setup(this);
    specialist_prov_.setup(this);

    return RTC::RTC_OK;
}


RTC::ReturnCode_t RTCHokuyoAIST::onActivated(RTC::UniqueId ec_id)
{
    coil::Guard<coil::Mutex> guard(mutex_);
    laser_.set_verbose(verbose_);
    try
    {
        open_laser();
    }
    catch(hokuyoaist::BaseError &e)
    {
        std::cerr << "Error setting up laser: " << e.what() << '\n';
        return RTC::RTC_ERROR;
    }

    sensor_geom_.geometry.pose.position.x = x_;
    sensor_geom_.geometry.pose.position.y = y_;
    sensor_geom_.geometry.pose.position.z = z_;
    sensor_geom_.geometry.pose.orientation.r = roll_;
    sensor_geom_.geometry.pose.orientation.p = pitch_;
    sensor_geom_.geometry.pose.orientation.y = yaw_;

    return RTC::RTC_OK;
}


RTC::ReturnCode_t RTCHokuyoAIST::onDeactivated(RTC::UniqueId ec_id)
{
    coil::Guard<coil::Mutex> guard(mutex_);

    try
    {
        close_laser();
    }
    catch(hokuyoaist::BaseError &e)
    {
        std::cerr << "Error shutting down laser: " << e.what() << '\n';
        return RTC::RTC_ERROR;
    }

    return RTC::RTC_OK;
}


RTC::ReturnCode_t RTCHokuyoAIST::onExecute(RTC::UniqueId ec_id)
{
    coil::Guard<coil::Mutex> guard(mutex_);
    try
    {
        if(!pull_mode_)
        {
            get_scan();
        }
    }
    catch(hokuyoaist::BaseError &e)
    {
        std::cerr << "Error getting laser scan: " << e.what() << '\n';
        time_t now = time(NULL);
        if (now - last_error_time_ <= error_time_)
        {
            last_error_time_ = now;
            return RTC::RTC_ERROR;
        }
        else
        {
            last_error_time_ = now;
            try
            {
                std::cerr << "Attempting to reset laser.\n";
                reset_laser();
            }
            catch(hokuyoaist::BaseError &e)
            {
                std::cerr << "Reset failed: " << e.what() << '\n';
                return RTC::RTC_ERROR;
            }
            catch(flexiport::PortException &e)
            {
                std::cerr << "Reset failed: " << e.what() << '\n';
                return RTC::RTC_ERROR;
            }
        }
    }
    return RTC::RTC_OK;
}


void RTCHokuyoAIST::set_power(bool enable)
{
    coil::Guard<coil::Mutex> guard(mutex_);
    laser_.set_power(enable);
}


void RTCHokuyoAIST::enable_intensity_data(bool enable)
{
    coil::Guard<coil::Mutex> guard(mutex_);
    enable_intns_ = enable;
}


void RTCHokuyoAIST::set_config(RTC::RangerConfig const& config)
{
    coil::Guard<coil::Mutex> guard(mutex_);

    start_angle_ = sensor_config_.minAngle = config.minAngle;
    end_angle_ = sensor_config_.maxAngle = config.maxAngle;
    sensor_config_.angularRes = config.angularRes;
    cluster_count_ = config.angularRes / base_ang_res_;
    if(cluster_count_ < 1)
        cluster_count_ = 1;
}


void RTCHokuyoAIST::request_scan()
{
    coil::Guard<coil::Mutex> guard(mutex_);
    get_scan();
}


void RTCHokuyoAIST::open_laser()
{
    laser_.open(port_opts_);

    hokuyoaist::SensorInfo info;
    laser_.get_sensor_info(info);
    if(start_angle_ == 0.0)
    {
        sensor_config_.minAngle = info.min_angle;
    }
    else
    {
        sensor_config_.minAngle = start_angle_;
    }
    if(end_angle_ == 0.0)
    {
        sensor_config_.maxAngle = info.max_angle;
    }
    else
    {
        sensor_config_.maxAngle = end_angle_;
    }
    base_ang_res_ = info.resolution;
    sensor_config_.angularRes = info.resolution * cluster_count_;
    sensor_config_.minRange = info.min_range;
    sensor_config_.maxRange = info.max_range;
    sensor_config_.rangeRes = 0.01;
    sensor_config_.frequency = info.speed / 60.0;

    try
    {
        laser_.set_high_sensitivity(high_sens_);
    }
    catch(hokuyoaist::ResponseError &e)
    {
        if(e.desc_code() != 30)
        {
            throw;
        }
        std::cerr << "Warning: could not alter high-sensitivity setting "
            "(possibly not supported): " << e.what() << '\n';
    }

    if(calibrate_time_)
    {
        laser_.calibrate_time();
    }

    if(pull_mode_)
    {
        laser_.set_power(false);
    }
    else
    {
        laser_.set_power(true);
    }
}


void RTCHokuyoAIST::close_laser()
{
    laser_.set_power(false);
    laser_.close();
    scan_data_.clean_up();
}


void RTCHokuyoAIST::reset_laser()
{
    laser_.reset();
    close_laser();
    open_laser();
}


void RTCHokuyoAIST::get_scan()
{
    if(start_angle_ == 0.0 && end_angle_ == 0.0)
    {
        // Get a full scan
        if(new_data_mode_)
        {
            if(enable_intns_)
            {
                laser_.get_new_ranges_intensities(scan_data_, -1, -1,
                        cluster_count_);
            }
            else
            {
                laser_.get_new_ranges(scan_data_, -1, -1, cluster_count_);
            }
        }
        else
        {
            if(enable_intns_)
            {
                laser_.get_ranges_intensities(scan_data_, -1, -1,
                        cluster_count_);
            }
            else
            {
                laser_.get_ranges(scan_data_, -1, -1, cluster_count_);
            }
        }
    }
    else
    {
        // Get a partial scan
        if(new_data_mode_)
        {
            if(enable_intns_)
            {
                laser_.get_new_ranges_intensities_by_angle(scan_data_,
                        start_angle_, end_angle_, cluster_count_);
            }
            else
            {
                laser_.get_new_ranges_by_angle(scan_data_, start_angle_,
                        end_angle_, cluster_count_);
            }
        }
        else
        {
            if(enable_intns_)
            {
                laser_.get_ranges_intensities_by_angle(scan_data_,
                        start_angle_, end_angle_, cluster_count_);
            }
            else
            {
                laser_.get_ranges_by_angle(scan_data_, start_angle_,
                        end_angle_, cluster_count_);
            }
        }
    }

    write_scan();
}


void RTCHokuyoAIST::write_scan()
{
    ranges_.tm.sec = scan_data_.system_time_stamp() / 1000000000;
    ranges_.tm.nsec = scan_data_.system_time_stamp() % 1000000000;
    ranges_.ranges.length(scan_data_.ranges_length());
    for(unsigned int ii = 0; ii < scan_data_.ranges_length(); ii++)
    {
        if (scan_data_[ii] < 20)
            ranges_.ranges[ii] = 0.0;
        else
            ranges_.ranges[ii] = scan_data_[ii] / 1000.0;
    }
    ranges_.geometry = sensor_geom_;
    ranges_.config = sensor_config_;

    if(scan_data_.intensities_length() > 0)
    {
        intensities_.tm.sec = scan_data_.system_time_stamp() / 1000000000;
        intensities_.tm.nsec = scan_data_.system_time_stamp() % 1000000000;
        intensities_.intensities.length(scan_data_.intensities_length());
        for(unsigned int ii = 0; ii < scan_data_.intensities_length(); ii++)
        {
            intensities_.intensities[ii] = scan_data_.intensities()[ii];
        }
        intensities_.geometry = sensor_geom_;
        intensities_.config = sensor_config_;
    }

    ranges_port_.write();
    intensities_port_.write();
}


static const char* spec[] =
{
    "implementation_id", "RTCHokuyoAIST",
    "type_name",         "RTCHokuyoAIST",
    "description",       "Hokuyo laser scanner component.",
    "version",           "1.0",
    "vendor",            "Geoffrey Biggs, AIST",
    "category",          "Sensor",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "999",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.port_opts", "type=serial,device=/dev/ttyACM0,timeout=1",
    "conf.default.start_angle", "0.0",
    "conf.default.end_angle", "0.0",
    "conf.default.cluster_count", "1",
    "conf.default.enable_intensity", "0",
    "conf.default.high_sensitivity", "0",
    "conf.default.pull_mode", "0",
    "conf.default.new_data_mode", "0",
    "conf.default.verbose", "0",
    "conf.default.error_time", "5",
    "conf.default.x", "0.0",
    "conf.default.y", "0.0",
    "conf.default.z", "0.0",
    "conf.default.roll", "0.0",
    "conf.default.pitch", "0.0",
    "conf.default.yaw", "0.0",
    // Widget
    "conf.__widget__.port_opts", "text",
    "conf.__widget__.start_angle", "spin",
    "conf.__widget__.end_angle", "spin",
    "conf.__widget__.cluster_count", "spin",
    "conf.__widget__.enable_intensity", "radio",
    "conf.__widget__.high_sensitivity", "radio",
    "conf.__widget__.pull_mode", "radio",
    "conf.__widget__.new_data_mode", "radio",
    "conf.__widget__.verbose", "radio",
    "conf.__widget__.error_time", "spin",
    "conf.__widget__.x", "spin",
    "conf.__widget__.y", "spin",
    "conf.__widget__.z", "spin",
    "conf.__widget__.roll", "spin",
    "conf.__widget__.pitch", "spin",
    "conf.__widget__.yaw", "spin",
    // Constraints
    "conf.__constraints__.cluster_count", "1<=x",
    "conf.__constraints__.error_time", "0<=x",
    ""
};

extern "C"
{
    void rtc_init(RTC::Manager* manager)
    {
        coil::Properties profile(spec);
        manager->registerFactory(profile, RTC::Create<RTCHokuyoAIST>,
                RTC::Delete<RTCHokuyoAIST>);
    }
};

