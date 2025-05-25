#include "SE_tileMap.h"

extern std::string tiledResourcesPath;

static constexpr auto k_graphicsLayerName = "graphics";
static constexpr auto k_objectsLayerName = "events";

void PopulateGraphicsTileMap(const TiledTileMap& p_ttm, const TiledTileMapLayer& p_tiledLayer,
                             SimpleEngine::TileLayer& p_layer)
{
    using namespace SimpleEngine;

    for (uint32_t tileIndex = 0; tileIndex < p_tiledLayer.data.size(); tileIndex++)
    {
        const TiledTileMapLayerTile& tile = p_tiledLayer.data[tileIndex];
        if (tile.id == 0)
        {
            continue;
        }

        const TiledTileMapTileSet* tileSet = p_ttm.getTileSetPtrForTileId(tile.id);
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
        uint32_t tileRow = tileIndex / p_ttm.width;
        uint32_t tileColumn = tileIndex % p_ttm.width;
        Tile& targetTile = p_layer.m_tiles[tileIndex];

        targetTile.m_offset = {static_cast<float>(tileColumn * p_ttm.tileWidth),
                               static_cast<float>(tileRow * p_ttm.tileHeight)};
        targetTile.m_rotationInRadians = rotation;
        targetTile.m_sprite = {tiledResourcesPath + tileSet->sheet.image,
                               {{Modifier::Type::ATLAS, {tileSet->sheet.rows, tileSet->sheet.columns, tile.id - 1}},
                                {Modifier::Type::FLIP, {flipHorizontally, flipVertically}}}};

        // setup animation counter

        targetTile.m_counterPtr = nullptr;
        auto it = tileSet->sheet.tiles.find(tile.id - tileSet->firstId);
        if (it != tileSet->sheet.tiles.end())
        {
            const TiledTileSheetTile& sheetTileRef = it->second;

            if (!sheetTileRef.keyframes.empty())
            {
                std::vector<Counter::State> states;
                for (const TiledTileSheetTileAnimationFrame& frame : sheetTileRef.keyframes)
                {
                    states.emplace_back(frame.tileId, frame.duration);
                }
                targetTile.m_counterPtr.reset(
                    new Counter(&(targetTile.m_sprite.m_modifiers[0].m_atlas.m_spriteIndex), states));
            }
        }
    }
}

void PopulateObjectsTileMap(const TiledTileMap& ttm, const TiledTileMapLayer& layer, SimpleEngine::TileLayer& newLayer)
{
    using namespace SimpleEngine;

    for (const TiledTileMapLayerObject& object : layer.objects)
    {
        uint32_t tileColumn = object.x / ttm.tileWidth;
        uint32_t tileRow = object.y / ttm.tileHeight;
        uint32_t tileIndex = tileRow * ttm.width + tileColumn;
        newLayer.m_tiles[tileIndex].m_events.push_back({object.id, object.name, object.type, {}});
    }
}

SimpleEngine::TileMap::TileMap(const std::string p_asset)
{
    TiledTileMap ttm;
    TiledTileMap::fromJson(p_asset, ttm);

    m_NumberOfColumns = ttm.width;
    m_NumberOfRows = ttm.height;
    m_TileHeight = ttm.tileHeight;
    m_TileWidth = ttm.tileWidth;

    for (const TiledTileMapLayer& layerGroup : ttm.layers)
    {
        assert(!layerGroup.layers.empty());

        m_layers.push_back({});
        TileLayer& newLayer = m_layers.back();

        newLayer.m_tiles.reserve(m_NumberOfColumns * m_NumberOfRows);
        for (uint32_t i = 0; i < m_NumberOfColumns * m_NumberOfRows; i++)
        {
            newLayer.m_tiles.push_back({});
        }

        for (const TiledTileMapLayer& layer : layerGroup.layers)
        {
            if (!layer.data.empty() && layer.name == k_graphicsLayerName)
            {
                PopulateGraphicsTileMap(ttm, layer, newLayer);
            }
            else if (!layer.objects.empty() && layer.name == k_objectsLayerName)
            {
                PopulateObjectsTileMap(ttm, layer, newLayer);
            }
            else
            {
                assert(false);
            }
        }
    }
}
