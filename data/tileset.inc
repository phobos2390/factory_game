IF !DEF(TILESET_INC)
TILESET_INC SET 1

SECTION "tileset", ROM0
OPT g.123
tileset_start:
INCLUDE "background.inc"
INCLUDE "ore_tiles.inc"
INCLUDE "pipe_tiles.inc"
INCLUDE "furnace.inc"
INCLUDE "miner.inc"
INCLUDE "assembler.inc"
INCLUDE "character.inc"
tileset_end:

TS SET $10

TILE_VALUE_FROM_ADDR : MACRO
  ((\1 - tileset_start) / TS)
ENDM

TGRS SET (background_tiles.grass - tileset_start) / TS
TWAT SET (background_tiles.water - tileset_start) / TS
TVRE SET (background_tiles.vertical_edge - tileset_start) / TS
TDLE SET (background_tiles.down_left_edge - tileset_start) / TS
THOE SET (background_tiles.horizontal_edge - tileset_start) / TS
TRDE SET (background_tiles.right_down_edge - tileset_start) / TS
TURE SET (background_tiles.up_right_edge - tileset_start) / TS
TLUE SET (background_tiles.left_up_edge - tileset_start) / TS
TCHF SET TGRS ; character is sprite
TFEO SET (ore_tiles_start.iron_ore - tileset_start) / TS
TCUO SET (ore_tiles_start.copper_ore - tileset_start) / TS
TCOL SET (ore_tiles_start.coal_ore - tileset_start) / TS
TFJP SET (pipe_tile.full_junction - tileset_start) / TS
TLGP SET (pipe_tile.left_to_ground - tileset_start) / TS
TRGP SET (pipe_tile.right_to_ground - tileset_start) / TS
TDGP SET (pipe_tile.down_to_ground - tileset_start) / TS
TUGP SET (pipe_tile.up_to_ground - tileset_start) / TS
TLRP SET (pipe_tile.left_right_pipe - tileset_start) / TS
TUDP SET (pipe_tile.up_down_pipe - tileset_start) / TS
TLUP SET (pipe_tile.left_up_pipe - tileset_start) / TS
TURP SET (pipe_tile.up_right_pipe - tileset_start) / TS
TDLP SET (pipe_tile.down_left_pipe - tileset_start) / TS
TRDP SET (pipe_tile.right_down_pipe - tileset_start) / TS
TLJP SET (pipe_tile.down_left_up_junction - tileset_start) / TS
TUJP SET (pipe_tile.left_up_right_junction - tileset_start) / TS
TRJP SET (pipe_tile.up_right_down_junction - tileset_start) / TS
TDJP SET (pipe_tile.right_down_left_junction - tileset_start) / TS
TFOF SET (furnace_tiles.off - tileset_start) / TS
TFON SET (furnace_tiles.on - tileset_start) / TS
TMOF SET (miner_tile.off - tileset_start) / TS
TMON SET (miner_tile.on - tileset_start) / TS
TAOF SET (assembler_tiles.off - tileset_start) / TS
TAON SET (assembler_tiles.on - tileset_start) / TS

ENDC
