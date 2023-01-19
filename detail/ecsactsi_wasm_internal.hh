#pragma once

#include <map>
#include <string>
#include <variant>
#include <vector>
#include "ecsact/runtime/common.h"
#include "wasm_ecsact_system_execution.h"
#include "ecsactsi_wasm.h"

namespace ecsactsi_wasm::detail {
using load_modules_result_t = std::variant<
	ecsactsi_wasm_error,
	std::map<ecsact_system_like_id, ecsact_internal_wasm_system_module_info>>;

auto engine() -> wasm_engine_t*;

auto load_modules(
	char*                  wasm_data,
	int                    wasm_data_size,
	int                    systems_count,
	ecsact_system_like_id* system_ids,
	const char**           wasm_exports
) -> load_modules_result_t;

auto init_module(ecsact_internal_wasm_system_module_info& info) -> wasm_trap_t*;

void set_wasmer_last_error_message();
auto set_last_error_message(const std::string& str) -> void;
auto get_last_error_message() -> const std::string&;

auto trap_message(wasm_trap_t*) -> std::string;
} // namespace ecsactsi_wasm::detail
