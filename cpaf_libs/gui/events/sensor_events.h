#pragma once
#include <cstdint>

namespace cpaf::gui::sensor {

////////////////////////////////////////////////////////////
/// \brief Give access to the real-time state of the sensors
///
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// \brief Sensor type
///
////////////////////////////////////////////////////////////
enum class type : std::uint8_t
{
    accelero_meter,     //!< Measures the raw acceleration (m/s^2)
    gyroscope,          //!< Measures the raw rotation rates (degrees/s)
    magneto_meter,      //!< Measures the ambient magnetic field (micro-teslas)
    gravity,            //!< Measures the direction and intensity of gravity, independent of device acceleration (m/s^2)
    user_acceleration,  //!< Measures the direction and intensity of device acceleration, independent of the gravity (m/s^2)
    orientation,        //!< Measures the absolute 3D orientation (degrees)

    COUNT               //!< Keep last -- the total number of sensor types
};

} //END namespace cpaf::gui::sensor
