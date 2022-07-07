workspace(name = "ecsactsi_wasm")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

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
    name = "ecsact",
    commit = "9343d507a4fe28bf99d7e9d8c9b49a42b2d754c8",
    remote = "git@github.com:seaube/ecsact.git",
)

git_repository(
    name = "ecsact_entt",
    commit = "47fdb9612d2f1273592aeb3434c2b3110662c9b7",
    remote = "git@github.com:seaube/ecsact-entt.git",
)

load("//:wasmer.bzl", "wasmer_repo")

wasmer_repo(name = "wasmer")

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
