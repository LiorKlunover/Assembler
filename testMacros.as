
MAIN: ,, mov,@r3,LENGTH

mcro, m1
    sub @r1, @r4
    bne END
endmcro
prn -5
m1
L1: inc K
bne, , LOOP
mcro m3
    sub @r2, @r1
    LOOP END
    lior hagever
endmcro
mcro m2
    lior hagever
endmcro

LENGTH: .data 6,-9,15
STR:
m2
K: .data 22