// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef IREE_HAL_VULKAN_ENUMLATED_SEMAPHORE_H_
#define IREE_HAL_VULKAN_ENUMLATED_SEMAPHORE_H_

#include "iree/hal/api.h"
#include "iree/hal/vulkan/command_queue.h"
#include "iree/hal/vulkan/handle_util.h"
#include "iree/hal/vulkan/timepoint_util.h"

iree_status_t iree_hal_vulkan_emulated_semaphore_create(
    iree::hal::vulkan::VkDeviceHandle* logical_device,
    iree::hal::vulkan::TimePointSemaphorePool* semaphore_pool,
    iree_host_size_t command_queue_count,
    iree::hal::vulkan::CommandQueue** command_queues, uint64_t initial_value,
    iree_hal_semaphore_t** out_semaphore);

// Acquires a binary semaphore for waiting on the timeline to advance to the
// given |value|. The semaphore returned won't be waited by anyone else.
// |wait_fence| is the fence associated with the queue submission that waiting
// on this semaphore.
//
// Returns VK_NULL_HANDLE if there are no available semaphores for the given
// |value|.
iree_status_t iree_hal_vulkan_emulated_semaphore_acquire_wait_handle(
    iree_hal_semaphore_t* semaphore, uint64_t value,
    const iree::ref_ptr<iree::hal::vulkan::TimePointFence>& wait_fence,
    VkSemaphore* out_handle);

// Cancels the waiting attempt on the given binary |semaphore|. This allows
// the |semaphore| to be waited by others.
iree_status_t iree_hal_vulkan_emulated_semaphore_cancel_wait_handle(
    iree_hal_semaphore_t* semaphore, VkSemaphore handle);

// Acquires a binary semaphore for signaling the timeline to the given |value|.
// |value| must be smaller than the current timeline value. |signal_fence| is
// the fence associated with the queue submission that signals this semaphore.
iree_status_t iree_hal_vulkan_emulated_semaphore_acquire_signal_handle(
    iree_hal_semaphore_t* semaphore, uint64_t value,
    const iree::ref_ptr<iree::hal::vulkan::TimePointFence>& signal_fence,
    VkSemaphore* out_handle);

iree_status_t iree_hal_vulkan_emulated_semaphore_multi_wait(
    iree::hal::vulkan::VkDeviceHandle* logical_device,
    const iree_hal_semaphore_list_t* semaphore_list, iree_time_t deadline_ns,
    VkSemaphoreWaitFlags wait_flags);

#endif  // IREE_HAL_VULKAN_ENUMLATED_SEMAPHORE_H_
