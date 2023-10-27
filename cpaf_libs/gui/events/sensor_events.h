#pragma once
#include <cstdint>
#include <cpaf_libs/gui/events/base_events.h>

namespace cpaf::gui::events {

/** @brief Give access to the real-time state of the sensors
 *
*/
struct sensor : public base
{
    /** @brief Sensor type
     *
    */
    enum class type : std::uint8_t
    {
        unknown,            //!< Unknown sensor type
        accelero_meter,     //!< Measures the raw acceleration (m/s^2)
        gyroscope,          //!< Measures the raw rotation rates (degrees/s)
        magneto_meter,      //!< Measures the ambient magnetic field (micro-teslas)
        gravity,            //!< Measures the direction and intensity of gravity, independent of device acceleration (m/s^2)
        acceleration,       //!< Measures the direction and intensity of device acceleration, independent of the gravity (m/s^2)
        orientation,        //!< Measures the absolute 3D orientation (degrees)
        COUNT               //!< Keep last -- the total number of sensor types
    };

    static std::string  to_name     (sensor::type t);
    std::string         name        () const override { return "sensor"; }
    std::string         to_string   (to_str_mode mode = to_str_mode::normal) const override;


    sensor::type tp     = sensor::type::unknown;    //!< Type of the sensor
    float        x      = 0;                        //!< Current value of the sensor on X axis
    float        y      = 0;                        //!< Current value of the sensor on Y axis
    float        z      = 0;                        //!< Current value of the sensor on Z axis
};


} //END namespace cpaf::gui::events
