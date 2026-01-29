#include <ti/getcsc.h>
#include <graphx.h>
#include <sys/util.h>
#include <keypadc.h>
#include "player.h"
#include "object.h"
#include "defines.h"
#include "enemy.h"
#include "gfx/gfx.h"

extern unsigned char tilemap_map[];

player_t player;
object_t platform;
int scroll_x;
int scroll_y;
uint16_t game_timer = 0;
uint8_t global_frame = 0;

gfx_sprite_t *enemy_sprites[] = {
    cactusman,
    fireguy
};

bool game_over = true;

void draw_sprite(int x, int y);
void init_player(void);
void init_objects(void);
void draw_platform(void);
void init_enemies(void);
void draw_enemy(void);

/**
 * Allocate resources for active enemies
 */
void spawn_manager()
{
    enemy_template_t *e = &spawn_list[0];

    for (char i = 0; i < MAX_ACTIVE_ENEMIES; i++, e++)
    {
        active_enemies[i] = (enemy_t){
            .active = true,
            .hitbox = (e->type == FIRE_GUY) ? {16, 32} : {16, 16},
            .type = e->type,
            .x = e->spawn_x,
            .y = e->spawn_y,
            .rel_x = e->spawn_x - scroll_x,
            .rel_y = e->spawn_y - scroll_y,
            .vx = 2,
            .vy = 0
        };
    }
}


// TODO

/**
 * Remove resources for inactive enemies
 */
void despawn_manager()
{
}


// TODO: Draw enemies be updated to use tilemap animations for all enemies
void draw_enemies()
{
    enemy_t *e = &active_enemies[0];
    for (char i = 0; i < MAX_ACTIVE_ENEMIES; i++, e++) {
        if (e->active)
        {
            if (e->type == FIRE_GUY)
            {
                gfx_TransparentSprite(fire_tiles[global_frame], e->rel_x, e->rel_y);
            }
            else
            {
                gfx_TransparentSprite(enemy_sprites[e->type], e->rel_x, e->rel_y);
            }
        }
    }
}


int main(void)
{
    uint8_t key;
    gfx_tilemap_t tilemap;

    init_player();
    init_objects();

    tilemap.map         = tilemap_map;
    tilemap.tiles       = blueskytileset02_tiles;
    tilemap.type_width  = gfx_tile_16_pixel;
    tilemap.type_height = gfx_tile_16_pixel;
    tilemap.tile_height = TILE_HEIGHT;
    tilemap.tile_width  = TILE_WIDTH;
    tilemap.draw_height = TILEMAP_DRAW_HEIGHT;
    tilemap.draw_width  = TILEMAP_DRAW_WIDTH;
    tilemap.height      = TILEMAP_HEIGHT;
    tilemap.width       = TILEMAP_WIDTH;
    tilemap.y_loc       = scroll_y;
    tilemap.x_loc       = scroll_x;

    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(0);
    gfx_SetDrawBuffer();

    spawn_manager();

    do
    {
        game_timer++;
        global_frame = (game_timer >> 1) & 7;

        uint8_t block_mapped;
        uint8_t block_ptr;
        kb_key_t arrows;
        kb_key_t g1_key, g2_key, g3_key, g7_key;

        key = os_GetCSC();

        kb_Scan();

        g1_key = kb_Data[1];
        g2_key = kb_Data[2];
        g7_key = kb_Data[7];
        pressed_2nd = (g1_key & kb_2nd);
        pressed_down = (g7_key & kb_Down);
        pressed_left = (g7_key & kb_Left);
        pressed_right = (g7_key & kb_Right);
        pressed_up = (g7_key & kb_Up);

        move_player();
        update_enemies();
        move_objects();
        detect_enemy_collision();
        gfx_Tilemap(&tilemap, scroll_x, scroll_y);
        draw_sprite(player.rel_x, player.rel_y);
        draw_enemies();
        detect_enemy_collision();
        gfx_SwapDraw();
        if (game_over)
        {
            init_player();
        }

    } while (key != sk_Enter);

    gfx_End();

    return 0;
}


/**
 * Init the player state
 */
void init_player(void)
{
    player.x = START_X;
    player.y = START_Y;
    player.hitbox.width = PLAYER_HITBOX_WIDTH;
    player.hitbox.height = PLAYER_HITBOX_HEIGHT;
    scroll_x = 0;
    scroll_y = 0;

    game_over = false;
}


/**
 * Init in game objects
 */
void init_objects(void)
{
    platform.bounding_box.width = 32;
    platform.bounding_box.height = 16;
    platform.x = 64;
    platform.y = 176;
}


/**
 * Draw a sprite at world coords
 */
void draw_sprite(int x, int y)
{
    gfx_TransparentSprite(Sprite0003, x, y);
}
