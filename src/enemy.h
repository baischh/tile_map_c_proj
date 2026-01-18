#include <stdint.h>
#include "game_sprite.h"
#include "defines.h"


typedef enum {
    CACTUS_MAN,
    FIRE_GUY
} enemy_type;

typedef struct {
    uint8_t type;
    int spawn_x, spawn_y;
} enemy_template_t;

typedef struct {
    bool active;
    hitbox_t hitbox;
    uint8_t type;
    int x; // world postion
    int y;  
    int rel_x; // on screen position
    int rel_y;
    int vy;
    int vx;
} enemy_t;

void detect_enemy_collision(void);
void move_enemy(enemy_t *e);
void update_enemies();
void detect_collision_tilemap(void);

extern const enemy_template_t spawn_list[];
extern enemy_t active_enemies[MAX_ACTIVE_ENEMIES];