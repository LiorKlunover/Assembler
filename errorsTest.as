MAIN:  mov,@r3,LENGTH
MAIn :  mov,@r3,LENGTH
r2:  mov,@r3,LENGTH
LOOP: jmp L1
mcro m1
sub @r1, @r4
    bne END
endmcro
prn -5
bne LOOP
m1
L1: inc K
bne, LOOP
mcro m2
sub @r2, @r1
    LOOP END
lior hagever
endmcro
END : stop
STR: .string “abcdef”
LENGTH: .data 6,-9,15
m2
K: .data 22
