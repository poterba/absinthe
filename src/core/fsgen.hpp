#pragma once

#include <cstdio>

namespace absinthe {
namespace core {

int fsgen_check_consistency(const char* firmwareName, const char* deviceName);
int generate_rop(FILE* out, int is_bootstrap, const char* firmwareName, const char* deviceName, int pid_len, unsigned int slide);

} // namespace core
} // namespace absinthe
