#include <stdlib.h>
#include <string.h>
#include <stdint.h>


extern const uint8_t tile_props[];

uint8_t moveable_tile(int x, int y);
uint8_t moveable_tile_left_bottom(int x, int y);
uint8_t moveable_tile_right_bottom(int x, int y);
uint8_t get_tile_props(int x, int y);