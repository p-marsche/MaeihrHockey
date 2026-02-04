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
    static const int THRESHOLD_POS = 90;
    static const int THRESHOLD_NEG = -90;
    static const int UP    = THRESHOLD_NEG;
    static const int DOWN  = THRESHOLD_POS;
    static const int LEFT  = THRESHOLD_NEG;
    static const int RIGHT = THRESHOLD_POS;
    // actual value is ~ -0,78
    static const int ZERO_POS = 10;
    static const int ZERO_NEG = -10;
};
} // namespace mmt_gd