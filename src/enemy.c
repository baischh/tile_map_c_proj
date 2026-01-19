#include <stdbool.h>
#include <stdint.h>

#include "defines.h"
#include "enemy.h"
#include "player.h"
#include "tile_handlers.h"

extern bool game_over;
extern int scroll_x, scroll_y;
extern player_t player;

enemy_t active_enemies[MAX_ACTIVE_ENEMIES];

void update_enemies()
{
    enemy_t *e = &active_enemies[0];

    for (char i = 0; i < MAX_ACTIVE_ENEMIES; i++, e++)
    {
        move_enemy(e);
    }
}


/**
 * 
 */
void detect_collison_tilemap()
{
}


/**
 * Detects collision with the moving enemy
 */
void detect_enemy_collision(void)
{
    enemy_t *e = &active_enemies[0];

    for (char i = 0; i < MAX_ACTIVE_ENEMIES; i++, e++)
    {
        if (e->active != true) continue;
        if (e->rel_x < -16 || e->rel_x > 320) continue; 

        if (
            player.x < e->x + e->hitbox.width &&
            player.x + player.hitbox.width > e->x &&
            player.y < e->y + e->hitbox.height &&
            player.y + player.hitbox.height > e->y
        ){
            game_over = true;
        }
    }

}


void move_enemy(enemy_t *e) {

    int next_x = e->x + e->vx;
    uint8_t p_side;
    uint8_t p_side_bot;

    if (e->vx < 0) {
        p_side = get_tile_props(next_x, e->y + 8);
        p_side_bot = get_tile_props(next_x, e->y + e->hitbox.height);
    } else {
        p_side = get_tile_props(next_x + e->hitbox.width, e->y + 8);
        p_side_bot = get_tile_props(next_x + e->hitbox.width, e->y + e->hitbox.height);
    }

    if ((p_side & PROP_SOLID) || !(p_side_bot & PROP_SOLID)) {
        e->vx = -e->vx;
    } else {
        e->x = next_x;
    }

    e->rel_x = e->x - scroll_x; 
}