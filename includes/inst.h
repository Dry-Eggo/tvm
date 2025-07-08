#pragma once
#include <registers.h>

#define MAX_DECORATORS 3

typedef enum {
  OP_MOV,
  OP_FUNCTION,
  OP_ARGS_DEF,
  OP_RETURN_DEF,
  OP_HALT,
  OP_JMP,
  OP_ALLOC,
  OP_FREE,
  OP_WRITE,
  OP_LOAD,
  
  // Arithmetics
  OP_ADD,
  OP_SUB,
  OP_DIV,
  OP_MUL,
  OP_CMP,
  
  // Debug
  OP_DUMP_REGISTER,
  OP_DUMP_HEAP,
  OP_PRINT_INT,
  OP_INVALID
} Opcode;

typedef enum {
  OPK_String,
  OPK_Integer,
  OPK_Register,
} OperandKind;

typedef struct {
  OperandKind kind;
  union {
    const char *string_value;
    long int_value;
  } data;
} Operand;

typedef enum {
    DECOR_EXPORT,
    DECOR_EQ,
    DECOR_NE,
    DECOR_GT,
    DECOR_LT,
    DECOR_GTE,
    DECOR_LTE,
} Decorator;

typedef struct {
    int opc;
    int decorator_count;
    Decorator decorators[MAX_DECORATORS];
    
    Opcode opcode;
    Operand op1;
    Operand op2;
    Operand op3;
} Instruction;

Instruction nova_make_inst(Opcode op, Decorator* decor, int dc);
Instruction nova_make_inst1(Opcode op, Operand op1, Decorator* decor, int dc);
Instruction nova_make_inst2(Opcode op, Operand op1, Operand op2, Decorator* decor, int dc);
Instruction nova_make_inst3(Opcode op, Operand op1, Operand op2, Operand op3, Decorator* decor, int dc);
Operand nova_make_opr_string(const char *s);
Operand nova_make_opr_int(int i);
Operand nova_make_opr_reg(int i);
