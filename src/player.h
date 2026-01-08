#include <stdint.h>

extern bool pressed_left;
extern bool pressed_right;
extern bool pressed_up;
extern bool pressed_down;
extern bool pressed_alpha;
extern bool pressed_2nd;

extern bool allow_up_press;

void move_player(void);

typedef struct {
   uint8_t width, height;
} hitbox_t;

typedef struct {
    hitbox_t hitbox;
    int x; // world postion
    int y;  
    int rel_x; // on screen position
    int rel_y;
    int scrollx; // camera position
    int scrolly;
    int vy;
    int vx;
    int8_t momentum;
} player_t;

extern player_t player;