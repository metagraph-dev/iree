# Copyright 2020 Google LLC
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

# bazel_to_cmake: DO NOT EDIT (custom configuration vars)

set(IREE_HAL_DRIVER_MODULES)
if(${IREE_HAL_DRIVER_DYLIB})
  list(APPEND IREE_HAL_DRIVER_MODULES iree::hal::dylib::registration)
endif()
if(${IREE_HAL_DRIVER_METAL})
  list(APPEND IREE_HAL_DRIVER_MODULES iree::hal::metal::registration)
endif()
if(${IREE_HAL_DRIVER_VMLA})
  list(APPEND IREE_HAL_DRIVER_MODULES iree::hal::vmla::registration)
endif()
if(${IREE_HAL_DRIVER_VULKAN})
  list(APPEND IREE_HAL_DRIVER_MODULES iree::hal::vulkan::registration)
endif()

# TODO: Move to either hal/metal/CMakeLists.txt or
# hal/metal/registration/CMakeLists.txt if bazel-to-cmake issues can be
# resolved.
if(APPLE)
  find_library(Foundation Foundation)
  find_library(Metal Metal)
endif()

iree_cc_library(
  NAME
    drivers
  HDRS
    "init.h"
  SRCS
    "init.c"
  DEPS
    iree::base::api
    iree::base::tracing
    ${IREE_HAL_DRIVER_MODULES}
    # TODO: Also move as above if bazel-to-cmake issues can be resolved.
    $<$<PLATFORM_ID:Darwin>:${Foundation}>
    $<$<PLATFORM_ID:Darwin>:${Metal}>
  PUBLIC
)
