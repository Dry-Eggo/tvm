#include <runtime.h>
#include <stddef.h>

int main() {
    Nova vm;
    nova_vm_init_runtime(&vm);
    Instruction program[] = {
	nova_make_inst2(OP_ALLOC, nova_make_opr_int(30), nova_make_opr_reg(0), NULL, 0),
	nova_make_inst2(OP_ALLOC, nova_make_opr_int(30), nova_make_opr_reg(1), NULL, 0),
	// nova_make_inst1(OP_FREE,  nova_make_opr_reg(0),  NULL, 0),
	nova_make_inst(OP_DUMP_REGISTER, NULL, 0),
	nova_make_inst(OP_DUMP_HEAP, NULL, 0),
	nova_make_inst(OP_HALT, NULL, 0),
    };
    nova_vm_set_program(&vm, program, arr_len(program));
    nova_vm_exec(&vm);
}
