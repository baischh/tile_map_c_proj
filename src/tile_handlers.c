#include <stdbool.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>

#include "tile_handlers.h"
#include "defines.h"
#include "player.h"
#include "gfx/gfx.h"


extern const uint8_t tilemap_map[];

// assign props to each tile type
const uint8_t tile_props[] = {
    0,
    0,
    PROP_DANGER,
    PROP_CLIMBABLE,
    PROP_SOLID | PROP_EDGE,
    PROP_SOLID,
    PROP_SOLID | PROP_EDGE,
    PROP_CLIMBABLE,
    0,
    PROP_DANGER,
    0,
    PROP_SOLID,
    0,
    0,
    0,
    0
};

/**
 * Returns the prop bits for a given position as a uint8_t
 * ex. PROP_SOLID = 0000 0001 = 1
 * Props defined in defines.h and assigned in tile_handers.c
 */
uint8_t get_tile_props(int x, int y) {
    int tx = x / TILE_WIDTH;
    int ty = y / TILE_HEIGHT;
    uint8_t tile = tilemap_map[ty * TILEMAP_WIDTH + tx];

    return tile_props[tile];
}