#include "wasm_ecsact_memory.hh"

static thread_local wasm_memory_t* _current_wasm_mem = nullptr;

wasm_memory_t* ecsactsi_wasm::current_wasm_memory_rw() {
	return _current_wasm_mem;
}

const wasm_memory_t* ecsactsi_wasm::current_wasm_memory_ro() {
	return _current_wasm_mem;
}

void ecsactsi_wasm::detail::set_current_wasm_memory(wasm_memory_t* mem) {
	_current_wasm_mem = mem;
}
