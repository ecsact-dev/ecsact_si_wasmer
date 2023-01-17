#include "ecsactsi_wasi.h"

#include <cstdio>
#include <string_view>

#include "wasm_ecsact_memory.hh"

wasm_trap_t* ecsactsi_wasi_proc_exit(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	return nullptr;
}

wasm_trap_t* ecsactsi_wasi_fd_seek(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = 0;

	return nullptr;
}

wasm_trap_t* ecsactsi_wasi_fd_write(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto mem = ecsactsi_wasm::current_wasm_memory_rw();
	assert(args->data[0].kind == WASM_I32);
	auto fd = args->data[0].of.i32;

	assert(args->data[1].kind == WASM_I32);
	auto iovec = ecsactsi_wasm::wasm_memory_cast<ecsactsi_wasi_ciovec_t>(
		mem,
		args->data[1].of.i32
	);

	assert(args->data[2].kind == WASM_I32);
	auto iovec_len = args->data[2].of.i32;

	assert(args->data[3].kind == WASM_I32);
	auto out_write_amount =
		ecsactsi_wasm::wasm_memory_cast<size_t>(mem, args->data[3].of.i32);

	// Only supporting stdout and stderr for now
	assert(fd == 1 || fd == 2);

	auto f = fd == 1 ? stdout : stderr;

	*out_write_amount = 0;

	for(int i = 0; iovec_len > i; ++i) {
		auto io = iovec[i];

		if(io.buf_len > 0) {
			auto buf = ecsactsi_wasm::wasm_memory_cast<const char>(mem, io.buf);
			auto str = std::string_view(buf, io.buf_len);
			*out_write_amount += io.buf_len;

			std::fprintf(f, "%.*s", static_cast<int>(str.size()), str.data());
		}
	}

	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = 0;

	return nullptr;
}

wasm_trap_t* ecsactsi_wasi_fd_read(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = 0;

	return nullptr;
}

wasm_trap_t* ecsactsi_wasi_fd_close(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = 0;

	return nullptr;
}

wasm_trap_t* ecsactsi_wasi_environ_sizes_get(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto mem = ecsactsi_wasm::current_wasm_memory_rw();
	auto retptr0 =
		ecsactsi_wasm::wasm_memory_cast<size_t>(mem, args->data[3].of.i32);
	auto retptr1 =
		ecsactsi_wasm::wasm_memory_cast<size_t>(mem, args->data[3].of.i32);

	*retptr0 = 0;
	*retptr1 = 0;

	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = 0;

	return nullptr;
}

wasm_trap_t* ecsactsi_wasi_environ_get(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto mem = ecsactsi_wasm::current_wasm_memory_rw();
	auto environ_arg =
		ecsactsi_wasm::wasm_memory_cast<uint8_t>(mem, args->data[0].of.i32);
	auto environ_buf_arg =
		ecsactsi_wasm::wasm_memory_cast<uint8_t>(mem, args->data[1].of.i32);

	// Even though our environment variable size is `0` the API expects the key
	// values pairs to be null terminated.
	environ_arg[0] = '\0';
	environ_buf_arg[0] = '\0';

	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = 0;

	return nullptr;
}

wasm_trap_t* ecsactsi_wasi_fd_fdstat_get(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	auto mem = ecsactsi_wasm::current_wasm_memory_rw();
	assert(args->data[0].kind == WASM_I32);
	auto fd = args->data[0].of.i32;
	assert(args->data[1].kind == WASM_I32);
	auto ret = ecsactsi_wasm::wasm_memory_cast<ecsactsi_wasi_fdstat_t>(
		mem,
		args->data[1].of.i32
	);

	ret->fs_filetype = 0;
	ret->fs_flags = 0;
	ret->fs_rights_base = 0;
	ret->fs_rights_inheriting = 0;

	results->data[0].kind = WASM_I32;
	results->data[0].of.i32 = 0;

	return nullptr;
}
