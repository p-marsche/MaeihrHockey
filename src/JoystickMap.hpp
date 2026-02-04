#pragma once

namespace mmt_gd
{
/// <summary>
/// Axis and value mapping for arcade joystick, for easier readability
/// </summary>
struct JoystickMap
{
public:
    static const int AXIS_VERTICAL   = 1;
    static const int AXIS_HORIZONTAL = 0;
    // actual values are +/-100, but room for error
    static const int UP    = -90;
    static const int DOWN  = 90;
    static const int LEFT  = -90;
    static const int RIGHT = 90;
    // actual value is ~ -0,78
    static const int ZERO_POS = 10;
    static const int ZERO_NEG = 10;
};
} // namespace mmt_gd