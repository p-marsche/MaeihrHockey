//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#include "stdafx.h"

#include "Config.hpp"

namespace mmt_gd
{
const std::string Config::assetBasePath = "../assets/";
const std::string Config::texturesPath  = assetBasePath + "textures/";
const std::string Config::soundsPath    = assetBasePath + "sounds/";
const std::string Config::fontsPath     = assetBasePath + "fonts/";
const std::string Config::musicPath     = assetBasePath + "music/";
const std::string Config::imagesPath	= assetBasePath + "images/";
const std::string Config::guiPath       = assetBasePath + "gui/";
const std::string Config::fragShaderPath = assetBasePath + "shaders/frag/";
const std::string Config::tilemapPath = assetBasePath;
} // namespace mmt_gd