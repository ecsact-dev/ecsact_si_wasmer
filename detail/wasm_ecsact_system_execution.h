/**
 * @file
 * @internal
 */

#ifndef WASM_ECSACT_SYSTEM_EXECUTION__H
#define WASM_ECSACT_SYSTEM_EXECUTION__H

#include <string>
#include <wasm.h>
#include "ecsact/runtime/common.h"

struct ecsact_internal_execution_context_linked_list {
	ecsact_system_execution_context*               ctx;
	ecsact_internal_execution_context_linked_list* next;
};

struct ecsact_internal_wasm_system_module_info {
	wasm_module_t*                         system_module;
	wasm_instance_t*                       instance;
	wasm_func_t*                           system_impl_func;
	std::string                            system_impl_func_name;
	wasm_memory_t*                         system_impl_memory;
	wasm_store_t*                          store;
	const ecsact_system_execution_context* parent;
};

ecsact_internal_wasm_system_module_info* get_ecsact_internal_module_info(
	ecsact_system_like_id sys_id
);

wasm_trap_t* wasm_ecsact_system_execution_context_action(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

wasm_trap_t* wasm_ecsact_system_execution_context_add(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

wasm_trap_t* wasm_ecsact_system_execution_context_remove(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

wasm_trap_t* wasm_ecsact_system_execution_context_get(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

wasm_trap_t* wasm_ecsact_system_execution_context_update(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

wasm_trap_t* wasm_ecsact_system_execution_context_has(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

wasm_trap_t* wasm_ecsact_system_execution_context_generate(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

wasm_trap_t* wasm_ecsact_system_execution_context_parent(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

wasm_trap_t* wasm_ecsact_system_execution_context_same(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

wasm_trap_t* wasm_ecsact_system_execution_context_other(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

wasm_trap_t* wasm_ecsact_system_execution_context_entity(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

#endif // WASM_ECSACT_SYSTEM_EXECUTION__H
