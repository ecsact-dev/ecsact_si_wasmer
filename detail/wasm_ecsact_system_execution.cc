#include "wasm_ecsact_system_execution.h"

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "ecsact/runtime/dynamic.h"

#include "detail/ecsactsi_wasm_mem_stack.hh"

namespace {

auto get_execution_context( //
	const wasm_val_t& val
) -> ecsact_system_execution_context* {
	assert(val.kind == WASM_I32);
	return ecsactsi_wasm::detail::call_mem_read<ecsact_system_execution_context*>(
		val.of.i32
	);
}

auto get_execution_context_memory( //
	const wasm_val_t& val
) -> wasm_memory_t* {
	assert(val.kind == WASM_I32);
	// wasm memory is always allocated right before the execution context
	return ecsactsi_wasm::detail::call_mem_read<wasm_memory_t*>(
		val.of.i32 -
		sizeof(ecsact_system_execution_context*) // NOLINT(bugprone-sizeof-expression)
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

wasm_trap_t* wasm_ecsact_system_execution_context_action(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto ctx = get_execution_context(args->data[0]);
	auto memory = get_execution_context_memory(args->data[0]);

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
	auto memory = get_execution_context_memory(args->data[0]);

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
	auto memory = get_execution_context_memory(args->data[0]);

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
	auto memory = get_execution_context_memory(args->data[0]);

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
	auto memory = get_execution_context_memory(args->data[0]);
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
	auto ctx = get_execution_context(args->data[0]);
	auto system_id = ecsact_system_execution_context_id(ctx);
	auto info = get_ecsact_internal_module_info(system_id);

	ecsactsi_wasm::detail::call_mem_alloc(info->system_impl_memory);
	auto parent = ecsact_system_execution_context_parent(ctx);

	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = ecsactsi_wasm::detail::call_mem_alloc(parent);

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

	ecsactsi_wasm::detail::call_mem_alloc(info->system_impl_memory);
	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = ecsactsi_wasm::detail::call_mem_alloc(other);

	return nullptr;
}

wasm_trap_t* wasm_ecsact_system_execution_context_entity(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto entity = ecsact_system_execution_context_entity( //
		get_execution_context(args->data[0])
	);
	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = static_cast<int32_t>(entity);

	return nullptr;
}
