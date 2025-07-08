#include <inst.h>
#include <runtime.h>
#include <stddef.h>

Decorator cmp_de[1] = {
    DECOR_NE,
};

int main(void) {
  Nova vm;
  nova_vm_init_runtime(&vm);
  Instruction program[] = {
      nova_make_inst2(OP_MOV, nova_make_opr_reg(0), nova_make_opr_int(0), NULL, 0),
      nova_make_inst2(OP_MOV, nova_make_opr_reg(1), nova_make_opr_int(10), NULL, 0),
      nova_make_inst3(OP_CMP, nova_make_opr_reg(0), nova_make_opr_reg(1), nova_make_opr_reg(2), cmp_de, 1),
      nova_make_inst2(OP_PRINT_INT, nova_make_opr_reg(0), nova_make_opr_int(0), NULL, 0),
      nova_make_inst2(OP_ADD, nova_make_opr_reg(0), nova_make_opr_int(1), NULL, 0),
      nova_make_inst2(OP_JMP, nova_make_opr_reg(2), nova_make_opr_int(2), NULL, 0),
      nova_make_inst(OP_HALT, NULL, 0),
  };
  nova_vm_set_program(&vm, program, arr_len(program));
  nova_vm_exec(&vm);
}
