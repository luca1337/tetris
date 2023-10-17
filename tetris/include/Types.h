#pragma once

enum class TetraminoType {
    I,
    J,
    L,
    O,
    S,
    T,
    Z
};

enum class DirectionType{
    Left,
    Right,
    Down
};

enum class StateType{
    Setup,
    MainMenu,
    
    Play,
    Check,
    Blink,
    Clear,
};