#pragma once

#include "GameObject.hpp"
#include "IEvent.hpp"

namespace mmt_gd
{
    // int -> playerID, so we know which side it is
    // player 1 scores -> data = 2
    class GoalEvent final : public BasicDataEvent<int>
    {
    public:
        using BasicDataEvent::BasicDataEvent;

        static const EventType Type;

        EventType getEventType() override
        {
            return Type;
        }
    };
} // namespace mmt_gd

