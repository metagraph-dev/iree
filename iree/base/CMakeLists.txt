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

# bazel_to_cmake: DO NOT EDIT (tracing rule move)

iree_add_all_subdirs()

iree_cc_library(
  NAME
    api
  HDRS
    "api.h"
  SRCS
    "api.c"
  DEPS
    ::core_headers
    ::tracing
  PUBLIC
)

iree_cc_library(
  NAME
    core_headers
  HDRS
    "alignment.h"
    "atomics.h"
    "bitfield.h"
    "debugging.h"
    "math.h"
    "memory.h"
    "target_platform.h"
  DEPS
    absl::core_headers
    absl::span
  PUBLIC
)

iree_cc_test(
  NAME
    bitfield_test
  SRCS
    "bitfield_test.cc"
  DEPS
    ::core_headers
    iree::testing::gtest
    iree::testing::gtest_main
)

iree_cc_library(
  NAME
    arena
  HDRS
    "arena.h"
  SRCS
    "arena.cc"
  DEPS
    ::logging
    absl::core_headers
    absl::span
  PUBLIC
)

iree_cc_test(
  NAME
    arena_test
  SRCS
    "arena_test.cc"
  DEPS
    ::arena
    iree::testing::gtest
    iree::testing::gtest_main
)

iree_cc_library(
  NAME
    atomic_slist
  HDRS
    "atomic_slist.h"
  SRCS
    "atomic_slist.c"
  DEPS
    ::core_headers
    ::synchronization
  PUBLIC
)

iree_cc_test(
  NAME
    atomic_slist_test
  SRCS
    "atomic_slist_test.cc"
  DEPS
    ::atomic_slist
    iree::testing::gtest
    iree::testing::gtest_main
)

iree_cc_library(
  NAME
    dynamic_library
  HDRS
    "dynamic_library.h"
  SRCS
    "dynamic_library_posix.cc"
    "dynamic_library_win32.cc"
  DEPS
    ::core_headers
    ::file_path
    ::logging
    ::status
    ::tracing
    absl::memory
    absl::span
    absl::strings
  PUBLIC
)

iree_cc_library(
  NAME
    file_io
  HDRS
    "file_io.h"
  DEPS
    ::core_headers
    ::status
    absl::memory
    absl::span
    absl::strings
    iree::base::internal::file_io_internal
  PUBLIC
)

iree_cc_library(
  NAME
    file_io_hdrs
  HDRS
    "file_io.h"
  DEPS
    ::status
    absl::strings
  PUBLIC
)

iree_cc_test(
  NAME
    file_io_test
  SRCS
    "file_io_test.cc"
  DEPS
    ::file_io
    ::file_path
    ::logging
    ::status
    absl::strings
    iree::testing::gtest
    iree::testing::gtest_main
)

iree_cc_library(
  NAME
    file_mapping
  HDRS
    "file_mapping.h"
  DEPS
    ::ref_ptr
    ::status
    absl::memory
    absl::span
    absl::strings
    iree::base::internal::file_mapping_internal
  PUBLIC
)

iree_cc_library(
  NAME
    file_mapping_hdrs
  HDRS
    "file_mapping.h"
  DEPS
    ::ref_ptr
    ::status
    absl::span
  PUBLIC
)

iree_cc_library(
  NAME
    file_path
  HDRS
    "file_path.h"
  SRCS
    "file_path.cc"
  DEPS
    absl::strings
  PUBLIC
)

iree_cc_test(
  NAME
    file_path_test
  SRCS
    "file_path_test.cc"
  DEPS
    ::file_path
    iree::testing::gtest
    iree::testing::gtest_main
)

iree_cc_library(
  NAME
    flags
  HDRS
    "flags.h"
  SRCS
    "flags.cc"
  DEPS
    ::api
    absl::flags_parse
  PUBLIC
)

iree_cc_library(
  NAME
    flatcc
  HDRS
    "flatcc.h"
  DEPS
    ::flatcc_dummy
    flatcc::runtime
  PUBLIC
)

flatbuffer_c_library(
  NAME
    flatcc_dummy
  SRCS
    "flatcc.fbs"
  FLATCC_ARGS
    "--reader"
    "--builder"
    "--verifier"
    "--json"
  PUBLIC
)

iree_cc_library(
  NAME
    intrusive_list
  HDRS
    "intrusive_list.h"
    "intrusive_list_ref_ptr.inc"
    "intrusive_list_unique_ptr.inc"
  DEPS
    ::logging
    ::ref_ptr
  PUBLIC
)

iree_cc_test(
  NAME
    intrusive_list_test
  SRCS
    "intrusive_list_ref_ptr_test.cc"
    "intrusive_list_test.cc"
    "intrusive_list_unique_ptr_test.cc"
  DEPS
    ::intrusive_list
    absl::memory
    iree::testing::gtest
    iree::testing::gtest_main
)

iree_cc_library(
  NAME
    logging
  HDRS
    "logging.h"
  SRCS
    "logging.cc"
  DEPS
    ::tracing
    absl::core_headers
    absl::flags
    absl::str_format
  PUBLIC
)

iree_cc_library(
  NAME
    main
  HDRS
    "main.h"
  SRCS
    "main_posix.cc"
    "main_win32.cc"
  DEPS
    ::core_headers
    ::logging
  PUBLIC
)

iree_cc_test(
  NAME
    math_test
  SRCS
    "math_test.cc"
  DEPS
    ::core_headers
    iree::testing::gtest
    iree::testing::gtest_main
)

iree_cc_library(
  NAME
    ref_ptr
  HDRS
    "ref_ptr.h"
  DEPS
    ::logging
    absl::core_headers
  PUBLIC
)

iree_cc_test(
  NAME
    ref_ptr_test
  SRCS
    "ref_ptr_test.cc"
  DEPS
    ::ref_ptr
    iree::testing::gtest
    iree::testing::gtest_main
)

iree_cc_library(
  NAME
    signature_mangle
  HDRS
    "signature_mangle.h"
  SRCS
    "signature_mangle.cc"
  DEPS
    ::status
    absl::inlined_vector
    absl::memory
    absl::optional
    absl::span
    absl::strings
  PUBLIC
)

iree_cc_test(
  NAME
    signature_mangle_test
  SRCS
    "signature_mangle_test.cc"
  DEPS
    ::signature_mangle
    iree::testing::gtest
    iree::testing::gtest_main
)

iree_cc_library(
  NAME
    status
  HDRS
    "status.h"
  DEPS
    iree::base::internal::status_internal
  PUBLIC
)

iree_cc_test(
  NAME
    status_test
  SRCS
    "status_test.cc"
  DEPS
    ::status
    iree::testing::gtest
    iree::testing::gtest_main
)

iree_cc_library(
  NAME
    synchronization
  HDRS
    "synchronization.h"
  SRCS
    "synchronization.c"
  DEPS
    ::api
    ::core_headers
    ::tracing
  PUBLIC
)

iree_cc_binary(
  NAME
    synchronization_benchmark
  SRCS
    "synchronization_benchmark.cc"
  DEPS
    ::synchronization
    benchmark
    iree::testing::benchmark_main
  TESTONLY
)

iree_run_binary_test(
  NAME
    synchronization_benchmark_test
  TEST_BINARY
    ::synchronization_benchmark
  ARGS
    "--benchmark_min_time=0"
)

iree_cc_test(
  NAME
    synchronization_test
  SRCS
    "synchronization_test.cc"
  DEPS
    ::synchronization
    iree::testing::gtest
    iree::testing::gtest_main
)

iree_cc_library(
  NAME
    time
  HDRS
    "time.h"
  DEPS
    ::api
  PUBLIC
)

iree_cc_test(
  NAME
    time_test
  SRCS
    "time_test.cc"
  DEPS
    ::time
    iree::testing::gtest
    iree::testing::gtest_main
)

iree_cc_library(
  NAME
    threading
  HDRS
    "threading.h"
  SRCS
    "threading.c"
    "threading_darwin.c"
    "threading_impl.h"
    "threading_pthreads.c"
    "threading_win32.c"
  DEPS
    ::api
    ::core_headers
    ::synchronization
    ::tracing
  PUBLIC
)

iree_cc_test(
  NAME
    threading_benchmark
  SRCS
    "threading_benchmark.cc"
  DEPS
    ::threading
    benchmark
    iree::testing::benchmark_main
)

iree_cc_test(
  NAME
    threading_test
  SRCS
    "threading_impl.h"
    "threading_test.cc"
  DEPS
    ::synchronization
    ::threading
    iree::testing::gtest
    iree::testing::gtest_main
)

# TODO(benvanik): redirect to internal/tracing/ or something.
if(${IREE_ENABLE_RUNTIME_TRACING})
  iree_cc_library(
    NAME
      tracing
    HDRS
      "tracing.h"
      "${IREE_ROOT_DIR}/third_party/tracy/Tracy.hpp"
      "${IREE_ROOT_DIR}/third_party/tracy/TracyC.h"
    SRCS
      "tracing.cc"
    DEPS
      ::core_headers
      absl::core_headers
    DEFINES
      # TODO(#2114): Change the mode to 2.
      "IREE_TRACING_MODE=1"
    PUBLIC
  )
else()
  iree_cc_library(
    NAME
      tracing
    HDRS
      "tracing.h"
    DEPS
      absl::core_headers
    PUBLIC
  )
endif()

iree_cc_library(
  NAME
    wait_handle
  HDRS
    "wait_handle.h"
  SRCS
    "wait_handle.c"
    "wait_handle_epoll.c"
    "wait_handle_impl.h"
    "wait_handle_kqueue.c"
    "wait_handle_poll.c"
    "wait_handle_posix.c"
    "wait_handle_posix.h"
    "wait_handle_win32.c"
  DEPS
    ::api
    ::core_headers
    ::tracing
  PUBLIC
)

iree_cc_test(
  NAME
    wait_handle_test
  SRCS
    "wait_handle_test.cc"
  DEPS
    ::wait_handle
    iree::testing::gtest
    iree::testing::gtest_main
)
