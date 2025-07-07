#include <types.h>

Trap tvm_trap_segfault(const char* msg) {
    Trap t;
    t.kind = TRAP_SEGFAULT;
    t.seg_f = (seg_fault){msg};
    return t;
}

Trap tvm_trap_inv_op  (const char* op_name, const char* msg) {
    Trap t;
    t.kind = TRAP_INVALID_OPERAND;
    t.inv_op = (invalid_op) {op_name, msg};
    return t;
}

Trap tvm_trap_inv_reg (RegId id) {
    Trap t;
    t.kind = TRAP_INVALID_REGISTER;
    t.id_value = id;
    return t;
}

Trap tvm_trap_inv_pc  (ProgramCounter pc) {
    Trap t;
    t.kind = TRAP_INVALID_PROGRAM_INDEX;
    t.id_value = pc;
    return t;
}

Trap tvm_trap_inv_ins (int opcode) {
    Trap t;
    t.kind = TRAP_INVALID_INSTRUCTION;
    t.id_value = opcode;
    return t;
}

Trap tvm_trap_inv_dec (const char* n, int d) {
    Trap t;
    t.kind = TRAP_INVALID_DECORATOR;
    t.str_ipair = (str_int) {n, d};
    return t;
}

Trap tvm_trap_inv_dc  (const char* n, const char* m) {
    Trap t;
    t.kind = TRAP_INVALID_DECORATOR_COUNT;
    t.str_pair = (str_str) {n, m};
    return t;
}
