MAIN: mov @r3,LENGTH
LENGTH: .data 6,-9,15
STR: .string    "aaaa"
LOOP: jmp L1
END: stop
    .entry LOOP
prn -5
bne LOOP
sub @r1, @r4
    bne END
L1: inc K
bne LOOP
sub @r2, @r1
K: .data 22
    .extern END
    .entry STR
    .entry MAIN
    .entry K
    .entry LENGTH
    .entry L1

