IF !DEF(TEST_LIST_INC)
TEST_LIST_INC SET 1

INCLUDE "character_move_test.inc"

SECTION "test_directory", ROM0
test_defs: 
.begin:
.first_begin:
  DW position_to_background_pointer_test_name, position_to_background_pointer_test
.first_end:
  DW y_velocity_decay_test_name, y_velocity_decay_test
.end:

sizeof_test_defs EQU (test_defs.end - test_defs.begin)
sizeof_single_test_def EQU (test_defs.first_end - test_defs.first_begin)
number_of_tests EQU sizeof_test_defs / sizeof_single_test_def

SECTION "test_names", ROM0
position_to_background_pointer_test_name: DB "pos to bckgrnd", $0
x_velocity_decay_test_name:               DB "DEP X velocity decay",$0
y_velocity_decay_test_name:               DB "DEP Y velocity decay",$0

ENDC ; TEST_LIST_INC
