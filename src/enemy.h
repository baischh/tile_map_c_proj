#include <stdint.h>
#include "character.h"

void detect_enemy_collision(void);
void move_enemie(void);

typedef struct {
    hitbox_t hitbox;
    int x; // world postion
    int y;  
    int rel_x; // on screen position
    int rel_y;
    int vy;
    int vx;
} enemie_t;

extern enemie_t enemy;