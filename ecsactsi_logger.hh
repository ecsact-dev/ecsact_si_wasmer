#pragma once

#include <string>
#include <mutex>
#include <vector>

#include "ecsactsi_wasm.h"

namespace ecsactsi_wasm::detail {

struct log_line_entry {
	ecsactsi_wasm_log_level log_level = {};
	std::string             message;
};

class log_transaction {
	std::unique_lock<std::mutex> _lk;
	log_transaction(std::mutex& m);
	friend auto start_transaction() -> log_transaction;

public:
	log_transaction(log_transaction&&);
	~log_transaction();
};

auto start_transaction() -> log_transaction;

auto push_log_line(const log_transaction& transaction, log_line_entry entry)
	-> void;

auto get_log_lines(const log_transaction& transaction)
	-> const std::vector<log_line_entry>&;

auto clear_log_lines(const log_transaction& transaction) -> void;

} // namespace ecsactsi_wasm::detail
