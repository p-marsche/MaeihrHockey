//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#include "stdafx.h"

#include "Game.hpp"
#include "box2d/box2d.h"


int main()
{
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try
    {
        mmt_gd::Game game;
        game.run();
    }
    catch (std::exception e)
    {
        //std::cerr << e.what() << std::endl;
    }

    return 0;
}
