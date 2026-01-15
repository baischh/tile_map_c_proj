#include <stdbool.h>
#include <stdint.h>

#include "defines.h"
#include "enemy.h"

extern bool game_over;

/**
 * Detects collision with the moving enemy
 */
void detect_enemy_collision(void)
{
    if
    (
        player.x < enemy.x + 32 &&
        player.x + player.hitbox.width > enemy.x &&
        player.y < enemy.y + 16 &&
        player.y + player.hitbox.height > enemy.y
    )
    {
        game_over = true;
    }
}


void move_enemy(void)
{
   if(moving_forward) 
   {
        enemy.x++;
   }
   else if (!moving_forward)
   {
    enemy.x--;
   }

   if(moving_forward && enemy.x >= 128)
   {
    moving_forward = false;
   }

   if(!moving_forward && enemy.x <= 64)
   {
    moving_forward = true;
   }
}