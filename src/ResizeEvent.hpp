//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "GameObject.hpp"
#include "IEvent.hpp"

namespace mmt_gd
{
    // int -> playerID, so we know which side it is
    // player 1 scores -> data = 2
    class ResizeEvent final : public IEvent
    {
    public:
        static const EventType Type;

        EventType getEventType() override
        {
            return Type;
        }
    };
} // namespace mmt_gd

