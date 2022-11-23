workspace(name = "ecsactsi_wasm")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "ecsact_runtime",
    sha256 = "",
    strip_prefix = "ecsact_runtime-95b1730cce14c0bb9d3521a89f207a21d5c954d8",
    url = "https://github.com/ecsact-dev/ecsact_runtime/archive/95b1730cce14c0bb9d3521a89f207a21d5c954d8.zip",
)

http_archive(
    name = "rules_ecsact",
    sha256 = "e6d888c63aa536b5b7c6af10d217cdb8ad98b2262fa9d02515a99edbd0d94eea",
    strip_prefix = "rules_ecsact-0.2.1",
    url = "https://github.com/ecsact-dev/rules_ecsact/archive/refs/tags/0.2.1.tar.gz",
)

http_archive(
    name = "ecsact_lang_cpp",
    sha256 = "f66c5d31cc303022c313a424f4c432c41ff517b73143915436c6c6f06a969a47",
    strip_prefix = "ecsact_lang_cpp-34d73ebce5e99ae9711efa79a30ca7acda8599f0",
    url = "https://github.com/ecsact-dev/ecsact_lang_cpp/archive/34d73ebce5e99ae9711efa79a30ca7acda8599f0.zip",
)

load("@rules_ecsact//ecsact:repositories.bzl", "ecsact_register_toolchains", "rules_ecsact_dependencies")

rules_ecsact_dependencies()

ecsact_register_toolchains()

http_archive(
    name = "ecsact_rt_entt",
    sha256 = "da70a04bdddfb9e652e24caa5a2f09811006b2c6fe456239cc0fef1cd6ed1169",
    strip_prefix = "ecsact_rt_entt-b1a3c4b44113997ea8227e22ec18b39b30434f95",
    urls = ["https://github.com/ecsact-dev/ecsact_rt_entt/archive/b1a3c4b44113997ea8227e22ec18b39b30434f95.zip"],
)

http_archive(
    name = "com_github_skypjack_entt",
    sha256 = "f7031545130bfc06f5fe6b2f8c87dcbd4c1254fab86657e2788b70dfeea57965",
    strip_prefix = "entt-3.10.1",
    url = "https://github.com/skypjack/entt/archive/refs/tags/v3.10.1.tar.gz",
)

http_archive(
    name = "boost",
    sha256 = "c41441a6e9f8038ad626e9f937ddc3675ab896b6c3512eefc6840037b3816d03",
    strip_prefix = "boost-563e8e0de4eac4b48a02d296581dc2454127608e",
    urls = ["https://github.com/bazelboost/boost/archive/563e8e0de4eac4b48a02d296581dc2454127608e.zip"],
)

load("@boost//:index.bzl", "boost_http_archives")

boost_http_archives()

_nlohmann_json_build_file = """
load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "json",
    visibility = ["//visibility:public"],
    includes = ["include"],
    hdrs = glob(["include/**/*.hpp"]),
    strip_include_prefix = "include",
)
"""

http_archive(
    name = "nlohmann_json",
    build_file_content = _nlohmann_json_build_file,
    sha256 = "62c585468054e2d8e7c2759c0d990fd339d13be988577699366fe195162d16cb",
    url = "https://github.com/nlohmann/json/releases/download/v3.10.4/include.zip",
)

http_archive(
    name = "com_google_protobuf",
    sha256 = "7308590dbb95e77066b99c5674eed855c8257e70658d2af586f4a81ff0eea2b1",
    strip_prefix = "protobuf-3.18.0",
    urls = ["https://github.com/protocolbuffers/protobuf/releases/download/v3.18.0/protobuf-cpp-3.18.0.tar.gz"],
)

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

protobuf_deps()

http_archive(
    name = "com_grail_bazel_toolchain",
    sha256 = "644b7c35adbfbf312d86176660a196e67e7a2c3f20114941a60d0379f2b4771f",
    strip_prefix = "bazel-toolchain-9e71d562023dc7994e747110ee1ca345ad6b4413",
    url = "https://github.com/grailbio/bazel-toolchain/archive/9e71d562023dc7994e747110ee1ca345ad6b4413.zip",
)

load("@com_grail_bazel_toolchain//toolchain:deps.bzl", "bazel_toolchain_dependencies")

bazel_toolchain_dependencies()

load("@com_grail_bazel_toolchain//toolchain:rules.bzl", "llvm_toolchain")

llvm_toolchain(
    name = "llvm_toolchain",
    cxx_standard = {"linux": "c++20"},
    llvm_version = "14.0.0",
)

load("@llvm_toolchain//:toolchains.bzl", "llvm_register_toolchains")

llvm_register_toolchains()

git_repository(
    name = "ecsact_entt",
    commit = "47fdb9612d2f1273592aeb3434c2b3110662c9b7",
    remote = "git@github.com:seaube/ecsact-entt.git",
)

load("//:wasmer.bzl", "wasmer_repo")

wasmer_repo(
    name = "wasmer",
    wasmer_version = "2.3.0",
)

http_archive(
    name = "magic_enum",
    sha256 = "5e7680e877dd4cf68d9d0c0e3c2a683b432a9ba84fc1993c4da3de70db894c3c",
    strip_prefix = "magic_enum-0.8.0",
    urls = ["https://github.com/Neargye/magic_enum/archive/refs/tags/v0.8.0.tar.gz"],
)

http_archive(
    name = "hedron_compile_commands",
    sha256 = "cb29ade67efd170c98b86fe75524fc053c01dcbe1f6211d00ce658e57441ed42",
    strip_prefix = "bazel-compile-commands-extractor-670e86177b6b5c001b03f4efdfba0f8019ff523f",
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/670e86177b6b5c001b03f4efdfba0f8019ff523f.tar.gz",
)

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")

hedron_compile_commands_setup()

http_archive(
    name = "emsdk",
    sha256 = "a755b38d5f5c6e5d73ff0f08dc9f7f2e0305230a5585cac82a02064740e93303",
    strip_prefix = "emsdk-50586ea897efd496f94ddf55236fbdd54c60465c/bazel",
    url = "https://github.com/zaucy/emsdk/archive/50586ea897efd496f94ddf55236fbdd54c60465c.zip",
)

load("@emsdk//:deps.bzl", emsdk_deps = "deps")

emsdk_deps()

load("@emsdk//:emscripten_deps.bzl", emsdk_emscripten_deps = "emscripten_deps")

emsdk_emscripten_deps()

load("@emsdk//:toolchains.bzl", "register_emscripten_toolchains")

register_emscripten_toolchains()
