#pragma once
#include <cstring>

namespace sprites {

struct SpriteInfo {
    const char* name;
    int x, y, w, h;
};

class TILESET {
public:
    static constexpr int ATLAS_WIDTH = 1138;
    static constexpr int ATLAS_HEIGHT = 210;
    static constexpr int SPRITE_COUNT = 24;
    
    static const SpriteInfo sprites[25];
    inline static const SpriteInfo* getSprite(const char* name);
};

// Sprite definitions
inline const SpriteInfo TILESET::sprites[] = {
    {"BigGrassSpike", 584, 0, 192, 64},
    {"BigWhole", 486, 0, 96, 144},
    {"BigWhole2", 162, 0, 160, 144},
    {"BigWhole3", 778, 0, 96, 96},
    {"BothSideProtrusion", 876, 0, 96, 80},
    {"DediumPlatform", 162, 146, 96, 31},
    {"GrassCornerBottomLeft", 811, 136, 64, 60},
    {"GrassCornerBottomRight", 877, 136, 64, 60},
    {"GrassEdgeLeft", 716, 144, 64, 52},
    {"GrassEdgeRight", 650, 144, 64, 52},
    {"GrassLarge", 876, 82, 95, 52},
    {"GrassLarge3", 584, 130, 64, 62},
    {"GrassMedium", 0, 160, 64, 32},
    {"GrassSmall", 66, 160, 32, 48},
    {"GrassSmall2", 714, 66, 32, 28},
    {"GrassSpike", 714, 98, 95, 44},
    {"HoleLarge", 0, 0, 160, 158},
    {"HoleMedium", 324, 0, 160, 144},
    {"LeftProtrusion", 1072, 0, 64, 80},
    {"RightProtrusion", 974, 64, 64, 80},
    {"SmallGrass", 100, 160, 32, 31},
    {"SmallWall", 811, 98, 62, 32},
    {"WallBottom", 584, 66, 128, 62},
    {"WallTop", 974, 0, 96, 62},
    {nullptr, 0, 0, 0, 0}
};

inline const SpriteInfo* TILESET::getSprite(const char* name) {
    if (!name) return nullptr;
    for (int i = 0; i < SPRITE_COUNT; i++) {
        const auto& sprite = sprites[i];
        if (sprite.name && std::strcmp(sprite.name, name) == 0)
            return &sprite;
    }
    return nullptr;
}

} // namespace sprites
