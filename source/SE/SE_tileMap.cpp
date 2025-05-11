#include "SE_tileMap.h"

extern std::string tiledResourcesPath;

SimpleEngine::TileMap::TileMap(const std::string p_asset)
{
    TiledTileMap ttm;
    TiledTileMap::fromJson(p_asset, ttm);

    m_NumberOfColumns = ttm.width;
    m_NumberOfRows = ttm.height;
    m_TileHeight = ttm.tileHeight;
    m_TileWidth = ttm.tileWidth;

    for (const TiledTileMapLayer& layer : ttm.layers)
    {
        if (layer.data.empty())
        {
            // TODO: add objets
            continue;
        }

        m_layers.push_back({{}});
        TileLayer& newLayer = m_layers.back();

        for (uint32_t tileIndex = 0; tileIndex < layer.data.size(); tileIndex++)
        {
            const TiledTileMapLayerTile& tile = layer.data[tileIndex];
            if (tile.id == 0)
            {
                continue;
            }

            const TiledTileMapTileSet* tileSet = ttm.getTileSetPtrForTileId(tile.id);
            if (!tileSet)
            {
                throw std::runtime_error("[TileMap] No tileset for tile id");
            }

            // setup tile sprite rotation
            double rotation = 0.0f;
            bool flipVertically = tile.flippedVertically;
            bool flipHorizontally = tile.flippedHorizontally;
            if (tile.flippedVertically && tile.flippedDiagonally)
            {
                // left
                rotation = M_PI * 1.5;
                flipVertically = false;
            }
            else if (tile.flippedHorizontally && tile.flippedDiagonally)
            {
                // right
                rotation = M_PI * 0.5;
                flipHorizontally = false;
            }
            else if (tile.flippedVertically && tile.flippedHorizontally)
            {
                // top
                rotation = M_PI;
                flipVertically = false;
                flipHorizontally = false;
            }

            // setup tile screen position
            uint32_t tileRow = tileIndex / m_NumberOfColumns;
            uint32_t tileColumn = tileIndex % m_NumberOfColumns;
            newLayer.m_tiles.push_back(
                {{static_cast<float>(tileColumn * m_TileWidth), static_cast<float>(tileRow * m_TileHeight)},
                 rotation,
                 {tiledResourcesPath + tileSet->sheet.image,
                  {{Modifier::Type::ATLAS, {tileSet->sheet.rows, tileSet->sheet.columns, tile.id - 1}},
                   {Modifier::Type::FLIP, {flipHorizontally, flipVertically}}}},
                 // TODO: add tile properties here
                 {}});

            // setup animation counter
            Counter* animatedSpriteCounterPtr = nullptr;
            auto it = tileSet->sheet.tiles.find(tile.id - tileSet->firstId);
            if (it != tileSet->sheet.tiles.end())
            {
                const TiledTileSheetTile& sheetTileRef = it->second;

                if (!sheetTileRef.keyframes.empty())
                {
                    Modifier& atlasModifier = newLayer.m_tiles.back().m_sprite.m_modifiers[0];
                    std::vector<Counter::State> states;
                    for (const TiledTileSheetTileAnimationFrame& frame : sheetTileRef.keyframes)
                    {
                        states.emplace_back(frame.tileId, frame.duration);
                    }
                    animatedSpriteCounterPtr = new Counter(&atlasModifier.m_atlas.m_spriteIndex, states);
                }
            }
        }
    }
}
