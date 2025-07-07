#include <registers.h>

Register tvm_make_register(Reg_Id id, Qword data) {
  return (Register){.id = id, .data = data};
}
