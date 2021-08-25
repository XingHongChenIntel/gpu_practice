/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2016 - 2021 Intel Corporation. All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to the
 * source code ("Material") are owned by Intel Corporation or its suppliers
 * or licensors. Title to the Material remains with Intel Corporation or its
 * suppliers and licensors. The Material contains trade secrets and proprietary
 * and confidential information of Intel or its suppliers and licensors. The
 * Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Intel in writing.
 */

#pragma once
#include "CL/cl.h"

/******************************
  * Internal only cl_mem_flags *
  ******************************/

#define CL_MEM_TILE_MAX (3)

// clang-format off
// cl_mem_properties_intel
#define CL_MEM_DEVICE_ID_INTEL                         0x10011

#define CL_MEM_CACHE_CLOS_INTEL                        0x10012 // placeholder

// cl_context_properties
#define CL_EXTERNAL_DEVICE_HANDLE_INTEL                0x300B
#define CL_EXTERNAL_DEVICEGROUP_INTEL                  0x300C

// cl_queue_properties
#define CL_QUEUE_FAMILY_DEPRECATED_INTEL               0x10008

// cl_queue_properties values
#define CL_QUEUE_FAMILY_TYPE_RCS_DEPRECATED_INTEL      0x0u
#define CL_QUEUE_FAMILY_TYPE_CCS0_DEPRECATED_INTEL     0x1u
#define CL_QUEUE_FAMILY_TYPE_CCS1_DEPRECATED_INTEL     0x2u
#define CL_QUEUE_FAMILY_TYPE_CCS2_DEPRECATED_INTEL     0x3u
#define CL_QUEUE_FAMILY_TYPE_CCS3_DEPRECATED_INTEL     0x4u
#define CL_QUEUE_FAMILY_TYPE_BCS_DEPRECATED_INTEL      0x5u

// cl_device_info
#define CL_DEVICE_NUM_QUEUE_FAMILIES_DEPRECATED_INTEL  0x10009

// cl_mem_flags
// Ensure that values are not already used in opencl/extensions/public/cl_ext_private.h
#define CL_MEM_TILE_ONLY_MEMORY_VISIBILITY (1u << 17)
#define CL_MEM_COMPRESSED_HINT_INTEL (1u << 21)
#define CL_MEM_UNCOMPRESSED_HINT_INTEL (1u << 22)
#define CL_MEM_DUPLICATE_RESOURCE_INTEL (1u << 24)
// use high bit for PAVP protected content usage
#define CL_MEM_PROTECTED_CONTENT_INTEL (1ull << 63)

// cl_kernel_info
#define CL_KERNEL_SUPPORT_COMPRESSION                  0x1000A

// cl_command_type
#define CL_COMMAND_ACQUIRE_EXTERNAL_MEMORY             0x1000B
#define CL_COMMAND_RELEASE_EXTERNAL_MEMORY             0x1000C
///// cl_intel_pvc_rt_validation
#define CL_COMMAND_INIT_RT_DISPATCH_GLOBALS_INTEL      0x10011 // placeholder // added by PW - needs discussion

// cl_kernel_exec_info 
#define CL_KERNEL_EXEC_INFO_THREAD_ARBITRATION_POLICY_STALL_BASED_ROUND_ROBIN_INTEL 0x10026
#define CL_KERNEL_EXEC_INFO_EU_THREAD_OVER_DISPATCH_INTEL 0x10027

#pragma pack(push, 1)
typedef enum _cl_external_mem_handle_type_enum {
    CL_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_FD = 1,
    CL_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32 = 2,
    CL_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_KMT = 3,
    CL_EXTERNAL_MEMORY_HANDLE_TYPE_D3D12_HEAP = 4,
    CL_EXTERNAL_MEMORY_HANDLE_TYPE_D3D12_RESOURCE = 5,
} cl_external_mem_handle_type;

typedef enum _cl_external_mem_properties {
    CL_EXTERNAL_MEMORY_HANDLE_TYPE = 1,
    CL_EXTERNAL_MEMORY_HANDLE_SIZE = 2
} cl_external_mem_properties;

typedef struct _cl_external_mem_desc_st {
    cl_external_mem_handle_type type;
    void *handle;
    cl_external_mem_properties *props;
    unsigned long long size;
} cl_external_mem_desc;
#pragma pack(pop)

// cl_intel_variable_eu_thread_count
#define CL_DEVICE_EU_THREAD_COUNTS_INTEL 0x1000A // placeholder
#define CL_KERNEL_EU_THREAD_COUNT_INTEL  0x1000B // placeholder

// cl_device_info - returns the number of cache classes of service supported by a a device (cl_uint)
#define CL_DEVICE_NUM_CACHE_CLOS_INTEL   0x1000C // placeholder

// cl_device_info - CL_DEVICE_FEATURE_CAPABILITIES_INTEL (cl_device_feature_capabilities_intel)
#define CL_DEVICE_FEATURE_FLAG_DPAS_INTEL (1 << 1)

// clang-format on
