#pragma once
#include <types.h>

typedef struct {
  RegId id;
  Qword data;
} Register;

typedef struct {
    int ok;
    union {
	Trap trap;
	Register registers;
    };
} RegisterResult;

Register nova_make_register(RegId id, Qword data);
