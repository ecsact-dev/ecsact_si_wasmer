#include "wasm_ecsact_system_execution.h"

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "ecsact/runtime/dynamic.h"

#include "wasm_ecsact_pointer_map.hh"

namespace {

std::unordered_map<ecsact_system_execution_context*, wasm_memory_t*> mem_map;

ecsact_system_execution_context* get_execution_context(const wasm_val_t& val) {
	assert(val.kind == WASM_I32);
	return ecsactsi_wasm::as_host_pointer<ecsact_system_execution_context>(
		val.of.i32
	);
}

template<typename EcsactID>
EcsactID ecsact_id_from_wasm_i32(const wasm_val_t& val) {
	assert(val.kind == WASM_I32);
	return static_cast<EcsactID>(val.of.i32);
}

void* get_void_ptr(const wasm_val_t& val, wasm_memory_t* memory) {
	assert(val.kind == WASM_I32);
	if(val.of.i32 == 0) {
		return nullptr;
	}

	auto mem_bytes = wasm_memory_data(memory);
	// the i32 val is an index of the wasm memory data
	return mem_bytes + val.of.i32;
}

const void* get_const_void_ptr(const wasm_val_t& val, wasm_memory_t* memory) {
	assert(val.kind == WASM_I32);
	if(val.of.i32 == 0) {
		return nullptr;
	}

	auto mem_bytes = wasm_memory_data(memory);
	// the i32 val is an index of the wasm memory data
	return mem_bytes + val.of.i32;
}

} // namespace

void set_wasm_ecsact_system_execution_context_memory(
	ecsact_system_execution_context* ctx,
	wasm_memory_t*                   memory
) {
	if(memory == nullptr) {
		mem_map.erase(ctx);
	} else {
		mem_map[ctx] = memory;
	}
}

wasm_trap_t* wasm_ecsact_system_execution_context_action(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto ctx = get_execution_context(args->data[0]);
	auto memory = mem_map.at(ctx);

	ecsact_system_execution_context_action(
		ctx,
		get_void_ptr(args->data[1], memory)
	);

	return nullptr;
}

wasm_trap_t* wasm_ecsact_system_execution_context_add(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto ctx = get_execution_context(args->data[0]);
	auto memory = mem_map.at(ctx);

	ecsact_system_execution_context_add(
		ctx,
		ecsact_id_from_wasm_i32<ecsact_component_like_id>(args->data[1]),
		get_const_void_ptr(args->data[2], memory)
	);

	return nullptr;
}

wasm_trap_t* wasm_ecsact_system_execution_context_remove(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	ecsact_system_execution_context_remove(
		get_execution_context(args->data[0]),
		ecsact_id_from_wasm_i32<ecsact_component_like_id>(args->data[1])
	);
	return nullptr;
}

wasm_trap_t* wasm_ecsact_system_execution_context_get(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto ctx = get_execution_context(args->data[0]);
	auto memory = mem_map.at(ctx);

	ecsact_system_execution_context_get(
		ctx,
		ecsact_id_from_wasm_i32<ecsact_component_like_id>(args->data[1]),
		get_void_ptr(args->data[2], memory)
	);

	return nullptr;
}

wasm_trap_t* wasm_ecsact_system_execution_context_update(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto ctx = get_execution_context(args->data[0]);
	auto memory = mem_map.at(ctx);

	ecsact_system_execution_context_update(
		ctx,
		ecsact_id_from_wasm_i32<ecsact_component_like_id>(args->data[1]),
		get_const_void_ptr(args->data[2], memory)
	);

	return nullptr;
}

wasm_trap_t* wasm_ecsact_system_execution_context_has(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	bool has_component = ecsact_system_execution_context_has(
		get_execution_context(args->data[0]),
		ecsact_id_from_wasm_i32<ecsact_component_like_id>(args->data[1])
	);

	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = has_component ? 1 : 0;

	return nullptr;
}

wasm_trap_t* wasm_ecsact_system_execution_context_generate(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto ctx = get_execution_context(args->data[0]);
	auto memory = mem_map.at(ctx);
	auto components_count = args->data[1].of.i32;

	std::vector<const void*> component_datas;
	component_datas.resize(components_count);

	// each i32 element represents a pointer in WASM memory
	auto component_datas_wasm =
		static_cast<int32_t*>(get_void_ptr(args->data[3], memory));
	for(int i = 0; components_count > i; ++i) {
		component_datas[i] = get_const_void_ptr(
			wasm_val_t{
				.kind = WASM_I32,
				.of{.i32 = component_datas_wasm[i]},
			},
			memory
		);
	}

	ecsact_system_execution_context_generate(
		ctx,
		components_count,
		static_cast<ecsact_component_id*>(get_void_ptr(args->data[2], memory)),
		component_datas.data()
	);
	return nullptr;
}

wasm_trap_t* wasm_ecsact_system_execution_context_parent(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto parent =
		ecsact_system_execution_context_parent(get_execution_context(args->data[0])
		);

	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = ecsactsi_wasm::as_guest_pointer(
		const_cast<void*>(static_cast<const void*>(parent))
	);

	return nullptr;
}

wasm_trap_t* wasm_ecsact_system_execution_context_same(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	bool same = ecsact_system_execution_context_same(
		get_execution_context(args->data[0]),
		get_execution_context(args->data[1])
	);

	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = same ? 1 : 0;

	return nullptr;
}

wasm_trap_t* wasm_ecsact_system_execution_context_other(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto ctx = get_execution_context(args->data[0]);
	auto system_id = ecsact_system_execution_context_id(ctx);
	auto info = get_ecsact_internal_module_info(system_id);

	auto other = ecsact_system_execution_context_other(
		ctx,
		ecsact_id_from_wasm_i32<ecsact_entity_id>(args->data[1])
	);

	set_wasm_ecsact_system_execution_context_memory(
		other,
		info->system_impl_memory
	);

	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = ecsactsi_wasm::as_guest_pointer(other);

	return nullptr;
}

wasm_trap_t* wasm_ecsact_system_execution_context_entity(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto entity =
		ecsact_system_execution_context_entity(get_execution_context(args->data[0])
		);
	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = static_cast<int32_t>(entity);

	return nullptr;
}
