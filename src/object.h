#include <stdint.h>

extern bool moving_forward;

extern void move_objects(void);

typedef struct {
    uint8_t width, height;
} bounding_box_t;

typedef struct {
    bounding_box_t bounding_box;
    int x;
    int y;
} object_t;

extern object_t platform;