#pragma once

struct GameConfig {
    static constexpr float fixed_dt = 1.0f / 60.0f;
    static constexpr float max_frame_dt = 0.25f;
    static constexpr float gravity = 600.0f;
    static constexpr float jump_power = 1000.0f;
    static constexpr int ground_level = 950;
    static constexpr int window_width = 640;
    static constexpr int window_height = 480;
};
