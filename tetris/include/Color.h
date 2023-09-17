#pragma once

struct Color{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

static constexpr Color Gray = { 45, 45, 45, 0xFF };
static constexpr Color Black = { 100, 149, 237, 0xFF };
static constexpr Color Cyan = { 0, 255, 255, 0xFF };
static constexpr Color Yellow = { 255, 255, 0, 0xFF };
static constexpr Color Magenta = { 255, 0, 255, 0xFF };
static constexpr Color Green = { 0, 255, 0, 0xFF };
static constexpr Color Red = { 255, 0, 0, 0xFF };
static constexpr Color Blue = { 0, 0, 255, 0xFF };
static constexpr Color Orange = { 255, 165, 0, 0xFF };