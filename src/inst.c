#include <inst.h>
#include <string.h>

Instruction nova_make_inst(Opcode op, Decorator* decor, int dc) {
    Instruction i;
    i.opcode = op;
    i.opc = 1;
    i.decorator_count = dc;
    memcpy(i.decorators, decor, dc * sizeof(Decorator));
    return i;
}

Instruction nova_make_inst2(Opcode op, Operand op1, Operand op2, Decorator* decor, int dc) {
    Instruction i;
    i.opcode = op;
    i.op1 = op1;
    i.op2 = op2;
    i.opc = 2;
    i.decorator_count = dc;
    memcpy(i.decorators, decor, dc * sizeof(Decorator));
    return i;
}

Instruction nova_make_inst3(Opcode op, Operand op1, Operand op2, Operand op3, Decorator* decor, int dc) {
    Instruction i;
    i.opcode = op;
    i.op1 = op1;
    i.op2 = op2;
    i.op3 = op3;
    i.opc = 3;
    i.decorator_count = dc;
    memcpy(i.decorators, decor, dc * sizeof(Decorator));
    return i;
}

Operand nova_make_opr_string(const char *s) {
    Operand op;
    op.kind = OPK_String;
    op.data.string_value = strdup(s);
    return op;
}

Operand nova_make_opr_int(int i) {
    Operand op;
    op.kind = OPK_Integer;
    op.data.int_value = i;
    return op;
}

Operand nova_make_opr_reg(int i) {
    Operand op;
    op.kind = OPK_Register;
    op.data.int_value = i;
    return op;
}
