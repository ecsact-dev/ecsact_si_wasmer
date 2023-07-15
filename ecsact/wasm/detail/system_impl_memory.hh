#pragma once

#include <string_view>
#include <array>
#include <cstddef>
#include <wasm.h>

namespace ecsact::wasm::detail {

struct system_impl_invocation_state {
	struct handle {
		std::array<std::byte, 4096> host_mem_stack;

		~handle();

		auto func_name() const -> const std::string&;
	};

	static auto thread_claim() -> handle;

	wasm_module_t*   module;
	wasm_instance_t* instance;
	wasm_store_t*    store;
	wasm_func_t*     func;
	std::string      func_name;
	wasm_memory_t*   memory;
};

auto reserve_wasm_stores( //
	size_t count
) -> void;

} // namespace ecsact::wasm::detail
