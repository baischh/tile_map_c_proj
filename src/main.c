#include <ti/getcsc.h>
#include <graphx.h>
#include <sys/util.h>
#include <keypadc.h>
#include "player.h"
#include "objects.h"
#include "defines.h"

/* Include the converted graphics file */
#include "gfx/gfx.h"

/* Include the external tilemap data */
extern unsigned char tilemap_map[];

player_t player;
object_t platform;

bool game_over = true;

/* Prototype for draw sprite */
void draw_sprite(int x, int y);
void init_player(void);
void init_objects(void);
void draw_platform(void);

int main(void)
{
    uint8_t key;
    gfx_tilemap_t tilemap;

    init_player();
    init_objects();

    /* Initialize the tilemap structure */
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
    tilemap.y_loc       = player.scrolly;
    tilemap.x_loc       = player.scrollx;

    /* Initialize graphics drawing */
    gfx_Begin();

    /* Set the palette */
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetColor(1);
    gfx_SetTextFGColor(2);
    gfx_SetTextBGColor(1);
    gfx_SetTransparentColor(0);

    /* Draw to buffer to avoid tearing */
    gfx_SetDrawBuffer();

    /* Set monospace font with width of 8 */
    gfx_SetMonospaceFont(8);

    /* Wait for the enter key to quit */
    do
    {
        uint8_t block_mapped;
        uint8_t block_ptr;

        /* Get the key */
        key = os_GetCSC();

        kb_key_t arrows;
        kb_key_t g1_key, g2_key, g3_key, g7_key;

        /* Scan the keypad to update kb_Data */
        kb_Scan();

        g1_key = kb_Data[1];
        g2_key = kb_Data[2];
        g7_key = kb_Data[7];
        pressed_2nd = (g1_key & kb_2nd);
        pressed_down = (g7_key & kb_Down);
        pressed_left = (g7_key & kb_Left);
        pressed_right = (g7_key & kb_Right);

        /* Get the arrow key statuses */
        move_player();
        move_objects();

        gfx_Tilemap(&tilemap, player.scrollx, player.scrolly);
        draw_sprite(player.rel_x, player.rel_y);
        draw_platform();
        gfx_SwapDraw();
        if (game_over)
        {
            init_player();
        }

    } while (key != sk_Enter);

    /* End graphics drawing */
    gfx_End();

    return 0;
}

void init_player(void)
{
    /* Coordinates used for the sprite */
    player.x = START_X;
    player.y = START_Y;
    player.hitbox.width = PLAYER_HITBOX_WIDTH;
    player.hitbox.height = PLAYER_HITBOX_HEIGHT;
    player.scrollx = 0;
    player.scrolly = 0;

    game_over = false;
}

void init_objects(void)
{
    platform.bounding_box.width = 32;
    platform.bounding_box.height = 16;
    platform.x = 64;
    platform.y = 176;
}

/* Function for drawing the main sprite */
void draw_sprite(int x, int y)
{
    gfx_TransparentSprite(Sprite0003, x, y);
}

void draw_platform(void)
{
    int x = platform.x - player.scrollx;
    int y = platform.y - player.scrolly;
    gfx_TransparentSprite(brick_platform, x, y);
}