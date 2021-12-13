IF !DEF (CHARACTER_MOVE_TEST_INC)
CHARACTER_MOVE_TEST_INC SET 1

INCLUDE "character_move.inc"

SECTION "x velocity decay test", ROM0
x_velocity_decay_test:
.succeed:
  ld hl, 1
  ret
.fail:
  ld hl, 0  
  ret

SECTION "y velocity decay test", ROM0
y_velocity_decay_test:
  ld hl, 1
  ret

SECTION "position to bckground test", ROM0
position_to_background_pointer_test:
  ld a, $40
  ld [character_pos.y], a
  ld [character_pos.x], a
  ld hl, character_pos.y

  call position_to_background_pointer
  
expected_value SET ($40 / $8) + (($40 / $8) * $20)
  ld a, LOW(expected_value)
  xor c
  jr nz, .fail
  ld a, HIGH(expected_value)
  xor b
  jr nz, .fail
.succeed:
  ld hl, 1
  ret
.fail:
  ld hl, 0  
  ret

SECTION "Error strings", ROM0
first_pass:  DB "\nfailed first pass", $0
second_pass: DB "\nfailed second pass", $0
third_pass: DB "\nfailed third pass", $0
fourth_pass: DB "\nfailed fourth pass", $0

ENDC
