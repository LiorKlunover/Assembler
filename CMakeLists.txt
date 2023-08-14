cmake_minimum_required(VERSION 3.25)
project(untitled5 C)
set(CMAKE_C_STANDARD 90)
add_executable(Assembler Assembler.c code.c code.h preAssembler.c preAssembler.h table.h table.c lexer.c lexer.h firstPass.c firstPass.h globals.h
        secondPass.c
        secondPass.h)
