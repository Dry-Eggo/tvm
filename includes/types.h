#pragma once

#define B(i) ((Byte)(i))
#define W(i) ((Word)(i))
#define D(i) ((Dword)(i))
#define Q(i) ((Qword)(i))

typedef int ProgramCounter;
typedef int RegId;
typedef signed char Byte;
typedef short Word;
typedef signed int Dword;
typedef signed long long Qword;


typedef enum {
    TRAP_SEGFAULT,
    TRAP_INVALID_OPERAND,
    TRAP_INVALID_PROGRAM_INDEX,
    TRAP_INVALID_REGISTER,
    TRAP_INVALID_INSTRUCTION,
    TRAP_INVALID_DECORATOR,
    TRAP_INVALID_DECORATOR_COUNT,
} TrapKind;

typedef struct seg_fault { const char* msg; } seg_fault;
typedef struct invalid_op{ const char* opcode; const char* msg; } invalid_op;

typedef struct str_str   { const char* str1; const char* str2; } str_str;
typedef struct str_int   { const char* str; int i; } str_int;

typedef struct {
    TrapKind kind;
    union {
	struct seg_fault seg_f;
	struct invalid_op inv_op;
	struct str_str    str_pair;
	struct str_int    str_ipair;
	int    id_value;
    };
} Trap;

Trap nova_trap_segfault(const char* msg);
Trap nova_trap_inv_op  (const char*, const char*);
Trap nova_trap_inv_reg (RegId id);
Trap nova_trap_inv_pc  (ProgramCounter pc);
Trap nova_trap_inv_ins (int opcode);
Trap nova_trap_inv_dec (const char*, int d);
Trap nova_trap_inv_dc  (const char*, const char*);
