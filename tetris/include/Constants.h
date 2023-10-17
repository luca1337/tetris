#pragma once

static constexpr auto BlockSize = 30;

static constexpr auto Columns = 10;
static constexpr auto Rows = 20;

static constexpr auto BackgroundIndex = 0;
static constexpr auto BoundIndex = 1;

static constexpr auto MaxNumTetraminos = 7;

static constexpr auto TetraminoIIndex = 2;
static constexpr auto TetraminoJIndex = 3;
static constexpr auto TetraminoLIndex = 4;
static constexpr auto TetraminoOIndex = 5;
static constexpr auto TetraminoSIndex = 6;
static constexpr auto TetraminoTIndex = 7;
static constexpr auto TetraminoZIndex = 8;

#define COUNT_OF(array) (sizeof(array) / sizeof(array[0]))

// timer values
static constexpr auto MaxBlinkTime = 0.97f;