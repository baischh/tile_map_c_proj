NAME = minmap
ICON = icon.png
DESCRIPTION = "Platformer style game"
COMPRESSED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

EXTRA_C_SOURCES = src/tilemap/tilemap.c

include $(shell cedev-config --makefile)

# rule to convert CSV tilemap to C source file
src/tilemap/tilemap.c: src/tilemap/level02.csv
	@convbin -j csv -i $< -k c -o $@ -n tilemap_map
