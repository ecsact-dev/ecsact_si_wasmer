#ifndef ECSACTSI_WASM_H
#define ECSACTSI_WASM_H

#include <ecsact/runtime/common.h>

typedef enum ecsactsi_wasm_error {
	/**
	 * No error.
	 */
	ECSACTSI_WASM_OK,

	/**
	 * Unable to open WASM file.
	 */
	ECSACTSI_WASM_ERR_FILE_OPEN_FAIL,

	/**
	 * Unable to read WASM file.
	 */
	ECSACTSI_WASM_ERR_FILE_READ_FAIL,

	/**
	 * No system implementation export found.
	 */
	ECSACTSI_WASM_ERR_INVALID_EXPORT,
} ecsactsi_wasm_error;

/**
 * Load WASM file at path `wasm_file_path` and call 
 * `ecsact_set_system_execution_impl` for each wasm export that matches the
 * system names retrieved from the ecsact runtime meta API. If your runtime
 * does not have the meta API use `ecsactsi_wasm_load_file` instead.
 * @param wasm_file_path path to WASM file
 * @returns `ECSACTSI_WASM_OK` if successful
 */
ecsactsi_wasm_error ecsactsi_wasm_load_file_meta
	( const char*  wasm_file_path
	);

/**
 * Load WASM file at path `wasm_file_path` and call 
 * `ecsact_set_system_execution_impl` for the specified `system_ids` matching
 * the `wasm_exports` names.
 * @param wasm_file_path path to WASM file
 * @param systems_count Length of `system_ids` and `wasm_exports`
 * @param system_ids Sequential array of system ids that will have their system 
 *        implementations set to by the wasm exports dicated by `wasm_exports`
 *        in the same order. Length is determined by `systems_count`.
 * @param wasm_exports Sequential array of wasm export names used as system 
 *        implementations in the same order as `system_ids`. Length is
 *        determined by `systems_count`.
 * @return `ECSACTSI_WASM_OK` if no error. If there is an error for _any_ of the
 *         systems then **none of the systems are loaded**.
 */
ecsactsi_wasm_error ecsactsi_wasm_load_file
	( const char*        wasm_file_path
	, int                systems_count
	, ecsact_system_id*  system_ids
	, const char**       wasm_exports
	);

#endif//ECSACTSI_WASM_H
