#pragma once

#include "stdafx.h"

namespace mmt_gd
{
	enum PaddleAbility
	{
		DASH,
		ENLARGE
	};

	enum PaddlePassive
	{
		HEAVY,
		BOUNCY,
		NOTHING
	};

	// paddleIndex = 0, 1, 2
	struct PaddleConfig
	{
    public:
        int m_paddleIndex;
        PaddleAbility m_ability;
        PaddlePassive m_passive;
	};

	// playerIndex = 0, 1
	struct PlayerConfig
	{
        int                                   m_playerIndex;
        std::array<PaddleConfig, 3>			  m_config;
	};

}


