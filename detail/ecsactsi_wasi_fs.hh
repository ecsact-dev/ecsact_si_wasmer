#pragma once

#include <cstdint>
#include <cstdio>
#include <string_view>

namespace ecsactsi_wasi::detail::fs {
auto allow_file_read_access(
	std::string_view real_path,
	std::string_view virtual_path
) -> std::int32_t;

auto real_path(int32_t fd) -> std::string;
auto real_path(std::string_view virtual_path) -> std::string;

auto ensure_open(int32_t pseudo_fd) -> std::FILE*;
auto close(int32_t pseudo_fd) -> void;
} // namespace ecsactsi_wasi::detail::fs
