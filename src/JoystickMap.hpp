//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

namespace mmt_gd
{
/// <summary>
/// Axis and value mapping for arcade joystick, for easier readability
/// </summary>
struct JoystickMap
{
public:
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };
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
    static const int ZERO_SQAURED = ZERO_POS * ZERO_POS; ///< for comparisons with squared ("absolute") position
};
} // namespace mmt_gd