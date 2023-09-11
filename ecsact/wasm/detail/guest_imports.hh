#pragma once

#include <string>
#include <functional>
#include <map>
#include <wasm.h>

namespace ecsactsi_wasm::detail {
using allowed_guest_imports_t = std::unordered_map<
	std::string, // Function name
	std::function<wasm_func_t*(wasm_store_t*)>>;

using allowed_guest_modules_t = std::unordered_map<
	std::string, // Module name
	allowed_guest_imports_t>;
} // namespace ecsactsi_wasm::detail
