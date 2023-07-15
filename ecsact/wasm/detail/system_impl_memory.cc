#include "system_impl_memory.hh"

#include <unordered_map>
#include <cstddef>
#include <array>
#include <memory>
#include <wasm.h>
#include "ecsact/runtime/common.h"

namespace {

struct ecsact_si_wasm_module_info {
	std::shared_ptr<std::vector<std::byte>> binary_data;
};

thread_local struct {
	std::array<std::byte, 4096> host_memory = {};

	wasm_memory_t* memory = {};
	wasm_store_t*  store = {};

	ecsact_system_like_id id = {};
} ecsact_si_wasm_mem;

} // namespace
