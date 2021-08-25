/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2019 - 2020 Intel Corporation. All Rights Reserved.
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

#ifndef _ZEX_CMDLIST_H
#define _ZEX_CMDLIST_H
#if defined(__cplusplus)
#pragma once
#endif
#if !defined(_ZEX_API_H)
#pragma message("warning: this file is not intended to be included directly")
#endif
#include "zex_common.h"

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
/// @brief Reserve a section of contiguous command buffer space within the
///        command list.
///
/// @details
///     - The pointer returned is valid for both Host and device access.
///     - The application may **not** call this function from simultaneous
///       threads with the same command list handle.
///     - The implementation of this function should be lock-free.
///
/// @returns
///     - ::ZE_RESULT_SUCCESS
///     - ::ZE_RESULT_ERROR_UNINITIALIZED
///     - ::ZE_RESULT_ERROR_DEVICE_LOST
///     - ::ZE_RESULT_ERROR_INVALID_ARGUMENT
///         + nullptr == hCommandList
///         + nullptr == ptr
///         + 0 for size
///     - ::ZE_RESULT_ERROR_UNKNOWN
ZE_DLLEXPORT ze_result_t ZE_APICALL
zexCommandListReserveSpace(
    zex_command_list_handle_t hCommandList, ///< [in] handle of the command list
    size_t size,                            ///< [in] size (in bytes) to reserve
    void **ptr                              ///< [out] pointer to command buffer space reserved
);
ZE_DLLEXPORT ze_result_t ZE_APICALL
zexCommandListAppendMILoadRegReg(
    zex_command_list_handle_t hCommandList,
    ALU_REG destination,
    ALU_REG source);
ZE_DLLEXPORT ze_result_t ZE_APICALL
zexCommandListAppendMILoadRegMem(
    zex_command_list_handle_t hCommandList,
    ALU_REG reg,
    uint64_t address);
ZE_DLLEXPORT ze_result_t ZE_APICALL
zexCommandListAppendMILoadRegImm(
    zex_command_list_handle_t hCommandList,
    ALU_REG destination,
    uint32_t val);
ZE_DLLEXPORT ze_result_t ZE_APICALL
zexCommandListAppendMIStoreRegMem(
    zex_command_list_handle_t hCommandList,
    ALU_REG reg,
    uint64_t address);
ZE_DLLEXPORT ze_result_t ZE_APICALL
zexCommandListAppendMIMath(
    zex_command_list_handle_t hCommandList,
    zex_alu_operation_t *opArray,
    size_t noOperations);
ZE_DLLEXPORT ze_result_t ZE_APICALL
zexCommandListAppendPipeControl(
    zex_command_list_handle_t hCommandList,
    void *dstptr,
    uint64_t value);
#if defined(__cplusplus)
} // extern "C"
#endif

#endif // _ZEX_CMDLIST_H
