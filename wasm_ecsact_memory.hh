#pragma once

#include <wasm.h>
#include <stdint.h>

namespace ecsactsi_wasm {

/**
 * Get a pointer to a specified type from wasm memory. Be very careful when
 * calling this function. Using the incorrect type or wasm memory is UB.
 *
 * @param wasm_mem the wasm memory associated with the guest pointer
 * @param wasm_guest_ptr wasm guest represents pointers as an i32 which is
 *        simply an offset of @p wasm_mem.
 *
 * @returns pointer to T or undefined if invalid guest pointer or wasm
 *          memory.
 */
template<typename T>
T* wasm_memory_cast(wasm_memory_t* wasm_mem, int32_t wasm_guest_ptr) {
	auto mem_bytes = wasm_memory_data(wasm_mem);
	return reinterpret_cast<T*>(mem_bytes + wasm_guest_ptr);
}

/**
 * Get the current wasm memory.
 * @returns current wasm memory of nullptr if there is no current wasm memory
 */
wasm_memory_t* current_wasm_memory_rw();

/**
 * @copydoc current_wasm_memory_rw
 */
const wasm_memory_t* current_wasm_memory_ro();

namespace detail {
void set_current_wasm_memory(wasm_memory_t*);
}

} // namespace ecsactsi_wasm
