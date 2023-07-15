#pragma once

#include <unordered_map>
#include <variant>
#include <string>
#include <memory>
#include <cstddef>
#include <span>
#include <wasm.h>
#include "ecsact/wasm.h"
#include "ecsact/runtime/common.h"

namespace ecsact::wasm::detail {

struct loaded_module {
	std::vector<std::byte> wasm_binary_data;

	ecsact_system_like_id id;
	std::string           export_name;
};

struct load_modules_error {
	ecsactsi_wasm_error code;
};

using load_modules_result_t = std::variant<>;

auto load_modules(
	std::span<std::byte>             wasm_data,
	std::span<ecsact_system_like_id> system_ids,
	std::span<const char*>           export_names
) -> load_modules_result_t;

} // namespace ecsact::wasm::detail
