#include <graphx.h>
#include <stdint.h>
#include <stdlib.h>

#define TILE_WIDTH          16
#define TILE_HEIGHT         16

#define TILEMAP_WIDTH       100
#define TILEMAP_HEIGHT      16 

#define TILEMAP_DRAW_WIDTH  21
#define TILEMAP_DRAW_HEIGHT 15

#define SCROLL_MARGIN_X (10 * TILE_WIDTH)   // 2 tiles from left/right
#define SCROLL_MARGIN_Y (4 * TILE_HEIGHT)  // 2 tiles from top/bottom

#define SCREEN_HEIGHT (TILEMAP_DRAW_HEIGHT * TILE_HEIGHT)

#define START_X 32 
#define START_Y 64

#define PLAYER_HITBOX_WIDTH 16
#define PLAYER_HITBOX_HEIGHT 32

#define PROP_SOLID      (1 << 0) // 0000 0001 (1)
#define PROP_CLIMBABLE  (1 << 1) // 0000 0010 (2)
#define PROP_DANGER     (1 << 2) // 0000 0100 (4)
#define PROP_EDGE       (1 << 3) // 8
#define PROP_AIR        (1 << 4) // 16

#define MAX_ACTIVE_ENEMIES 8