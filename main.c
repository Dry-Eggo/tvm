#include <inst.h>
#include <runtime.h>
#include <stddef.h>

#define arr_len(arr) (sizeof(arr)/sizeof(arr[0]))

Decorator cmp_de[1] = {
    DECOR_EQ,
};

int main(void) {
  Tvm vm;
  tvm_vm_init_runtime(&vm);
  Instruction program[] = {
      tvm_make_inst2(OP_MOV, tvm_make_opr_reg(1), tvm_make_opr_int(10), NULL, 0),
      tvm_make_inst2(OP_MOV, tvm_make_opr_reg(2), tvm_make_opr_int(20), NULL, 0),
      tvm_make_inst3(OP_ADD, tvm_make_opr_reg(1), tvm_make_opr_reg(2),  tvm_make_opr_reg(3), NULL, 0),
      tvm_make_inst3(OP_CMP, tvm_make_opr_reg(1), tvm_make_opr_int(10), tvm_make_opr_reg(4), cmp_de, arr_len(cmp_de)),
      tvm_make_inst2(OP_MOV, tvm_make_opr_reg(5), tvm_make_opr_reg(3), NULL, 0),
      tvm_make_inst(OP_DUMP_REGISTER, NULL, 0),
      tvm_make_inst(OP_HALT, NULL, 0),
  };
  tvm_vm_set_program(&vm, program, arr_len(program));
  tvm_vm_exec(&vm);
}
