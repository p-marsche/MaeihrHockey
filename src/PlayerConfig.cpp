#include "stdafx.h"

#include "PlayerConfig.hpp"

namespace mmt_gd
{

    std::string PaddleConfig::ActiveToString(PaddleAbility in)
{
    switch (in)
    {
        case PaddleAbility::DASH:
            return "Dash";
        case PaddleAbility::ENLARGE:
            return "Enlarge";
        default:
            return "Invalid";
    }
}

PaddleAbility PaddleConfig::GetNextActive(PaddleAbility in)
{
    switch (in)
    {
        case PaddleAbility::DASH:
            return PaddleAbility::ENLARGE;
        case PaddleAbility::ENLARGE:
            return PaddleAbility::DASH;
        default:
            return PaddleAbility::DASH;
    }
}
std::string PaddleConfig::PassiveToString(PaddlePassive in)
{
    switch (in)
    {
        case PaddlePassive::NOTHING:
            return "Normal";
        case PaddlePassive::BOUNCY:
            return "Bouncy";
        case PaddlePassive::TRAP:
            return "Trap";
        default:
            return "Invalid";
    }
}

PaddlePassive PaddleConfig::GetNextPassive(PaddlePassive in)
{
    switch (in)
    {
        case PaddlePassive::NOTHING:
            return PaddlePassive::BOUNCY;
        case PaddlePassive::BOUNCY:
            return PaddlePassive::TRAP;
        case PaddlePassive::TRAP:
            return PaddlePassive::NOTHING;
        default:
            return PaddlePassive::NOTHING;
    }
}
}