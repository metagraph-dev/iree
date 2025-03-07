# Copyright 2019 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# A handwritten port of ruy's Bazel BUILD and *.bzl files.

set(RUY_SRC_ROOT
  "${IREE_ROOT_DIR}/third_party/ruy/ruy/"
)

iree_select_compiler_opts(RUY_COPTS_BASE
  ALL
    "-I${IREE_ROOT_DIR}/third_party/ruy"
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
  # This is assuming modern toolchains where no -mfloat-abi flag is
  # needed to actually enable NEON.
  set(RUY_COPTS_NEON -mfpu=neon)
endif()

set(RUY_COPTS
  ${RUY_COPTS_BASE}
  ${RUY_COPTS_NEON}
)

string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" _CMAKE_SYSTEM_PROCESSOR_LOWER)
if(_CMAKE_SYSTEM_PROCESSOR_LOWER STREQUAL x86_64 OR
   _CMAKE_SYSTEM_PROCESSOR_LOWER STREQUAL amd64)
  iree_select_compiler_opts(RUY_COPTS_AVX
    CLANG_OR_GCC
      -mavx
    MSVC_OR_CLANG_CL
      /arch:AVX
  )
  iree_select_compiler_opts(RUY_COPTS_AVX2_FMA
    CLANG_OR_GCC
      -mavx2
      -mfma
    MSVC_OR_CLANG_CL
      /arch:AVX2
  )
  iree_select_compiler_opts(RUY_COPTS_AVX512
    CLANG_OR_GCC
      -mavx512f
      -mavx512vl
      -mavx512cd
      -mavx512bw
      -mavx512dq
    MSVC_OR_CLANG_CL
      /arch:AVX512
  )
endif()

iree_select_compiler_opts(RUY_LINKOPTS_PTHREAD
  GCC
    -pthread
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    platform
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "platform.h"
  COPTS
    ${RUY_COPTS}
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    check_macros
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "check_macros.h"
  COPTS
    ${RUY_COPTS}
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    opt_set
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "opt_set.h"
  COPTS
    ${RUY_COPTS}
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    time
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "time.h"
  COPTS
    ${RUY_COPTS}
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    wait
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "wait.cc"
  HDRS
    "wait.h"
  COPTS
    ${RUY_COPTS}
  LINKOPTS
    ${RUY_LINKOPTS_PTHREAD}
  DEPS
    ruy::time
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    size_util
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "size_util.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::check_macros
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    tune
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "tune.cc"
  HDRS
    "tune.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::opt_set
    ruy::platform
    ruy::time
    ruy::cpuinfo
    ruy::cpu_cache_params
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    system_aligned_alloc
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "system_aligned_alloc.h"
  SRCS
    "system_aligned_alloc.cc"
  COPTS
    ${RUY_COPTS}
)


external_cc_library(
  PACKAGE
    ruy
  NAME
    prepacked_cache
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "prepacked_cache.cc"
  HDRS
    "prepacked_cache.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::system_aligned_alloc
    ruy::mat
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    allocator
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "allocator.cc"
  HDRS
    "allocator.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::opt_set
    ruy::size_util
    ruy::system_aligned_alloc
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    side_pair
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "side_pair.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::check_macros
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    block_map
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "block_map.cc"
  HDRS
    "block_map.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::check_macros
    ruy::opt_set
    ruy::side_pair
    ruy::size_util
    ruy::cpu_cache_params
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    blocking_counter
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "blocking_counter.cc"
  HDRS
    "blocking_counter.h"
  COPTS
    ${RUY_COPTS}
  LINKOPTS
    ${RUY_LINKOPTS_PTHREAD}
  DEPS
    ruy::check_macros
    ruy::wait
    ruy::time
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    thread_pool
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "thread_pool.cc"
  HDRS
    "thread_pool.h"
  COPTS
    ${RUY_COPTS}
  LINKOPTS
    ${RUY_LINKOPTS_PTHREAD}
  DEPS
    ruy::blocking_counter
    ruy::check_macros
    ruy::wait
    ruy::time
  PUBLIC
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    cpu_cache_params
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "path.h"
  COPTS
    ${RUY_COPTS}
  PUBLIC
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    cpuinfo
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "cpuinfo.h"
  SRCS
    "cpuinfo.cc"
  COPTS
    ${RUY_COPTS}
    -I${IREE_ROOT_DIR}/third_party/cpuinfo
    # TODO(benoitjacob): this should be done by either a configure check
    # or a port of the Bazel logic.
    "-DRUY_HAVE_CPUINFO"
  PUBLIC
  DEPS
    ruy::platform
    ruy::check_macros
    ruy::cpu_cache_params
    cpuinfo
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    path
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "path.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::platform
    ruy::size_util
  PUBLIC
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    performance_advisory
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "performance_advisory.h"
  COPTS
    ${RUY_COPTS}
  PUBLIC
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    matrix
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "matrix.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::check_macros
  PUBLIC
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    mul_params
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "mul_params.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::check_macros
    ruy::size_util
  PUBLIC
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    mat
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "mat.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::check_macros
    ruy::matrix
    ruy::size_util
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    asm_helpers
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "asm_helpers.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::opt_set
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    apply_multiplier
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "apply_multiplier.h"
  SRCS
    "apply_multiplier.cc"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::check_macros
    ruy::mul_params
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    kernel_common
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "kernel_common.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::check_macros
    ruy::apply_multiplier
    ruy::mat
    ruy::matrix
    ruy::opt_set
    ruy::path
    ruy::platform
    ruy::side_pair
    ruy::size_util
    ruy::mul_params
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    pack_common
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "pack_common.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::check_macros
    ruy::mat
    ruy::matrix
    ruy::opt_set
    ruy::path
    ruy::platform
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    kernel_arm
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "kernel_arm.h"
  SRCS
    "kernel_arm32.cc"
    "kernel_arm64.cc"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::asm_helpers
    ruy::check_macros
    ruy::kernel_common
    ruy::mat
    ruy::mul_params
    ruy::opt_set
    ruy::platform
    ruy::path
    ruy::side_pair
    ruy::size_util
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    pack_arm
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "pack_arm.h"
  SRCS
    "pack_arm.cc"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::asm_helpers
    ruy::check_macros
    ruy::mat
    ruy::opt_set
    ruy::pack_common
    ruy::platform
    ruy::path
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    kernel_avx512
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "kernel_x86.h"
  SRCS
    "kernel_avx512.cc"
  COPTS
    ${RUY_COPTS}
    ${RUY_COPTS_AVX512}
  DEPS
    ruy::check_macros
    ruy::kernel_common
    ruy::mat
    ruy::mul_params
    ruy::path
    ruy::opt_set
    ruy::platform
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    pack_avx512
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "pack_x86.h"
  SRCS
    "pack_avx512.cc"
  COPTS
    ${RUY_COPTS}
    ${RUY_COPTS_AVX512}
  DEPS
    ruy::check_macros
    ruy::mat
    ruy::opt_set
    ruy::pack_common
    ruy::path
    ruy::platform
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    have_built_path_for_avx512
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "have_built_path_for_avx512.cc"
  HDRS
    "have_built_path_for.h"
  COPTS
    ${RUY_COPTS}
    ${RUY_COPTS_AVX512}
  DEPS
    ruy::opt_set
    ruy::platform
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    kernel_avx2_fma
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "kernel_x86.h"
  SRCS
    "kernel_avx2_fma.cc"
  COPTS
    ${RUY_COPTS}
    ${RUY_COPTS_AVX2_FMA}
  DEPS
    ruy::check_macros
    ruy::kernel_common
    ruy::mat
    ruy::mul_params
    ruy::path
    ruy::opt_set
    ruy::platform
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    pack_avx2_fma
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "pack_x86.h"
  SRCS
    "pack_avx2_fma.cc"
  COPTS
    ${RUY_COPTS}
    ${RUY_COPTS_AVX2_FMA}
  DEPS
    ruy::check_macros
    ruy::mat
    ruy::opt_set
    ruy::pack_common
    ruy::path
    ruy::platform
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    have_built_path_for_avx2_fma
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "have_built_path_for_avx2_fma.cc"
  HDRS
    "have_built_path_for.h"
  COPTS
    ${RUY_COPTS}
    ${RUY_COPTS_AVX2_FMA}
  DEPS
    ruy::opt_set
    ruy::platform
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    kernel_avx
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "kernel_x86.h"
  SRCS
    "kernel_avx.cc"
  COPTS
    ${RUY_COPTS}
    ${RUY_COPTS_AVX}
  DEPS
    ruy::check_macros
    ruy::kernel_common
    ruy::mat
    ruy::mul_params
    ruy::path
    ruy::opt_set
    ruy::platform
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    pack_avx
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "pack_x86.h"
  SRCS
    "pack_avx.cc"
  COPTS
    ${RUY_COPTS}
    ${RUY_COPTS_AVX}
  DEPS
    ruy::check_macros
    ruy::mat
    ruy::opt_set
    ruy::pack_common
    ruy::path
    ruy::platform
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    have_built_path_for_avx
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "have_built_path_for_avx.cc"
  HDRS
    "have_built_path_for.h"
  COPTS
    ${RUY_COPTS}
    ${RUY_COPTS_AVX}
  DEPS
    ruy::opt_set
    ruy::platform
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    kernel
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "kernel.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::check_macros
    ruy::apply_multiplier
    ruy::mat
    ruy::kernel_arm
    ruy::kernel_avx
    ruy::kernel_avx2_fma
    ruy::kernel_avx512
    ruy::kernel_common
    ruy::matrix
    ruy::mul_params
    ruy::opt_set
    ruy::path
    ruy::platform
    ruy::side_pair
    ruy::size_util
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    pack
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "pack.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::check_macros
    ruy::mat
    ruy::matrix
    ruy::opt_set
    ruy::pack_arm
    ruy::pack_avx
    ruy::pack_avx2_fma
    ruy::pack_avx512
    ruy::pack_common
    ruy::path
    ruy::platform
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    have_built_path_for
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "have_built_path_for.h"
  DEPS
    ruy::have_built_path_for_avx
    ruy::have_built_path_for_avx2_fma
    ruy::have_built_path_for_avx512
    ruy::platform
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    context
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "context.h"
  SRCS
    "context.cc"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::allocator
    ruy::check_macros
    ruy::ctx
    ruy::have_built_path_for
    ruy::path
    ruy::performance_advisory
    ruy::platform
    ruy::prepacked_cache
    ruy::thread_pool
    ruy::tune
  PUBLIC
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    ctx
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "ctx.cc"
  HDRS
    "ctx.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::allocator
    ruy::check_macros
    ruy::cpuinfo
    ruy::have_built_path_for
    ruy::path
    ruy::performance_advisory
    ruy::platform
    ruy::prepacked_cache
    ruy::thread_pool
    ruy::tune
  PUBLIC
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    context_get_ctx
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "context_get_ctx.cc"
  HDRS
    "context_get_ctx.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::context
    ruy::ctx
  PUBLIC
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    trmul_params
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "trmul_params.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::mat
    ruy::mul_params
    ruy::path
    ruy::side_pair
    ruy::tune
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    trmul
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "trmul.cc"
  HDRS
    "trmul.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::allocator
    ruy::block_map
    ruy::check_macros
    ruy::cpu_cache_params
    ruy::cpuinfo
    ruy::ctx
    ruy::mat
    ruy::matrix
    ruy::mul_params
    ruy::opt_set
    ruy::side_pair
    ruy::size_util
    ruy::thread_pool
    ruy::trmul_params
    ruy::tune
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    prepare_packed_matrices
  ROOT
    ${RUY_SRC_ROOT}
  SRCS
    "prepare_packed_matrices.cc"
  HDRS
    "prepare_packed_matrices.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::allocator
    ruy::ctx
    ruy::matrix
    ruy::prepacked_cache
    ruy::side_pair
    ruy::trmul_params
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    create_trmul_params
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "create_trmul_params.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::allocator
    ruy::check_macros
    ruy::ctx
    ruy::kernel
    ruy::mat
    ruy::mul_params
    ruy::pack
    ruy::path
    ruy::performance_advisory
    ruy::platform
    ruy::side_pair
    ruy::trmul_params
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    validate
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "validate.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::check_macros
    ruy::mat
    ruy::mul_params
    ruy::side_pair
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    frontend
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "frontend.h"
  SRCS
    "frontend.cc"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::allocator
    ruy::create_trmul_params
    ruy::ctx
    ruy::mat
    ruy::mul_params
    ruy::prepare_packed_matrices
    ruy::trmul
    ruy::trmul_params
    ruy::validate
    ruy::profiler_instrumentation
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    ruy
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "ruy.h"
  COPTS
    ${RUY_COPTS}
  DEPS
    ruy::context
    ruy::context_get_ctx
    ruy::frontend
    ruy::mat
    ruy::path
    ruy::mul_params
  PUBLIC
)

external_cc_library(
  PACKAGE
    ruy
  NAME
    profiler_instrumentation
  ROOT
    ${RUY_SRC_ROOT}
  HDRS
    "profiler/instrumentation.h"
  SRCS
    "profiler/instrumentation.cc"
  COPTS
    ${RUY_COPTS}
  LINKOPTS
    ${RUY_LINKOPTS_PTHREAD}
  PUBLIC
)
