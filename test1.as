LOOP: jmp L1
M33: .string "aaa"
L1: .data 6,6,-9
    .entry L1
    .entry MAIN
    .entry STR
    .entry M33
STR: .string "abcdef"
MAIN: mov @r3 , LENGTH
sub @r1, @r4
LENGTH: .data 6,-9,15
prn -5
bne LOOP
bne END
sub @r2, @r1
    .entry LOOP
K: .data 22
    .entry LENGTH
    .extern END
    .entry K