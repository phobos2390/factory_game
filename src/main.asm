; main area
INCLUDE "vblank_utils.inc"
INCLUDE "hardware.inc"
INCLUDE "register_utils.inc"
INCLUDE "print_utils.inc"
INCLUDE "constants.inc"
INCLUDE "interrupt_vectors.inc"
INCLUDE "vram_utils.inc"
INCLUDE "joypad_eval.inc"
INCLUDE "oam_utils.inc"
INCLUDE "timing_utils.inc"
INCLUDE "sram_utils.inc"
INCLUDE "model_number_util.inc"

INCLUDE "tileset.inc"
INCLUDE "character_move.inc"
INCLUDE "map.inc"

SECTION "Game code", ROM0
main:
  ld [GB_model_number], a
  call init_display
  call init_font
  call init_window
  call init_vblank_list
  call init_timing_table
  
  call init_vram_iterator

  call init_joypad_table
  call init_callbacks

  call init_sprite_table
  
  call timer_cb_init

  call init_vblank_callback

  call user_init

  call final_init
  ei

  jp main_loop


SECTION "user data init", ROM0
user_init:
  ld hl, map_tiles.start
  ld de, background_start
  ld bc, map_tiles.end - map_tiles.start
  call memcopy

  call init_character_move_mode
  call set_character_move_mode

  ret

SECTION "INIT vblank callback", ROM0
init_vblank_callback:
  ld hl, timing_table_cb
  LD_I_ADDR_R16 vblank_f, hl
  ret

SECTION "vblank_callback", ROM0
vblank_callback:
  call read_joypad
  call eval_held_button
  call eval_rising_edge
  call dma_update_sprites
  ret

SECTION "INIT timers", ROM0
timer_cb_init:
  ld hl, vblank_callback
  ld bc, 0
  ld de, $1
  call add_timing_table_entry_callback

  ret

SECTION "Sprite pallete", ROM0
regular_pallete:
DW %0111111111111111
DW %0000011111100000
DW %0111110000000000
DW %0000000000000000

SECTION "set OAM sprite pallete", ROM0
init_sprite_pallete:
  ld a, [GB_model_number]
  cp $1
  ret z

  ld a, [rIE]
  push af
    ld a, IEF_VBLANK
    ld [rIE], a
    halt
    halt
  pop af
  ld [rIE], a

  ld a, %10000000
  ld [rOCPS], a

  ld c, LOW(rOCPD)

  call init_sprite_pallete.subroutine
  xor a
  ld [rOCPS], a

  ld a, %10000000
  ld [rBCPS], a

  ld c, LOW(rBCPD)
  call init_sprite_pallete.subroutine
  xor a
  ld [rBCPS], a

  ret

.subroutine:
  ld b, $8
.loop:
  ld hl, regular_pallete
  ld a, [hl+]
  ldh [c], a
  ld a, [hl+]
  ldh [c], a
  ld a, [hl+]
  ldh [c], a
  ld a, [hl+]
  ldh [c], a
  ld a, [hl+]
  ldh [c], a
  ld a, [hl+]
  ldh [c], a
  ld a, [hl+]
  ldh [c], a
  ld a, [hl]
  ldh [c], a
  dec b
  jr nz, .loop
  ret

  
  ret

SECTION "Initialize display", ROM0
init_display:
  VBLANK_WAIT

  xor a ; ld a, 0 ; We only need to reset a value with bit 7 reset, but 0 does the job
  ld [rLCDC], a ; We will have to write to LCDC again later, so it's not a bother, really.
  ret

SECTION "Initialize font", ROM0
init_font:
;  bc - IN & OUT size
;  de - destination pointer
  ld hl, tileset_start
  ld de, tileset_vram_start
  ld bc, tileset_end - tileset_start
  call memcopy_to_vram
  
  ret

SECTION "Finalize initialization", ROM0
final_init:
  ; Init display registers
  ld a, %11100100
  ld [rBGP], a
  ld [rOBP0], a
  ld [rOBP1], a

  call init_sprite_pallete

  xor a ; ld a, 0
  ld [rSCY], a
  ld [rSCX], a

  ; Shut sound down
  ld [rNR52], a

  ; Turn on screen, background,  bg data       window data, enable window, and sprite, tileset_select
  ld a, LCDCF_ON | LCDCF_BGON | LCDCF_BG9800 | LCDCF_WIN9C00| LCDCF_WINON | LCDCF_OBJON | LCDCF_BG8000
  ld [rLCDC], a

  ld a, [rTAC]
  or a, TACF_START ; enable timer
  ld [rTAC], a

  ld a, [rIE]
  or IEF_HILO | IEF_SERIAL | IEF_TIMER | IEF_LCDC | IEF_VBLANK
  ld [rIE], a

  ret
