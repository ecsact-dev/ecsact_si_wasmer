#pragma once

#include "ecsact/wasm/detail/guest_imports.hh"
#include "ecsact/wasm/detail/util.hh"
#include "ecsact/wasm/detail/wasi.hh"

namespace ecsact::wasm::detail {
const auto guest_wasi_module_imports = allowed_guest_imports_t{
	{
		"proc_exit",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type =
				wasm_functype_new_1_0(wasm_valtype_new(WASM_I32) // exit_code
				);
			wasm_func_t* fn = wasm_func_new(store, fn_type, &ecsactsi_wasi_proc_exit);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"fd_seek",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_4_1(
				wasm_valtype_new_i32(), // fd
				wasm_valtype_new_i64(), // offset
				wasm_valtype_new_i32(), // whence
				wasm_valtype_new_i32(), // retptr0
				wasm_valtype_new_i32() // error code (return)
			);
			wasm_func_t* fn = wasm_func_new(store, fn_type, &ecsactsi_wasi_fd_seek);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"fd_write",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_4_1(
				wasm_valtype_new_i32(), // fd
				wasm_valtype_new_i32(), // iovs
				wasm_valtype_new_i32(), // iovs_len
				wasm_valtype_new_i32(), // retptr0
				wasm_valtype_new_i32() // error code (return)
			);
			wasm_func_t* fn = wasm_func_new(store, fn_type, &ecsactsi_wasi_fd_write);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"fd_read",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_4_1(
				wasm_valtype_new_i32(), // fd
				wasm_valtype_new_i32(), // iovs
				wasm_valtype_new_i32(), // iovs_len
				wasm_valtype_new_i32(), // retptr0
				wasm_valtype_new_i32() // error code (return)
			);
			wasm_func_t* fn = wasm_func_new(store, fn_type, &ecsactsi_wasi_fd_read);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"fd_close",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_1_1(
				wasm_valtype_new_i32(), // fd
				wasm_valtype_new_i32() // error code (return)
			);
			wasm_func_t* fn = wasm_func_new(store, fn_type, &ecsactsi_wasi_fd_close);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"environ_sizes_get",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_2_1(
				wasm_valtype_new_i32(), // retptr0
				wasm_valtype_new_i32(), // retptr1
				wasm_valtype_new_i32() // error code (return)
			);
			wasm_func_t* fn =
				wasm_func_new(store, fn_type, &ecsactsi_wasi_environ_sizes_get);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"environ_get",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_2_1(
				wasm_valtype_new_i32(), // environ
				wasm_valtype_new_i32(), // environ_buf
				wasm_valtype_new_i32() // error code (return)
			);
			wasm_func_t* fn =
				wasm_func_new(store, fn_type, &ecsactsi_wasi_environ_get);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
	{
		"fd_fdstat_get",
		[](wasm_store_t* store) -> wasm_func_t* {
			wasm_functype_t* fn_type = wasm_functype_new_2_1(
				wasm_valtype_new_i32(), // fd
				wasm_valtype_new_i32(), // retptr0
				wasm_valtype_new_i32() // error code (return)
			);
			wasm_func_t* fn =
				wasm_func_new(store, fn_type, &ecsactsi_wasi_fd_fdstat_get);

			// wasm_functype_delete(fn_type);

			return fn;
		},
	},
};

}
