/**
 * @file
 * @brief Ecsact System Implementation with WebAssembly header
 */

#ifndef ECSACTSI_WASM_H
#define ECSACTSI_WASM_H

#include "ecsact/runtime/common.h"

#ifndef ECSACTSI_WASM_API_FN
#	if defined(ECSACTSI_WASM_API)
#		define ECSACTSI_WASM_API_FN(ret, name) ECSACTSI_WASM_API ret name
#	elif defined(ECSACTSI_WASM_API_LOAD_AT_RUNTIME)
#		if defined(ECSACTSI_WASM_API_EXPORT)
#			define ECSACTSI_WASM_API_FN(ret, name) \
				ECSACT_EXTERN ECSACT_EXPORT(#name) ret(*name)
#		else
#			define ECSACTSI_WASM_API_FN(ret, name) \
				ECSACT_EXTERN ECSACT_IMPORT("env", #name) ret(*name)
#		endif
#	elif defined(ECSACTSI_WASM_API_EXPORT)
#		define ECSACTSI_WASM_API_FN(ret, name) \
			ECSACT_EXTERN ECSACT_EXPORT(#name) ret name
#	else
#		define ECSACTSI_WASM_API_FN(ret, name) \
			ECSACT_EXTERN ECSACT_IMPORT("env", #name) ret name
#	endif
#endif // ECSACTSI_WASM_API_FN

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
	 * Unable to compile WASM file module.
	 */
	ECSACTSI_WASM_ERR_COMPILE_FAIL,

	/**
	 * Unable to instantiate WASM file module.
	 */
	ECSACTSI_WASM_ERR_INSTANTIATE_FAIL,

	/**
	 * Export name was not found in WASM file.
	 */
	ECSACTSI_WASM_ERR_EXPORT_NOT_FOUND,

	/**
	 * Export name was found, but was not a function.
	 */
	ECSACTSI_WASM_ERR_EXPORT_INVALID,

	/**
	 * WASM file contains an unknown guest import.
	 */
	ECSACTSI_WASM_ERR_GUEST_IMPORT_UNKNOWN,

	/**
	 * WASM file contains correctly named guest import, but was not a function
	 */
	ECSACTSI_WASM_ERR_GUEST_IMPORT_INVALID,

	/**
	 * Invoking `_initialize()` resulted in a wasm trap
	 */
	ECSACTSI_WASM_ERR_INITIALIZE_FAIL,

	/**
	 * Ecsact dynamic module is 'load at runtime' but
	 * 'ecsact_set_system_execution_impl' has not been set.
	 */
	ECSACTSI_WASM_ERR_NO_SET_SYSTEM_EXECUTION,
} ecsactsi_wasm_error;

ECSACTSI_WASM_API_FN(void, ecsactsi_wasm_last_error_message)
( //
	char*   out_message,
	int32_t out_message_max_length
);

ECSACTSI_WASM_API_FN(int32_t, ecsactsi_wasm_last_error_message_length)();

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
ECSACTSI_WASM_API_FN(ecsactsi_wasm_error, ecsactsi_wasm_load_file)
( //
	const char*            wasm_file_path,
	int                    systems_count,
	ecsact_system_like_id* system_ids,
	const char**           wasm_exports
);

ECSACTSI_WASM_API_FN(ecsactsi_wasm_error, ecsactsi_wasm_load)
( //
	char*                  wasm_data,
	int                    wasm_data_size,
	int                    systems_count,
	ecsact_system_like_id* system_ids,
	const char**           wasm_exports
);

/**
 * Unload 1 or more systems. If a system is not already loaded this is a noop.
 * @param systems_count number of systems impls to unload
 * @param system_ids array of system IDs to unload. Sequential list length of
 *        @p systems_count.
 */
ECSACTSI_WASM_API_FN(void, ecsactsi_wasm_unload)
( //
	int                    systems_count,
	ecsact_system_like_id* system_ids
);

/**
 * Reset state. Effectively called `ecsactsi_wasm_unload` for each system
 * implementation and clears the trap handler.
 */
ECSACTSI_WASM_API_FN(void, ecsactsi_wasm_reset)();

/**
 * @param system_id System ID associated with the impl that triggered the trap
 * @param trap_message The trap message contents. Null-terminated string.
 */
typedef void (*ecsactsi_wasm_trap_handler)( //
	ecsact_system_like_id system_id,
	const char*           trap_message
);

/**
 * Register a function to be called when a system implementation trap occurs. It
 * is recommended that a trap handler is set otherwise the trap message will be
 * quietly discarded.
 * @param handler The handler function that will be called when a system impl
 *        function trap occurs. Calling this overwrites the last handler. May be
 *        `NULL` to remove the current handler.
 */
ECSACTSI_WASM_API_FN(void, ecsactsi_wasm_set_trap_handler)
( //
	ecsactsi_wasm_trap_handler handler
);

typedef enum ecsactsi_wasm_log_level {
	ECSACTSI_WASM_LOG_LEVEL_INFO = 0,
	ECSACTSI_WASM_LOG_LEVEL_WARNING = 1,
	ECSACTSI_WASM_LOG_LEVEL_ERROR = 2,
} ecsactsi_wasm_log_level;

typedef void (*ecsactsi_wasm_log_consumer)( //
	ecsactsi_wasm_log_level log_level,
	const char*             message,
	int32_t                 message_length,
	void*                   user_data
);

/**
 * Invokes @p consumer for 1 or more lines that have been printed to stdout or
 * stderr between the last `ecsactsi_wasm_consume_logs` call until there are no
 * more lines left to consume.
 */
ECSACTSI_WASM_API_FN(void, ecsactsi_wasm_consume_logs)
( //
	ecsactsi_wasm_log_consumer consumer,
	void*                      consumer_user_data
);

/**
 * Expose a file on the host for read access during initialization.
 */
ECSACTSI_WASM_API_FN(int32_t, ecsactsi_wasm_allow_file_read_access)
( //
	const char* real_file_path,
	int32_t     real_file_path_length,
	const char* virtual_file_path,
	int32_t     virtual_file_path_length
);

// # BEGIN FOR_EACH_ECSACTSI_WASM_API_FN
#ifdef ECSACT_MSVC_TRADITIONAL
#	define FOR_EACH_ECSACTSI_WASM_API_FN(fn, ...) ECSACT_MSVC_TRADITIONAL_ERROR()
#else
#	define FOR_EACH_ECSACTSI_WASM_API_FN(fn, ...)              \
		fn(ecsactsi_wasm_last_error_message, __VA_ARGS__);        \
		fn(ecsactsi_wasm_last_error_message_length, __VA_ARGS__); \
		fn(ecsactsi_wasm_load_file, __VA_ARGS__);                 \
		fn(ecsactsi_wasm_load, __VA_ARGS__);                      \
		fn(ecsactsi_wasm_unload, __VA_ARGS__);                    \
		fn(ecsactsi_wasm_reset, __VA_ARGS__);                     \
		fn(ecsactsi_wasm_set_trap_handler, __VA_ARGS__);          \
		fn(ecsactsi_wasm_consume_logs, __VA_ARGS__);              \
		fn(ecsactsi_wasm_allow_file_read_access, __VA_ARGS__)
#endif

#endif // ECSACTSI_WASM_H
