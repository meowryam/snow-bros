#pragma once

// This just defines the 4 possible states of our game
// Think of it like a variable that can only be one of these 4 values
enum class GameState {
    LOGIN,
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    KEY_REMAP,
    SHOP,
    LEVEL_SELECT
};