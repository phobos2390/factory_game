IF !DEF(CHARACTER_MOVE_INC)
CHARACTER_MOVE_INC SET 1

INCLUDE "register_utils.inc"
INCLUDE "oam_utils.inc"
INCLUDE "joypad_eval.inc"
INCLUDE "tileset.inc"
INCLUDE "timing_utils.inc"

SECTION "character_move mode variables", WRAM0
character_move_variables_start:
character_pos:
.y: DS 1
.x: DS 1
character_sprite:
.y: DS 1
.x: DS 1
.tile: DS 1
.flags: DS 1
.number: DS 1
.end:
p_map_position:
.low: DS 1
.high: DS 1
p_map_velocity:
.low: DS 1
.high: DS 1
character_velocity:
.y: DS 1
.x: DS 1
character_move_in_mode: DS 1
character_move_scroll:
.x_min: DS 1
.x_max: DS 1
.y_min: DS 1
.y_max: DS 1
character_variables:
.background_save: DS 1
character_move_variables_end:

SECTION "character_move mode a callback", ROM0
character_move_a_cb:
    ret

SECTION "character_move mode b callback", ROM0
character_move_b_cb:
    ret

SECTION "character_move mode start callback", ROM0
character_move_start_cb:
    ret

SECTION "character_move mode select callback", ROM0
character_move_select_cb:
    ret

SECTION "character_move mode up callback", ROM0
character_move_up_cb:
    ld a, $FF
    ld [character_velocity.y], a

    push bc
      ld bc, $FFE0
      ld a, [p_map_velocity.low]
      ld l, a
      ld a, [p_map_velocity.high]
      ld h, a
      add hl, bc      
    pop bc
    ld a, l
    ld [p_map_velocity.low], a
    ld a, h
    ld [p_map_velocity.high], a

    ld a, ((character.back - tileset_start) / TS)
    ld [character_sprite.tile], a
    
    xor a
    ld [character_sprite.flags], a

    jp character_move_mode_update_sprite

SECTION "character_move mode down callback", ROM0
character_move_down_cb:
    ld a, $1
    ld [character_velocity.y], a

    push bc
      ld bc, $20
      ld a, [p_map_velocity.low]
      ld l, a
      ld a, [p_map_velocity.high]
      ld h, a
      add hl, bc      
    pop bc
    ld a, l
    ld [p_map_velocity.low], a
    ld a, h
    ld [p_map_velocity.high], a
    

    ld a, ((character.front - tileset_start) / TS)
    ld [character_sprite.tile], a

    xor a
    ld [character_sprite.flags], a
    
    jp character_move_mode_update_sprite

SECTION "character_move mode left callback", ROM0
character_move_left_cb:
    ld a, $FF
    ld [character_velocity.x], a

    push bc
      ld bc, $FFFF
      ld a, [p_map_velocity.low]
      ld l, a
      ld a, [p_map_velocity.high]
      ld h, a
      add hl, bc      
    pop bc
    ld a, l
    ld [p_map_velocity.low], a
    ld a, h
    ld [p_map_velocity.high], a
    


    ld a, ((character.side - tileset_start) / TS)
    ld [character_sprite.tile], a

    ld a, %00100000 ; flip horizontally
    ld [character_sprite.flags], a

    jp character_move_mode_update_sprite

SECTION "character_move mode right callback", ROM0
character_move_right_cb:
    ld a, $1
    ld [character_velocity.x], a

    push bc
      ld bc, $1
      ld a, [p_map_velocity.low]
      ld l, a
      ld a, [p_map_velocity.high]
      ld h, a
      add hl, bc
    pop bc
    ld a, l
    ld [p_map_velocity.low], a
    ld a, h
    ld [p_map_velocity.high], a

    ld a, ((character.side - tileset_start) / TS)
    ld [character_sprite.tile], a

    xor a
    ld [character_sprite.flags], a

    jp character_move_mode_update_sprite

SECTION "character_move mode update sprite", ROM0
character_move_mode_update_sprite:
;   b - x position
;   c - y position
;   d - character tile 
;   e - flags
;   a - sprite number
    push hl
        ld hl, character_sprite
        ld c, [hl] ; ld c, [character_sprite.y]
        inc hl
        ld b, [hl] ; ld b, [character_sprite.x]
        inc hl
        ld d, [hl] ; ld d, [character_sprite.tile]
        inc hl
        ld e, [hl] ; ld e, [character_sprite.flags]
        inc hl
        ld a, [hl] ; ld e, [character_sprite.number] 
    pop hl
    call set_sprite_a
    ret

SECTION "character_move mode set", ROM0
set_character_move_mode:
    ld hl, pad_up_f
    ld de, character_move_up_cb
    call ld_ihl_de

    ld hl, pad_down_f
    ld de, character_move_down_cb
    call ld_ihl_de

    ld hl, pad_left_f
    ld de, character_move_left_cb
    call ld_ihl_de

    ld hl, pad_right_f
    ld de, character_move_right_cb
    call ld_ihl_de

    ld hl, button_a_f
    ld de, character_move_a_cb
    call ld_ihl_de

    ld hl, button_b_f
    ld de, character_move_b_cb
    call ld_ihl_de

    ld hl, button_start_f
    ld de, character_move_start_cb
    call ld_ihl_de

    ld hl, button_select_f
    ld de, character_move_select_cb
    call ld_ihl_de

    ld a, $1
    ld [character_move_in_mode], a
    inc a
    ld [held_refresh_rate], a

    ret

SECTION "position to bckground", ROM0
; b - pos.y
; c - pos.x
; bc - bc to pointer offset
position_bc_to_background_pointer_bc:
  ld a, b
  and $F8
  ld b, a
  ld a, c
  jr position_to_background_pointer.calculate
  
; hl - pos.y, pos.x
; bc - returns bc as pointer offset
position_to_background_pointer:
  ld a, [hl+] ; ld a, [character_pos.y], hl -> character_pos.x
  and $F8     ; a: 0bYYYY Y000
  ld b, a     ; b: 0bYYYY Y000
  ld a, [hl] ; ld a, [character_pos.x]
.calculate:
  srl a       ; a: 0b0XXX XXXX
  srl a       ; a: 0b00XX XXXX
  srl a       ; a: 0b000X XXXX
  ld c, a     ; c: 0b000X XXXX
  ld a, b     ; a: 0bYYYY Y000
  sla a       ; a: 0bYYYY 0000
  sla a       ; a: 0bYYY0 0000
  or c        ; a: 0bYYYX XXXX
  ld c, a     ; c: 0bYYYX XXXX bc: 0bYYYY Y000 YYYX XXXX
  ld a, b     ; a: 0bYYYY Y000
  swap a      ; a: 0bY000 YYYY
  sra a       ; a: 0b0Y00 0YYY
  sra a       ; a: 0b00Y0 00YY
  and $03     ; a: 0b0000 00YY
  ld b, a     ; b: 0b0000 00YY bc: 0b0000 00YY YYYX XXXX 
  ret

SECTION "character_move mode timing update", ROM0
character_move_mode_time_cb:
    ld a, [bc]
    or a
    ret z

    ld hl, character_velocity
    ld a, [hl+]
    or a, [hl]
    jp z, character_move_mode_update_sprite

.check_position_open:
    ld a, [character_pos.y]
    ld d, a
    ld hl, character_velocity.y
    add a, [hl]
    and a, $F8
    ld b, a
    xor a, d
    and a, $F8
    jr nz, .changed

    ld a, [character_pos.x]
    ld e, a
    ld hl, character_velocity.x
    add a, [hl]
    and a, $F8
    ld c, a
    xor a, e
    and a, $F8
    jr z, .check_scroll_x

.changed:
    ld hl, character_pos    
    call position_to_background_pointer
    ld hl, background_start
    add hl, bc
    
    ld a, [character_variables.background_save]
    ld [hl], a
   
    ld hl, character_pos.y
    ld b, [hl] ; ld b, [character_pos.y]
    inc hl
    ld c, [hl] ; ld c, [character_pos.x]
    ld hl, character_velocity.y
    ld a, b
    add a, [hl]
    ld b, a
    inc hl
    ld a, c
    add a, [hl]
    ld c, a
    call position_bc_to_background_pointer_bc

    ld hl, background_start
    add hl, bc

    push hl
      ld a, [hl] ; gets map tile character is over
      ld hl, map_tile_passable_table
      ld b, $0
      ld c, a
      add hl, bc
      ld a, [hl]
    pop hl
    or a
    jp z, .slow_character

    ld a, [hl]
    ld [character_variables.background_save], a
    ld a, [character_sprite.tile]
    ld [hl], a

    ld a, l
    ld [p_map_position.low], a
    ld a, h
    ld [p_map_position.high], a

.check_scroll_x:    
    ld a, [character_pos.x]
    ld hl, character_velocity.x
    add [hl]
    ld [character_pos.x], a

    ld a, [character_sprite.x]
    add [hl]

    ld hl, character_move_scroll.x_min
    cp [hl]
    jr z, .scroll_x
    jr c, .scroll_x

    inc hl ; ld hl, character_move_scroll.x_max
    cp [hl]
    jr nc, .scroll_x
    jr nz, .update_sprite_x

.scroll_x:
    ld c, LOW(rSCX)
    ld hl, character_velocity.x
    ld a, [c]
    add [hl]
    ld [c], a
    jr .check_scroll_y

.update_sprite_x:
    ld [character_sprite.x], a

.check_scroll_y:
    ld a, [character_pos.y]
    ld hl, character_velocity.y
    add [hl]
    ld [character_pos.y], a

    ld a, [character_sprite.y]
    add [hl]

    ld hl, character_move_scroll.y_min
    cp [hl]
    jr z, .scroll_y
    jr c, .scroll_y

    inc hl ; ld hl, character_move_scroll.y_max
    cp [hl]
    jr nc, .scroll_y
    jr nz, .update_sprite_y

.scroll_y:
    ld c, LOW(rSCY)
    ld hl, character_velocity.y
    ld a, [c]
    add [hl]
    ld [c], a
    jr .slow_character

.update_sprite_y:
    ld [character_sprite.y], a

.slow_character:
    xor a
    ld [character_velocity.x], a
    ld [character_velocity.y], a
    ld [p_map_velocity.low], a
    ld [p_map_velocity.high], a

.end:
    jp character_move_mode_update_sprite

SECTION "character_move mode init", ROM0
init_character_move_mode:
    ld hl, character_move_variables_start
    ld a, $44
    ld [hl+], a      ; ld [character_pos.y], middle of screen
    ld [hl+], a      ; ld [character_pos.x], middle of screen
    ld a, $10 + $40
    ld [hl+], a      ; ld [character_sprite.y], middle of screen
    ld a, $8 + $40
    ld [hl+], a      ; ld [character_sprite.x], middle of screen
     
    ld a, ((character.front - tileset_start) / TS)
    ld [hl+], a      ; ld [character_sprite.tile], character_front_tile
    
    xor a
    ld [hl+], a      ; ld [character_sprite.flags], 0
    ld [hl+], a      ; ld [character_sprite.number], 0

    ; $9908
    ld a, LOW(background_start + ($44 / $8) + ($40 * back_width / $8))
    ld [hl+], a      ; ld [p_map_position.low], LOW(pos)
    ld a, HIGH(background_start + ($44 / $8) + ($40 * back_width / $8))
    ld [hl+], a      ; ld [p_map_position.high], HIGH(pos)
    
    xor a
    ld [hl+], a      ; ld [p_map_velocity.low], 0
    ld [hl+], a      ; ld [p_map_velocity.high], 0

    ld [hl+], a      ; ld [character_velocity.x], 0
    ld [hl+], a      ; ld [character_velocity.y], 0
    ld [hl+], a      ; ld [character_move_in_mode], 0

    ld a, $8 + $20 - $1
    ld [hl+], a      ; ld [character_move_scroll.x_min]
    ld a, $A0 - $20 + $1
    ld [hl+], a      ; ld [character_move_scroll.x_max]

    ld a, $10 + $20 - $1
    ld [hl+], a      ; ld [character_move_scroll.y_min]
    ld a, $98 - $20 + $1
    ld [hl+], a      ; ld [character_move_scroll.y_max]

    ld a, [background_start + ($40 / $8) + ($40 * back_width / $8)]
    ld [hl+], a      ; ld [character_variables.background_save], a

    ld hl, character_move_mode_time_cb
    ld bc, character_move_in_mode
    ld de, $1
    call add_timing_table_entry_callback

    ret
ENDC ; CHARACTER_MOVE_INC
