#pragma once

#include "AssetManager.hpp"
#include "SpriteManager.hpp"
#include "TileLayerRenderComponent.hpp"
#include "Tileson.hpp"

namespace mmt_gd
{
class TileMapLoader
{
public:
    static void loadTileSetTextures(const std::unique_ptr<tson::Map>& map);

    static void createTileSprite(const std::unique_ptr<tson::Map>& map,
                                 std::vector<TileLayer>&           tileLayers,
                                 int                               layerIdx,
                                 const tson::Layer&                layer,
                                 int                               i,
                                 unsigned                          gid);

    static void loadTileLayers(const std::unique_ptr<tson::Map>& map, SpriteManager& spriteManager);

    static void loadObjectLayers(const std::unique_ptr<tson::Map>& map);
};
} // namespace mmt_gd
