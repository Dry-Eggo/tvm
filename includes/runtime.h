#pragma once
#include <registers.h>
#include <types.h>
#include <inst.h>
#include <memory.h>

#define MAX_REGISTER 22
#define arr_len(arr) (sizeof(arr)/sizeof(arr[0]))
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

  Heap heap;
} Nova;

void nova_vm_init_runtime(Nova *vm);
void nova_vm_set_program(Nova* vm, Instruction* program, int instruction_count);
void nova_vm_exec(Nova* vm);
void nova_vm_throw_error(Nova* vm, Trap trap);
