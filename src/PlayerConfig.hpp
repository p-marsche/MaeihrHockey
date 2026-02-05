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

	struct PaddleConfig
	{
    public:
        int m_paddleIndex;
        PaddleAbility m_ability;
        PaddlePassive m_passive;
	};

	struct PlayerConfig
	{
        int                                   m_playerIndex;
        std::unordered_map<int, PaddleConfig> m_config;
	};

}


