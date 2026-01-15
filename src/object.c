#include <stdbool.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>

#include "tile_handlers.h"
#include "defines.h"
#include "player.h"
#include "objects.h"

bool moving_forward = true;

/**
 * Detects collision with the moving platform
 */
void detect_object_collision(void)
{
    if
    (
        player.x < platform.x + 32 &&
        player.x + player.hitbox.width > platform.x &&
        player.y < platform.y + 16 &&
        player.y + player.hitbox.height > platform.y
    )
    {
        player_did_collide = true;
    }
}


void move_objects(void)
{
   if(moving_forward) 
   {
        platform.x++;
   }
   else if (!moving_forward)
   {
    platform.x--;
   }

   if(moving_forward && platform.x >= 128)
   {
    moving_forward = false;
   }

   if(!moving_forward && platform.x <= 64)
   {
    moving_forward = true;
   }
}