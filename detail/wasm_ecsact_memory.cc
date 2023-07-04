#include "wasm_ecsact_memory.hh"
#include "ecsactsi_wasm_mem_stack.hh"

#include <cassert>

namespace {
auto get_curr_mem() -> wasm_memory_t* {
	// offset 0 is always the current wasm memory
	return ecsactsi_wasm::detail::call_mem_read<wasm_memory_t*>(0);
}
} // namespace

wasm_memory_t* ecsactsi_wasm::current_wasm_memory_rw() {
	auto mem = get_curr_mem();
	assert(mem != nullptr);
	return mem;
}

const wasm_memory_t* ecsactsi_wasm::current_wasm_memory_ro() {
	auto mem = get_curr_mem();
	assert(mem != nullptr);
	return mem;
}
