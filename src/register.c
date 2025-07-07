#include <registers.h>

Register tvm_make_register(RegId id, Qword data) {
  return (Register){.id = id, .data = data};
}
