#include <assert.h>
#include <runtime.h>
#include <types.h>
#include <stdio.h>
#include <stdlib.h>

void tvm_vm_sure_reg(Tvm* vm, RegId id);
void tvm_vm_init_runtime(Tvm *vm) {
    assert_types
    vm->pc = 0;
    for (int i = 0; i < MAX_REGISTER; i++) {
	vm->registers[i] = tvm_make_register(i, Q(0));
    }
}

void tvm_vm_dump_register(Tvm* vm) {
    for (int i = 0; i < MAX_REGISTER; i++) {
	Register r = vm->registers[i];
	fprintf(stdout, "Register %3d: %lld\n", r.id, r.data);
    }
}

void tvm_vm_set_program(Tvm* vm, Instruction* program, int instruction_count) {
    vm->program = program;
    vm->program_count = instruction_count;
}

void tvm_vm_add(Tvm* vm, Instruction inst) {
    int v1 = 0;
    int v2 = 0;
    int r1;

    if (inst.op1.kind == OPK_Register) {
	int reg = inst.op1.data.int_value;
	tvm_vm_sure_reg(vm, reg);
	v1 = vm->registers[reg].data;
	r1 = reg;
    } else if (inst.op1.kind == OPK_Interger) {
	v1 = inst.op1.data.int_value;
    }

    if (inst.op2.kind == OPK_Register) {
	int reg = inst.op2.data.int_value;
	tvm_vm_sure_reg(vm, reg);
	v2 = vm->registers[reg].data;
    } else if (inst.op2.kind == OPK_Interger) {
	v2 = inst.op2.data.int_value;
    }
    
    if (inst.opc == 3) {
	int id = inst.op3.data.int_value;
	tvm_vm_sure_reg(vm, id);
	vm->registers[id].data = (v1 + v2);
    } else {
	vm->registers[r1].data = (v1 + v2);
    }
}

void tvm_vm_sub(Tvm* vm, Instruction inst) {
    int v1 = 0;
    int v2 = 0;
    int r1;

    if (inst.op1.kind == OPK_Register) {
	int reg = inst.op1.data.int_value;
	tvm_vm_sure_reg(vm, reg);
	v1 = vm->registers[reg].data;
	r1 = reg;
    } else if (inst.op1.kind == OPK_Interger) {
	v1 = inst.op1.data.int_value;
    }

    if (inst.op2.kind == OPK_Register) {
	int reg = inst.op2.data.int_value;
	tvm_vm_sure_reg(vm, reg);
	v2 = vm->registers[reg].data;
    } else if (inst.op2.kind == OPK_Interger) {
	v2 = inst.op2.data.int_value;
    }
    
    if (inst.opc == 3) {
	int id = inst.op3.data.int_value;
	tvm_vm_sure_reg(vm, id);
	vm->registers[id].data = (v1 - v2);
    } else {
	vm->registers[r1].data = (v1 - v2);
    }
}

void tvm_vm_mul(Tvm* vm, Instruction inst) {
    int v1 = 0;
    int v2 = 0;
    int r1;

    if (inst.op1.kind == OPK_Register) {
	int reg = inst.op1.data.int_value;
	tvm_vm_sure_reg(vm, reg);
	v1 = vm->registers[reg].data;
	r1 = reg;
    } else if (inst.op1.kind == OPK_Interger) {
	v1 = inst.op1.data.int_value;
    }

    if (inst.op2.kind == OPK_Register) {
	int reg = inst.op2.data.int_value;
	tvm_vm_sure_reg(vm, reg);
	v2 = vm->registers[reg].data;
    } else if (inst.op2.kind == OPK_Interger) {
	v2 = inst.op2.data.int_value;
    }
    
    if (inst.opc == 3) {
	int id = inst.op3.data.int_value;
	tvm_vm_sure_reg(vm, id);
	vm->registers[id].data = (v1 * v2);
    } else {
	vm->registers[r1].data = (v1 * v2);
    }
}

void tvm_vm_cmp(Tvm* vm, Instruction inst) {
    int v1 = 0;
    int v2 = 0;
    Decorator d = DECOR_EQ;

    if (inst.decorator_count > 1) {
	tvm_vm_throw_error(vm, tvm_trap_inv_dc("cmp", "Expected 1 decorator"));
    }

    d = inst.decorators[0];
    
    if (inst.op1.kind == OPK_Register) {
	RegId r1 = inst.op1.data.int_value;
	tvm_vm_sure_reg(vm, r1);
	v1 = vm->registers[r1].data;
    } else if (inst.op1.kind == OPK_Interger) {
	v1 = inst.op1.data.int_value;
    }

    if (inst.op2.kind == OPK_Register) {
	RegId r1 = inst.op2.data.int_value;
	tvm_vm_sure_reg(vm, r1);
	v2 = vm->registers[r1].data;
    } else if (inst.op2.kind == OPK_Interger) {
	v2 = inst.op2.data.int_value;
    }

    if (inst.opc == 3) {
	RegId id = inst.op3.data.int_value;
	tvm_vm_sure_reg(vm, id);
	if (d == DECOR_EQ) vm->registers[id].data = (v1 == v2);
	else if (d == DECOR_GT) vm->registers[id].data = (v1 > v2);
	else if (d == DECOR_GTE) vm->registers[id].data = (v1 >= v2);
	else if (d == DECOR_LT) vm->registers[id].data = (v1 < v2);
	else if (d == DECOR_LTE) vm->registers[id].data = (v1 <= v2);
	else if (d == DECOR_NE) vm->registers[id].data = (v1 != v2);
	else {
	    tvm_vm_throw_error(vm, tvm_trap_inv_dec("cmp", d));
	}
    }
}

void tvm_vm_sure_reg(Tvm* vm,int id) {
    if (id >= MAX_REGISTER) {
	tvm_vm_throw_error(vm, tvm_trap_inv_reg(id));
    }
}

void tvm_vm_exec(Tvm* vm) {
    for(;;) {
	if (vm->pc >= vm->program_count) break;

	Instruction inst = vm->program[vm->pc];
	if (inst.opcode == OP_MOV) {
	    if (inst.op1.kind == OPK_Register) {
		RegId r1 = inst.op1.data.int_value;
		tvm_vm_sure_reg(vm, r1);
		if (inst.op2.kind == OPK_Interger) {
		    vm->registers[r1].data = inst.op2.data.int_value;
		} else if (inst.op2.kind == OPK_Register) {
		    RegId id = inst.op2.data.int_value;
		    tvm_vm_sure_reg(vm, id);
		    vm->registers[r1] = vm->registers[id]; 
		}
	    } else {
		tvm_vm_throw_error(vm, tvm_trap_inv_op("mov", "Expected Register or Memory Address"));
	    }
	} else if (inst.opcode == OP_ADD) {
	    tvm_vm_add(vm, inst);
	} else if (inst.opcode == OP_SUB) {
	    tvm_vm_sub(vm, inst);
	} else if (inst.opcode == OP_MUL) {
	    tvm_vm_mul(vm, inst);
	} else if (inst.opcode == OP_DUMP_REGISTER) {
	    tvm_vm_dump_register(vm);	    
	} else if (inst.opcode == OP_CMP) {
	    tvm_vm_cmp(vm, inst);
	} else if (inst.opcode == OP_JMP) {
	    int v1 = 0;
	    if (inst.op1.kind == OPK_Register) {
		RegId r1 = inst.op1.data.int_value;
		tvm_vm_sure_reg(vm, r1);
		v1 = vm->registers[r1].data;
	    } else if (inst.op1.kind == OPK_Interger) {
		v1 = inst.op1.data.int_value;
	    }

	    int pc = 0;
	    if (inst.op2.kind == OPK_Interger) {
		pc = inst.op2.data.int_value;
	    } else if (inst.op2.kind == OPK_Register) {
		RegId r2 = inst.op2.data.int_value;
		tvm_vm_sure_reg(vm, r2);
		pc = vm->registers[r2].data;
	    }
	    if (v1 == 1) {
		vm->pc = pc;
		continue;
	    }
	    
	} else if (inst.opcode == OP_PRINT_INT) {
	    RegId id = inst.op1.data.int_value;
	    tvm_vm_sure_reg(vm, id);
	    printf("%lld\n", vm->registers[id].data);
	} else if (inst.opcode == OP_HALT) {
	    exit(0);
	} else {
	    tvm_vm_throw_error(vm, tvm_trap_inv_ins(inst.opcode));
	}
	
	vm->pc++;
    }
    // Execution Should never reach here
    // Program should cleanly exit in the loop
    tvm_vm_throw_error(vm, tvm_trap_inv_pc(vm->pc));
}

void tvm_vm_throw_error(Tvm* vm, Trap trap) {
    if (trap.kind == TRAP_SEGFAULT) {
	fprintf(stderr, "Tvm: Seg-fault: %s\n", trap.seg_f.msg);
    } else if (trap.kind == TRAP_INVALID_PROGRAM_INDEX) {
	fprintf(stderr, "Tvm: Invalid Program Index: '%d'\n", trap.id_value);
    } else if (trap.kind == TRAP_INVALID_REGISTER) {
	fprintf(stderr, "Tvm: '%d' is not a valid Register id\n", trap.id_value);
    } else if (trap.kind == TRAP_INVALID_OPERAND) {
	fprintf(stderr, "Tvm: Invalid Operand: '%s': %s\n", trap.inv_op.opcode, trap.inv_op.msg);
    } else if (trap.kind == TRAP_INVALID_INSTRUCTION) {
	fprintf(stderr, "Tvm: Invalid Instruction: '%d'\n", trap.id_value);
    } else if (trap.kind == TRAP_INVALID_DECORATOR) {
	fprintf(stderr, "Tvm: Invalid Decorator: '%s': '%d'\n", trap.str_ipair.str,trap.str_ipair.i);
    } else if (trap.kind == TRAP_INVALID_DECORATOR_COUNT) {
	fprintf(stderr, "Tvm: Invalid Decorator: '%s': '%s'\n", trap.str_pair.str1,trap.str_pair.str2);
    } 
    
    exit(trap.kind);
}
