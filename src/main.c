#include <ti/getcsc.h>
#include <graphx.h>
#include <sys/util.h>
#include <keypadc.h>

/* Include the converted graphics file */
#include "gfx/gfx.h"

/* Include the external tilemap data */
extern unsigned char tilemap_map[];

/* Tilemap defines */
#define TILE_WIDTH          16
#define TILE_HEIGHT         16

#define TILEMAP_WIDTH       20
#define TILEMAP_HEIGHT      20 

#define TILEMAP_DRAW_WIDTH  20
#define TILEMAP_DRAW_HEIGHT 14

#define Y_OFFSET            16 // for position bar ui element
#define X_OFFSET            0


#define START_X ((GFX_LCD_WIDTH - bug_width) / 2)
#define START_Y ((GFX_LCD_HEIGHT - bug_height) / 2)

/* Create a buffer to store the background behind the sprite */
gfx_UninitedSprite(background, bug_width, bug_height);

/* Prototype for draw sprite */
void DrawSprite(int x, int y);

int main(void)
{
    uint8_t key;
    unsigned int x_offset = 0;
    unsigned int y_offset = 0;
    gfx_tilemap_t tilemap;

    background->width = bug_width;
    background->height = bug_height;

    /* Coordinates used for the sprite */
    int x = START_X + x_offset;
    int y = START_Y + y_offset;

    /* Initialize the tilemap structure */
    tilemap.map         = tilemap_map;
    tilemap.tiles       = minimal_tileset_tiles;
    tilemap.type_width  = gfx_tile_16_pixel;
    tilemap.type_height = gfx_tile_16_pixel;
    tilemap.tile_height = TILE_HEIGHT;
    tilemap.tile_width  = TILE_WIDTH;
    tilemap.draw_height = TILEMAP_DRAW_HEIGHT;
    tilemap.draw_width  = TILEMAP_DRAW_WIDTH;
    tilemap.height      = TILEMAP_HEIGHT;
    tilemap.width       = TILEMAP_WIDTH;
    tilemap.y_loc       = Y_OFFSET;
    tilemap.x_loc       = X_OFFSET;

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

        /* Draw tilemap and coords */
        gfx_Tilemap_NoClip(&tilemap, x_offset, y_offset);
        gfx_FillRectangle(0, 0, 320, 16);
        gfx_PrintStringXY("x:", 64, 4);
        gfx_PrintInt(x_offset, 3);
        gfx_PrintString(" y:");
        gfx_PrintInt(y_offset, 3);
        gfx_PrintString(" block:");

        /* Or use gfx_GetTileMapped() and gfx_GetTile() */
        block_mapped = *gfx_TilePtrMapped(&tilemap, x_offset / TILE_WIDTH, y_offset / TILE_HEIGHT);
        block_ptr = *gfx_TilePtr(&tilemap, x_offset, y_offset);

        gfx_PrintUInt(block_mapped, 3);
        gfx_PrintString("/");
        gfx_PrintUInt(block_ptr, 3);

        /* write the initial sprite background to the background buffer */
        gfx_GetSprite(background, x, y);

        DrawSprite(x, y);

        /* Do something based on the keypress */
        switch (key)
        {
            case sk_Down:
                if (y_offset < (TILEMAP_HEIGHT * TILE_HEIGHT) - (TILEMAP_DRAW_HEIGHT * TILE_HEIGHT))
                {
                    y_offset += TILE_HEIGHT;
                }
                break;

            case sk_Left:
                if (x_offset)
                {
                    x_offset -= TILE_WIDTH;
                }
                break;

            case sk_Right:
                if (x_offset < (TILEMAP_WIDTH * TILE_WIDTH) - (TILEMAP_DRAW_WIDTH * TILE_WIDTH))
                {
                    x_offset += TILE_WIDTH;
                }
                break;

            case sk_Up:
                if (y_offset)
                {
                    y_offset -= TILE_HEIGHT;
                }
                break;

            default:
                break;
        }

        gfx_SwapDraw();

    } while (key != sk_Enter);

    /* End graphics drawing */
    gfx_End();

    return 0;
}

/* Function for drawing the main sprite */
void DrawSprite(int x, int y)
{
    // static int oldX = START_X;
    // static int oldY = START_Y;

    // /* Render the original background */
    // gfx_Sprite(background, oldX, oldY);

    // /* Get the background behind the sprite */
    // gfx_GetSprite(background, x, y);

    /* Render the sprite */
    gfx_TransparentSprite(bug, x, y);

    // oldX = x;
    // oldY = y;
}