/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2019 - 2021 Intel Corporation. All Rights Reserved.
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

#ifndef _ZEX_DRIVER_H
#define _ZEX_DRIVER_H
#if defined(__cplusplus)
#pragma once
#endif
#if !defined(_ZEX_API_H)
#pragma message("warning: this file is not intended to be included directly")
#endif

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
/// @brief Initialize the 'One API' driver and must be called before any other
///        API function.
///
/// @details
///     - If this function is not called then all other functions will return
///       ::ZE_RESULT_ERROR_UNINITIALIZED.
///     - Only one instance of a driver per process will be initialized.
///     - This function is thread-safe for scenarios where multiple libraries
///       may initialize the driver simultaneously.
///
/// @remarks
///   _Analogues_
///     - **cuInit**
///
/// @returns
///     - ::ZE_RESULT_SUCCESS
///     - ::ZE_RESULT_ERROR_UNINITIALIZED
///     - ::ZE_RESULT_ERROR_DEVICE_LOST
///     - ::ZE_RESULT_ERROR_INVALID_ARGUMENT
///         + invalid value for flags
///     - ::ZE_RESULT_ERROR_UNKNOWN
///     - ::ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY
ZE_DLLEXPORT ze_result_t ZE_APICALL
zexInit(
    ze_init_flag_t flags ///< [in] initialization flags
);

ZE_DLLEXPORT ze_result_t ZE_APICALL
zexDriverImportExternalPointer(
    ze_driver_handle_t hDriver, ///< [in] handle of the driver
    void *ptr,                  ///< [in] pointer to be imported to the driver
    size_t size                 ///< [in] size to be imported
);

ZE_DLLEXPORT ze_result_t ZE_APICALL
zexDriverReleaseImportedPointer(
    ze_driver_handle_t hDriver, ///< [in] handle of the driver
    void *ptr                   ///< [in] pointer to be released from the driver
);

ZE_DLLEXPORT ze_result_t ZE_APICALL
zexDriverGetHostPointerBaseAddress(
    ze_driver_handle_t hDriver, ///< [in] handle of the driver
    void *ptr,                  ///< [in] pointer to be checked if imported to the driver
    void **baseAddress          ///< [out] if not null, returns address of the base pointer of the imported pointer
);

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // _ZEX_DRIVER_H
