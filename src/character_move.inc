IF !DEF(CHARACTER_MOVE_INC)
CHARACTER_MOVE_INC SET 1

INCLUDE "register_utils.inc"
INCLUDE "oam_utils.inc"
INCLUDE "joypad_eval.inc"
INCLUDE "tileset.inc"

SECTION "character_move mode variables", WRAM0
character_move_variables_start:
character_sprite:
.y: DS 1
.x: DS 1
.tile: DS 1
.flags: DS 1
.number: DS 1
.end:
character_velocity:
.y: DS 1
.x: DS 1
character_move_in_mode: DS 1
character_move_scroll:
.x_min: DS 1
.x_max: DS 1
.y_min: DS 1
.y_max: DS 1
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

    ld a, ((character.back - tileset_start) / TS)
    ld [character_sprite.tile], a
    
    xor a
    ld [character_sprite.flags], a

    jp character_move_mode_update_sprite

SECTION "character_move mode down callback", ROM0
character_move_down_cb:
    ld a, $1
    ld [character_velocity.y], a
    
    ld a, ((character.front - tileset_start) / TS)
    ld [character_sprite.tile], a

    xor a
    ld [character_sprite.flags], a
    
    jp character_move_mode_update_sprite

SECTION "character_move mode left callback", ROM0
character_move_left_cb:
    ld a, $FF
    ld [character_velocity.x], a

    ld a, ((character.side - tileset_start) / TS)
    ld [character_sprite.tile], a

    ld a, %00100000 ; flip horizontally
    ld [character_sprite.flags], a

    jp character_move_mode_update_sprite

SECTION "character_move mode right callback", ROM0
character_move_right_cb:
    ld a, $1
    ld [character_velocity.x], a

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

SECTION "character_move mode timing update", ROM0
character_move_mode_time_cb:
    ld a, [bc]
    or a
    ret z

.check_scroll_x:    
    ld a, [character_sprite.x]
    ld hl, character_velocity.x
    add [hl]

    ld hl, character_move_scroll.x_min
    cp [hl]
    jr z, .scroll_x

    inc hl ; ld hl, character_move_scroll.x_max
    cp [hl]
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
    ld a, [character_sprite.y]
    ld hl, character_velocity.y
    add [hl]

    ld hl, character_move_scroll.y_min
    cp [hl]
    jr z, .scroll_y

    inc hl ; ld hl, character_move_scroll.y_max
    cp [hl]
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

    jp character_move_mode_update_sprite

SECTION "character_move mode init", ROM0
init_character_move_mode:
    ld hl, character_move_variables_start
    ld a, $10 + $40
    ld [hl+], a
    ld a, $8 + $40
    ld [hl+], a
     
    ld a, ((character.front - tileset_start) / TS)
    ld [hl+], a
    
    xor a
    ld [hl+], a
    ld [hl+], a
    ld [hl+], a
    ld [hl+], a
    ld [hl+], a

    ld a, $8 + $20
    ld [hl+], a
    ld a, $A0 - $20
    ld [hl+], a

    ld a, $10 + $20
    ld [hl+], a
    ld a, $98 - $20
    ld [hl+], a

    ld hl, character_move_mode_time_cb
    ld bc, character_move_in_mode
    ld de, $1
    call add_timing_table_entry_callback

    ret
ENDC ; CHARACTER_MOVE_INC
