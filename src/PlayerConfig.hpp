//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "stdafx.h"

namespace mmt_gd
{
	enum PaddleAbility
	{
		DASH = 0,
		ENLARGE = 1
	};

	enum PaddlePassive
	{
        NOTHING = 0,
		BOUNCY = 1,
        TRAP = 2,
		HEAVY = 3
	};

	// paddleIndex = 0, 1, 2
	struct PaddleConfig
	{
    public:
        int m_paddleIndex;
        PaddleAbility m_ability;
        PaddlePassive m_passive;

		static std::string ActiveToString(PaddleAbility in);
		static PaddleAbility GetNextActive(PaddleAbility in);
		static std::string PassiveToString(PaddlePassive in);
        static PaddlePassive GetNextPassive(PaddlePassive in);
	};

	// playerIndex = 0, 1
	struct PlayerConfig
	{
        int                                   m_playerIndex;
        std::array<PaddleConfig, 3>			  m_config;
	};

}


