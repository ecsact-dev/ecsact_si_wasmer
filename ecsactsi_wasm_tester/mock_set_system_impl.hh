#pragma once

#include "ecsact/runtime/core.h"

namespace ecsactsi_wasm_tester {

auto get_impl() -> ecsact_system_execution_impl;
auto set_impl(ecsact_system_execution_impl impl) -> void;

} // namespace ecsactsi_wasm_tester
