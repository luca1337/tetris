#pragma once

enum class TetraminoType
{
    I,
    J,
    L,
    O,
    S,
    T,
    Z
};

enum class DirectionType
{
    Left,
    Right,
    Down
};

enum class StateType
{
    Setup,

    Play,
    Check,
    Blink,
    Clear,
};

enum class PostProcessingType
{
    cctv,
    chroma,
    edge_detection,
    embossing
};

enum class ResourceType
{
    Shader,
    Texture,  
    Audio
};

enum ShapeType
{
    None,
    Circle,
    HalfCircle,
};