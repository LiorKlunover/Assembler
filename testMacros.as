MAIN: mov @r3,LENGTH
LENGTH: .data 6,-9,15
STR: .string    "aaaa"
LOOP: jmp L1
END: stop
    mcro m1
        add L2, @r5
    endmcro
.entry LOOP
m1
prn -5
bne LOOP
sub @r1, @r4
    bne END
L1: inc K
jmp L2
bne LOOP
sub @r2, @r1
K: .data 22
    .extern L2
    .entry END
    .entry STR
    .entry MAIN
    .entry K
    .entry LENGTH
    .entry L1

