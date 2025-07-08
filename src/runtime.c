#include <assert.h>
#include <runtime.h>
#include <types.h>
#include <stdio.h>
#include <stdlib.h>

void nova_vm_sure_reg(Nova* vm, RegId id);
void nova_vm_init_runtime(Nova *vm) {
    assert_types
    vm->pc = 0;
    for (int i = 0; i < MAX_REGISTER; i++) {
	vm->registers[i] = nova_make_register(i, Q(0));
    }
    nova_mem_zero(&vm->heap);
}

void nova_vm_dump_register(Nova* vm) {
    for (int i = 0; i < MAX_REGISTER; i++) {
	Register r = vm->registers[i];
	fprintf(stdout, "Register %3d: %lld\n", r.id, r.data);
    }
}

void nova_vm_set_program(Nova* vm, Instruction* program, int instruction_count) {
    vm->program = program;
    vm->program_count = instruction_count;
}

void nova_vm_add(Nova* vm, Instruction inst) {
    int v1 = 0;
    int v2 = 0;
    int r1;

    if (inst.op1.kind == OPK_Register) {
	int reg = inst.op1.data.int_value;
	nova_vm_sure_reg(vm, reg);
	v1 = vm->registers[reg].data;
	r1 = reg;
    } else if (inst.op1.kind == OPK_Integer) {
	v1 = inst.op1.data.int_value;
    }

    if (inst.op2.kind == OPK_Register) {
	int reg = inst.op2.data.int_value;
	nova_vm_sure_reg(vm, reg);
	v2 = vm->registers[reg].data;
    } else if (inst.op2.kind == OPK_Integer) {
	v2 = inst.op2.data.int_value;
    }
    
    if (inst.opc == 3) {
	int id = inst.op3.data.int_value;
	nova_vm_sure_reg(vm, id);
	vm->registers[id].data = (v1 + v2);
    } else {
	vm->registers[r1].data = (v1 + v2);
    }
}

void nova_vm_sub(Nova* vm, Instruction inst) {
    int v1 = 0;
    int v2 = 0;
    int r1;

    if (inst.op1.kind == OPK_Register) {
	int reg = inst.op1.data.int_value;
	nova_vm_sure_reg(vm, reg);
	v1 = vm->registers[reg].data;
	r1 = reg;
    } else if (inst.op1.kind == OPK_Integer) {
	v1 = inst.op1.data.int_value;
    }

    if (inst.op2.kind == OPK_Register) {
	int reg = inst.op2.data.int_value;
	nova_vm_sure_reg(vm, reg);
	v2 = vm->registers[reg].data;
    } else if (inst.op2.kind == OPK_Integer) {
	v2 = inst.op2.data.int_value;
    }
    
    if (inst.opc == 3) {
	int id = inst.op3.data.int_value;
	nova_vm_sure_reg(vm, id);
	vm->registers[id].data = (v1 - v2);
    } else {
	vm->registers[r1].data = (v1 - v2);
    }
}

void nova_vm_mul(Nova* vm, Instruction inst) {
    int v1 = 0;
    int v2 = 0;
    int r1;

    if (inst.op1.kind == OPK_Register) {
	int reg = inst.op1.data.int_value;
	nova_vm_sure_reg(vm, reg);
	v1 = vm->registers[reg].data;
	r1 = reg;
    } else if (inst.op1.kind == OPK_Integer) {
	v1 = inst.op1.data.int_value;
    }

    if (inst.op2.kind == OPK_Register) {
	int reg = inst.op2.data.int_value;
	nova_vm_sure_reg(vm, reg);
	v2 = vm->registers[reg].data;
    } else if (inst.op2.kind == OPK_Integer) {
	v2 = inst.op2.data.int_value;
    }
    
    if (inst.opc == 3) {
	int id = inst.op3.data.int_value;
	nova_vm_sure_reg(vm, id);
	vm->registers[id].data = (v1 * v2);
    } else {
	vm->registers[r1].data = (v1 * v2);
    }
}

void nova_vm_cmp(Nova* vm, Instruction inst) {
    int v1 = 0;
    int v2 = 0;
    Decorator d = DECOR_EQ;

    if (inst.decorator_count > 1) {
	nova_vm_throw_error(vm, nova_trap_inv_dc("cmp", "Expected 1 decorator"));
    }

    d = inst.decorators[0];
    
    if (inst.op1.kind == OPK_Register) {
	RegId r1 = inst.op1.data.int_value;
	nova_vm_sure_reg(vm, r1);
	v1 = vm->registers[r1].data;
    } else if (inst.op1.kind == OPK_Integer) {
	v1 = inst.op1.data.int_value;
    }

    if (inst.op2.kind == OPK_Register) {
	RegId r1 = inst.op2.data.int_value;
	nova_vm_sure_reg(vm, r1);
	v2 = vm->registers[r1].data;
    } else if (inst.op2.kind == OPK_Integer) {
	v2 = inst.op2.data.int_value;
    }

    if (inst.opc == 3) {
	RegId id = inst.op3.data.int_value;
	nova_vm_sure_reg(vm, id);
	if (d == DECOR_EQ) vm->registers[id].data = (v1 == v2);
	else if (d == DECOR_GT) vm->registers[id].data = (v1 > v2);
	else if (d == DECOR_GTE) vm->registers[id].data = (v1 >= v2);
	else if (d == DECOR_LT) vm->registers[id].data = (v1 < v2);
	else if (d == DECOR_LTE) vm->registers[id].data = (v1 <= v2);
	else if (d == DECOR_NE) vm->registers[id].data = (v1 != v2);
	else {
	    nova_vm_throw_error(vm, nova_trap_inv_dec("cmp", d));
	}
    }
}

void nova_vm_sure_reg(Nova* vm,int id) {
    if (id >= MAX_REGISTER) {
	nova_vm_throw_error(vm, nova_trap_inv_reg(id));
    }
}

void nova_vm_exec(Nova* vm) {
    for(;;) {
	if (vm->pc >= vm->program_count) break;

	Instruction inst = vm->program[vm->pc];
	if (inst.opcode == OP_MOV) {
	    if (inst.op1.kind == OPK_Register) {
		RegId r1 = inst.op1.data.int_value;
		nova_vm_sure_reg(vm, r1);
		if (inst.op2.kind == OPK_Integer) {
		    vm->registers[r1].data = inst.op2.data.int_value;
		} else if (inst.op2.kind == OPK_Register) {
		    RegId id = inst.op2.data.int_value;
		    nova_vm_sure_reg(vm, id);
		    vm->registers[r1].data = vm->registers[id].data; 
		}
	    } else {
		nova_vm_throw_error(vm, nova_trap_inv_op("mov", "Expected Register or Memory Address"));
	    }
	} else if (inst.opcode == OP_ADD) {
	    nova_vm_add(vm, inst);
	} else if (inst.opcode == OP_SUB) {
	    nova_vm_sub(vm, inst);
	} else if (inst.opcode == OP_MUL) {
	    nova_vm_mul(vm, inst);
	} else if (inst.opcode == OP_DUMP_REGISTER) {
	    nova_vm_dump_register(vm);	    
	} else if (inst.opcode == OP_CMP) {
	    nova_vm_cmp(vm, inst);
	} else if (inst.opcode == OP_DUMP_HEAP) {
	    nova_mem_dump(&vm->heap);
	} else if (inst.opcode == OP_JMP) {
	    int v1 = 0;
	    if (inst.op1.kind == OPK_Register) {
		RegId r1 = inst.op1.data.int_value;
		nova_vm_sure_reg(vm, r1);
		v1 = vm->registers[r1].data;
	    } else if (inst.op1.kind == OPK_Integer) {
		v1 = inst.op1.data.int_value;
	    }

	    int pc = 0;
	    if (inst.op2.kind == OPK_Integer) {
		pc = inst.op2.data.int_value;
	    } else if (inst.op2.kind == OPK_Register) {
		RegId r2 = inst.op2.data.int_value;
		nova_vm_sure_reg(vm, r2);
		pc = vm->registers[r2].data;
	    }
	    if (v1 == 1) {
		vm->pc = pc;
		continue;
	    }
	    
	} else if (inst.opcode == OP_ALLOC) {
	    // alloc %30, $r3 # $r3 = malloc(30)
	    Qword size = 0;
	    if (inst.op1.kind == OPK_Register) {
		RegId id = inst.op1.data.int_value;
		nova_vm_sure_reg(vm, id);
		size = vm->registers[id].data;
	    } else if (inst.op1.kind == OPK_Integer) {
		size = inst.op1.data.int_value;
	    }
	    // printf("Nova: Attempting to allocate: %lld bytes\n", size);

	    if (inst.op2.kind == OPK_Register) {
		RegId id = inst.op2.data.int_value;
		nova_vm_sure_reg(vm, id);
		vm->registers[id].data = nova_mem_alloc(&vm->heap, size);
	    }
	    
	} else if (inst.opcode == OP_PRINT_INT) {
	    RegId id = inst.op1.data.int_value;
	    nova_vm_sure_reg(vm, id);
	    printf("%lld\n", vm->registers[id].data);
	} else if (inst.opcode == OP_FREE) {
	    Qword ptr = 0;
	    if (inst.op1.kind == OPK_Register) {
		RegId id = inst.op1.data.int_value;
		printf("Nova: Accessing Register { %d }\n", id);
		nova_vm_sure_reg(vm, id);
		ptr = vm->registers[id].data;
	    } else if (inst.op1.kind == OPK_Integer) {
		ptr = inst.op1.data.int_value;
	    }
	    nova_mem_free(&vm->heap, ptr);
	} else if (inst.opcode == OP_HALT) {
	    exit(0);
	} else {
	    nova_vm_throw_error(vm, nova_trap_inv_ins(inst.opcode));
	}
	
	vm->pc++;
    }
    // Execution Should never reach here
    // Program should cleanly exit in the loop
    nova_vm_throw_error(vm, nova_trap_inv_pc(vm->pc));
}

void nova_vm_throw_error(Nova* vm, Trap trap) {
    if (trap.kind == TRAP_SEGFAULT) {
	fprintf(stderr, "Nova: Seg-fault: %s\n", trap.seg_f.msg);
    } else if (trap.kind == TRAP_INVALID_PROGRAM_INDEX) {
	fprintf(stderr, "Nova: Invalid Program Index: '%d'\n", trap.id_value);
    } else if (trap.kind == TRAP_INVALID_REGISTER) {
	fprintf(stderr, "Nova: '%d' is not a valid Register id\n", trap.id_value);
    } else if (trap.kind == TRAP_INVALID_OPERAND) {
	fprintf(stderr, "Nova: Invalid Operand: '%s': %s\n", trap.inv_op.opcode, trap.inv_op.msg);
    } else if (trap.kind == TRAP_INVALID_INSTRUCTION) {
	fprintf(stderr, "Nova: Invalid Instruction: '%d'\n", trap.id_value);
    } else if (trap.kind == TRAP_INVALID_DECORATOR) {
	fprintf(stderr, "Nova: Invalid Decorator: '%s': '%d'\n", trap.str_ipair.str,trap.str_ipair.i);
    } else if (trap.kind == TRAP_INVALID_DECORATOR_COUNT) {
	fprintf(stderr, "Nova: Invalid Decorator: '%s': '%s'\n", trap.str_pair.str1,trap.str_pair.str2);
    } 
    
    exit(trap.kind);
}
