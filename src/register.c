#include <registers.h>

Register nova_make_register(RegId id, Qword data) {
  return (Register){.id = id, .data = data};
}
