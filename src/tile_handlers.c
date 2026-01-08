#include <stdbool.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>

#include "tile_handlers.h"
#include "defines.h"
#include "player.h"

// assign props to each tile type
const uint8_t tile_props[] = {
    0,
    0,
    PROP_DANGER,
    PROP_CLIMBABLE,
    PROP_SOLID,
    PROP_SOLID,
    PROP_SOLID,
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