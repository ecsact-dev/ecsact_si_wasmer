#include "ecsactsi_wasi.h"

#include <cstdio>

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
			std::fprintf(f, "%.*s", static_cast<int>(io.buf_len), io.buf);
			*out_write_amount += io.buf_len;
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
	return nullptr;
}

wasm_trap_t* ecsactsi_wasi_fd_close(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	return nullptr;
}

wasm_trap_t* ecsactsi_wasi_environ_sizes_get(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	return nullptr;
}

wasm_trap_t* ecsactsi_wasi_environ_get(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
) {
	return nullptr;
}
