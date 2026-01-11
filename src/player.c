/**
 * player.c handles player movement and player state
 * manages key press flags
 */


#include <stdbool.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>

#include "tile_handlers.h"
#include "defines.h"
#include "player.h"
#include "objects.h"

bool pressed_left = false;
bool pressed_right = false;
bool pressed_up = false;
bool pressed_down = false;
bool pressed_alpha = false;
bool pressed_2nd = false;
bool allow_up_press = true;

bool player_did_collide = true;

extern const uint8_t tilemap_map[];
extern bool game_over;

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

/**
 * Return true if the tile at px coords is solid
 */
bool solid_at_px(int x, int y) {
    int tx = x / TILE_WIDTH;
    int ty = y / TILE_HEIGHT;
    uint8_t tile = tilemap_map[ty * TILEMAP_WIDTH + tx];
    return (tile == 4
            || tile == 5
            || tile == 6
            || tile == 11);
}



/**
 * Handle all player motion by managing three coordinate systems:  
 * 
 * player.xy - world coords
 * player.scroll_xy - view coords
 * player.relxy - player position on screen
 */
void move_player(void) {
    uint8_t new_vx, left_bottom_test, right_bottom_test, climbing;
    int8_t mm;

    int diff_y, prev_y;
    int new_y_top, new_y_bot, new_x_left, new_x_right;

    // load variables
    new_vx = player.vx;
    prev_y = player.y;
    new_y_top = prev_y;
    new_y_bot = new_y_top + player.hitbox.height;
    new_x_left = player.x;
    new_x_right = new_x_left + PLAYER_HITBOX_WIDTH;
    mm = player.momentum;

    player.rel_x = new_x_left - player.scrollx;
    player.rel_y = new_y_top - player.scrolly;


    //TODO: legacy code pulled from OIRAM update w/ bitwise props
    right_bottom_test = !(get_tile_props(new_x_right, new_y_bot + 1) & PROP_SOLID);
    left_bottom_test  = !(get_tile_props(new_x_left, new_y_bot + 1) & PROP_SOLID);

    climbing = ((get_tile_props(player.x, player.y + PLAYER_HITBOX_HEIGHT - 1) |
                get_tile_props(player.x + 16 - 1, player.y + PLAYER_HITBOX_HEIGHT - 1)) & PROP_CLIMBABLE);

    // if nothing below, start accelerating
    if (left_bottom_test && right_bottom_test && !climbing)
    {
        if (player.vy < 10)
        {
            if (player.vy >= 0)
            {
                player.vy += 2;
            } 
            else 
            {
                player.vy++;
            }
        }
    }
    else if (climbing)
    {
        player.vy = 4;
    }

    if (pressed_2nd) 
    {
        if(climbing)
        {
            player.vy = -2;
        }

        uint8_t p_left = get_tile_props(player.x, player.y + PLAYER_HITBOX_HEIGHT + player.vy);
        uint8_t p_right = get_tile_props(player.x + 16 - 1, player.y + PLAYER_HITBOX_HEIGHT + player.vy);

        // only allow jump while standing on solid tile
        if ((p_left | p_right) & PROP_SOLID) {
            player.vy = -11;
        }

        pressed_2nd = false;
    }

    if (player.vy)
    {
        int distance_to_move = player.vy;

        if ((get_tile_props(player.x, player.y + PLAYER_HITBOX_HEIGHT + player.vy) |
            get_tile_props(player.x + 16 - 1, player.y + PLAYER_HITBOX_HEIGHT + player.vy)) & PROP_SOLID)
        {
            distance_to_move = player.vy - ((player.y + PLAYER_HITBOX_HEIGHT + player.vy) % 16);
            player.vy = 0;
        }

        if ((get_tile_props(player.x, player.y - 1) |
            get_tile_props(player.x + PLAYER_HITBOX_WIDTH - 1, player.y - 1)) & PROP_SOLID)
        {
            player.vy = 0;
        }

        player.y += distance_to_move;
        new_y_top += player.vy;
        new_y_bot += player.vy;
    }


    if (pressed_right)
    {
        uint8_t p_top = get_tile_props(player.x + PLAYER_HITBOX_WIDTH + 1, player.y);
        uint8_t p_bot = get_tile_props(player.x + PLAYER_HITBOX_WIDTH + 1, player.y + PLAYER_HITBOX_HEIGHT - 1);
        uint8_t p_mid = get_tile_props(player.x + PLAYER_HITBOX_WIDTH + 1, player.y + (PLAYER_HITBOX_HEIGHT/2));

        if (!((p_top | p_bot | p_mid) & PROP_SOLID))
        {
            player.x += 4;
        }
    }

    if (pressed_left)
    {
        uint8_t p_top = get_tile_props(player.x - 1, player.y);
        uint8_t p_bot = get_tile_props(player.x - 1, player.y + PLAYER_HITBOX_HEIGHT - 1); 
        uint8_t p_mid = get_tile_props(player.x - 1, player.y + (PLAYER_HITBOX_HEIGHT/2)); 

        if (!((p_top | p_bot | p_mid) & PROP_SOLID))
        {
            player.x += -4;
        }
    }


    int rel_x = player.x - player.scrollx;
    int rel_y = player.y - player.scrolly;

    player.rel_x = rel_x;
    player.rel_y = rel_y;

    // horizontal follow
    if (rel_x > (GFX_LCD_WIDTH - SCROLL_MARGIN_X - player.hitbox.width)) player.scrollx += 4;
    if (rel_x < SCROLL_MARGIN_X)                               player.scrollx -= 4;

    // vertical follow (if you have a 16px UI bar, treat your “viewport” as starting at y=16)
    if (rel_y > (SCREEN_HEIGHT - SCROLL_MARGIN_Y - player.hitbox.height)) player.scrolly += 4;
    if (rel_y < SCROLL_MARGIN_Y)                                player.scrolly -= 4;

    // clamp camera
    if (player.scrollx < 0) player.scrollx = 0;
    if (player.scrolly < 0) player.scrolly = 0;

    int max_scroll_x = TILEMAP_WIDTH*TILE_WIDTH - TILEMAP_DRAW_WIDTH*TILE_WIDTH;
    int max_scroll_y = TILEMAP_HEIGHT*TILE_HEIGHT - TILEMAP_DRAW_HEIGHT*TILE_HEIGHT;
    if (player.scrollx > max_scroll_x) player.scrollx = max_scroll_x;
    if (player.scrolly > max_scroll_y) player.scrolly = max_scroll_y;

    if ((get_tile_props(player.x, player.y + PLAYER_HITBOX_HEIGHT - 1) | 
        get_tile_props(player.x + 16 - 1, player.y + PLAYER_HITBOX_HEIGHT - 1)) & PROP_DANGER)
    {
        game_over = true;
    }
}

