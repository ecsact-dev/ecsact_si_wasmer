#ifndef ECSACTSI_WASI_H
#define ECSACTSI_WASI_H

#include <wasm.h>
#include <stdint.h>

/**
 * A region of memory for scatter/gather writes.
 */
typedef struct ecsactsi_wasi_ciovec_t {
	/**
	 * The address of the buffer to be written.
	 */
	const uint8_t* buf;

	/**
	 * The length of the buffer to be written.
	 */
	size_t buf_len;
} ecsactsi_wasi_ciovec_t;

/**
 * Ecsact system implementation exited prematurely. Unlike normal usage of this
 * function @p exit_code being `0` does NOT mean success.
 * @param exit_code may be logged to the host, but otherwise is meaningless to
 *        the ecsact wasm host.
 */
// void ecsactsi_wasi_proc_exit(int32_t exit_code);
wasm_trap_t* ecsactsi_wasi_proc_exit(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

// TODO
// uint16_t ecsactsi_wasi_fd_seek(
// 	int32_t   fd,
// 	int64_t   offset,
// 	uint8_t   whence,
// 	uint64_t* retptr0
// );
wasm_trap_t* ecsactsi_wasi_fd_seek(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

// TODO
// uint16_t ecsactsi_wasi_fd_write(
// 	int32_t                       fd,
// 	const ecsactsi_wasi_ciovec_t* iovs,
// 	size_t                        iovs_len,
// 	size_t*                       retptr0
// );
wasm_trap_t* ecsactsi_wasi_fd_write(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

// TODO
// uint16_t ecsactsi_wasi_fd_read(
// 	int32_t                 fd,
// 	ecsactsi_wasi_ciovec_t* iovs,
// 	size_t                  iovs_len,
// 	size_t*                 retptr0
// );

wasm_trap_t* ecsactsi_wasi_fd_read(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

// TODO
// uint16_t ecsactsi_wasi_fd_close(int32_t fd);

wasm_trap_t* ecsactsi_wasi_fd_close(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

// TODO
// uint16_t ecsactsi_wasi_environ_sizes_get(size_t* retptr0, size_t* retptr1);
wasm_trap_t* ecsactsi_wasi_environ_sizes_get(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

// TODO
// uint16_t ecsactsi_wasi_environ_get(uint8_t** environ, uint8_t* environ_buf);

wasm_trap_t* ecsactsi_wasi_environ_get(
	const wasm_val_vec_t* args,
	wasm_val_vec_t*       results
);

#endif // ECSACTSI_WASI_H
