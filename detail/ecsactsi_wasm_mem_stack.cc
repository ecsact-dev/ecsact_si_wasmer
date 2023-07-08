#include "ecsactsi_wasm_mem_stack.hh"

#include <cassert>
#include <cstdint>
#include <atomic>

static thread_local struct {
	std::byte*  data = nullptr;
	std::size_t data_max_size = 0;
	std::size_t data_offset = 0;
} call_mem_info;

auto ecsactsi_wasm::detail::set_call_mem_data( //
	void*  data,
	size_t max_size
) -> void {
#ifndef NDEBUG
	if(data) {
		assert(call_mem_info.data == nullptr);
	} else {
		assert(call_mem_info.data != nullptr);
	}
#endif

	call_mem_info.data = static_cast<std::byte*>(data);
	call_mem_info.data_max_size = max_size;
	call_mem_info.data_offset = 0;
}

auto ecsactsi_wasm::detail::call_mem_alloc_raw( //
	size_t data_size
) -> std::int32_t {
	auto index = static_cast<std::int32_t>(call_mem_info.data_offset);
	call_mem_info.data_offset += data_size;
	assert(index >= 0);
	return index;
}

auto ecsactsi_wasm::detail::call_mem_read_raw( //
	std::int32_t offset
) -> void* {
	assert(call_mem_info.data != nullptr);
	assert(offset < call_mem_info.data_offset);
	return call_mem_info.data + offset;
}
