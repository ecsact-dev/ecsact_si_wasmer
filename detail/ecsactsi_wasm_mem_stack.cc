#include "ecsactsi_wasm_mem_stack.hh"

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
	call_mem_info.data = static_cast<std::byte*>(data);
	call_mem_info.data_max_size = max_size;
	call_mem_info.data_offset = 0;
}

auto ecsactsi_wasm::detail::call_mem_alloc_raw( //
	size_t data_size
) -> std::int32_t {
	auto index = static_cast<std::int32_t>(call_mem_info.data_offset);
	call_mem_info.data_offset += data_size;
	return index;
}

auto ecsactsi_wasm::detail::call_mem_read_raw( //
	std::int32_t offset
) -> void* {
	return call_mem_info.data + offset;
}
