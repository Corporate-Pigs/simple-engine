#pragma once

#include <cppvec/vec2.h>
#include <stdint.h>
#include <tiled.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "SE_counter.h"
#include "SE_sprite.h"

namespace SimpleEngine
{

struct Property
{
    std::string m_name;
    std::variant<bool, std::string> m_value;
};

struct Event
{
    uint32_t m_id;
    std::string m_name;
    std::string m_type;
    std::vector<Property> m_properties;
};

struct Tile
{
    cppvec::Vec2<float> m_offset;
    double m_rotationInRadians;
    Sprite m_sprite;
    std::shared_ptr<Counter> m_counterPtr;
    std::vector<Event> m_events;
    std::vector<Property> m_properties;
};

struct TileLayer
{
    std::vector<Tile> m_tiles;
};

struct TileMap
{
    std::unique_ptr<TiledTileMap> m_tiledTileMap;
    std::unordered_map<std::string, TiledTileSheet> m_tiledTileSheets;
    std::vector<TileLayer> m_layers;
    uint32_t m_NumberOfColumns, m_NumberOfRows, m_TileHeight, m_TileWidth;

    TileMap(const std::string p_asset);
};

}  // namespace SimpleEngine
