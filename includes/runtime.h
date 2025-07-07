#pragma once
#include <registers.h>
#include <types.h>
#include <inst.h>
#define MAX_REGISTER 22

#define assert_types     fprintf(stdout, "Tvm: Asserting Byte sizes...\n"); \
fprintf(stdout, "    |-  Byte  = %ld bytes\n", sizeof(Byte)); \
assert(sizeof(Byte) == 1);\
fprintf(stdout, "    |-  Word  = %ld bytes\n", sizeof(Word));\
assert(sizeof(Word) == 2);\
fprintf(stdout, "    |-  Dword = %ld bytes\n", sizeof(Dword));\
assert(sizeof(Dword) == 4);\
fprintf(stdout, "    |_  Qword = %ld bytes\n", sizeof(Qword));\
assert(sizeof(Qword) == 8);\
fprintf(stdout, "Tvm: Asserting Byte sizes: (Ok)\n");

typedef struct {
  ProgramCounter pc;
  Register registers[MAX_REGISTER];
  Instruction* program;
  ProgramCounter program_count;
} Tvm;

void tvm_vm_init_runtime(Tvm *vm);
void tvm_vm_set_program(Tvm* vm, Instruction* program, int instruction_count);
void tvm_vm_exec(Tvm* vm);
void tvm_vm_throw_error(Tvm* vm, Trap trap);
