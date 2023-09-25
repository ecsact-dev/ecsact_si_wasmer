load("@bazel_skylib//lib:selects.bzl", "selects")

linkopts = selects.with_or({
    (Label("//bazel:compiler_emscripten")): [
    ],
    ("@rules_cc//cc/compiler:clang"): [
        "-lc++experimental",
    ],
    ("@rules_cc//cc/compiler:msvc-cl", "@rules_cc//cc/compiler:clang-cl"): [
    ],
})
