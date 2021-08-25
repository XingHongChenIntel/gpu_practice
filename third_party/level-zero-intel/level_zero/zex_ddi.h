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

#ifndef _ZEX_DDI_H
#define _ZEX_DDI_H
#if defined(__cplusplus)
#pragma once
#endif
#include "zex_api.h"

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for zexInit
typedef ze_result_t(ZE_APICALL *zex_pfnInit_t)(
    ze_init_flag_t);

///////////////////////////////////////////////////////////////////////////////
/// @brief Table of Global functions pointers
typedef struct _zex_global_dditable_t {
    zex_pfnInit_t pfnInit;
} zex_global_dditable_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Exported function for filling application's Global table
///        with current process' addresses
///
/// @returns
///     - ::ZE_RESULT_SUCCESS
///     - ::ZE_RESULT_ERROR_INVALID_ARGUMENT
///         + invalid value for version
///         + nullptr for pDdiTable
///     - ::ZE_RESULT_ERROR_UNKNOWN
///         + version not supported
ZE_DLLEXPORT ze_result_t ZE_APICALL
zexGetGlobalProcAddrTable(
    ze_api_version_t version,        ///< [in] API version requested
    zex_global_dditable_t *pDdiTable ///< [in,out] pointer to table of DDI function pointers
);

///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for zexGetGlobalProcAddrTable
typedef ze_result_t(ZE_APICALL *zex_pfnGetGlobalProcAddrTable_t)(
    ze_api_version_t,
    zex_global_dditable_t *);

///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for zexCommandListReserveSpace
typedef ze_result_t(ZE_APICALL *zex_pfnCommandListReserveSpace_t)(
    zex_command_list_handle_t,
    size_t,
    void **);

typedef ze_result_t(ZE_APICALL *zex_pfnCommandListAppendMILoadRegReg_t)(
    zex_command_list_handle_t hCommandList,
    ALU_REG destination,
    ALU_REG source);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandListAppendMILoadRegMem_t)(
    zex_command_list_handle_t hCommandList,
    ALU_REG reg,
    uint64_t address);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandListAppendMILoadRegImm_t)(
    zex_command_list_handle_t hCommandList,
    ALU_REG destination,
    uint32_t val);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandListAppendMIStoreRegMem_t)(
    zex_command_list_handle_t hCommandList,
    ALU_REG reg,
    uint64_t address);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandListAppendMIMath_t)(
    zex_command_list_handle_t hCommandList,
    zex_alu_operation_t *opArray,
    size_t noOperations);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandListAppendPipeControl_t)(
    zex_command_list_handle_t hCommandList,
    void *dstptr,
    uint64_t value);

///////////////////////////////////////////////////////////////////////////////
/// @brief Table of CommandList functions pointers
typedef struct _zex_command_list_dditable_t {
    zex_pfnCommandListReserveSpace_t pfnReserveSpace;
    zex_pfnCommandListAppendMILoadRegReg_t pfnAppendMILoadRegReg;
    zex_pfnCommandListAppendMILoadRegMem_t pfnAppendMILoadRegMem;
    zex_pfnCommandListAppendMILoadRegImm_t pfnAppendMILoadRegImm;
    zex_pfnCommandListAppendMIStoreRegMem_t pfnAppendMIStoreRegMem;
    zex_pfnCommandListAppendMIMath_t pfnAppendMIMath;
    zex_pfnCommandListAppendPipeControl_t pfnAppendPipeControl;
} zex_command_list_dditable_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Exported function for filling application's CommandList table
///        with current process' addresses
///
/// @returns
///     - ::ZE_RESULT_SUCCESS
///     - ::ZE_RESULT_ERROR_INVALID_ARGUMENT
///         + invalid value for version
///         + nullptr for pDdiTable
///     - ::ZE_RESULT_ERROR_UNKNOWN
///         + version not supported
ZE_DLLEXPORT ze_result_t ZE_APICALL
zexGetCommandListProcAddrTable(
    ze_api_version_t version,              ///< [in] API version requested
    zex_command_list_dditable_t *pDdiTable ///< [in,out] pointer to table of DDI function pointers
);

///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for zexGetCommandListProcAddrTable
typedef ze_result_t(ZE_APICALL *zex_pfnGetCommandListProcAddrTable_t)(
    ze_api_version_t,
    zex_command_list_dditable_t *);

typedef ze_result_t(ZE_APICALL *zex_pfnCommandGraphCreate_t)(
    ze_device_handle_t hDevice,                ///< [in] handle of the device object
    const ze_command_list_desc_t *desc,        ///< [in] pointer to command list descriptor
    zex_command_graph_handle_t *phCommandGraph ///< [out] pointer to handle of command graph created
);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandGraphDestroy_t)(
    zex_command_graph_handle_t hCommandGraph);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandGraphClose_t)(
    zex_command_graph_handle_t hCommandGraph);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandGraphCreateNode_t)(
    zex_command_graph_handle_t hCommandGraph,
    zex_command_graph_handle_t *phCommandNode,
    zex_command_graph_handle_t *phParentNodes,
    size_t noParentNodes,
    COMMANDGRAPH_TYPE nodeType);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandGraphCreateLoadRegImemNode_t)(
    zex_command_graph_handle_t hCommandGraph,
    zex_command_graph_handle_t *phCommandNode,
    zex_command_graph_handle_t *phParentNodes,
    size_t noParentNodes,
    ALU_REG val,
    ALU_REG address);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandGraphCreateStoreRegImemNode_t)(
    zex_command_graph_handle_t hCommandGraph,
    zex_command_graph_handle_t *phCommandNode,
    zex_command_graph_handle_t *phParentNodes,
    size_t noParentNodes,
    ALU_REG val,
    ALU_REG address);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandGraphNodeAddChildren_t)(
    zex_command_graph_handle_t hCommandNode,
    zex_command_graph_handle_t *phChildrenNodes,
    size_t noChildrenNodes);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandGraphOpenNode_t)(
    zex_command_graph_handle_t hCommandNode,
    zex_command_list_handle_t *phCommandList);
typedef ze_result_t(ZE_APICALL *zex_pfnCommandGraphCloseNode_t)(
    zex_command_graph_handle_t hCommandNode);

///////////////////////////////////////////////////////////////////////////////
/// @brief Table of CommandGraph functions pointers
typedef struct _zex_command_graph_dditable_t {
    zex_pfnCommandGraphCreate_t pfnCreate;
    zex_pfnCommandGraphDestroy_t pfnDestroy;
    zex_pfnCommandGraphClose_t pfnClose;
    zex_pfnCommandGraphCreateNode_t pfnCreateNode;
    zex_pfnCommandGraphCreateLoadRegImemNode_t pfnCreateLoadRegImemNode;
    zex_pfnCommandGraphCreateStoreRegImemNode_t pfnCreateStoreRegImemNode;
    zex_pfnCommandGraphNodeAddChildren_t pfnNodeAddChildren;
    zex_pfnCommandGraphOpenNode_t pfnOpenNode;
    zex_pfnCommandGraphCloseNode_t pfnCloseNode;
} zex_command_graph_dditable_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Exported function for filling application's CommandGraph table
///        with current process' addresses
///
/// @returns
///     - ::ZE_RESULT_SUCCESS
///     - ::ZE_RESULT_ERROR_INVALID_ARGUMENT
///         + invalid value for version
///         + nullptr for pDdiTable
///     - ::ZE_RESULT_ERROR_UNKNOWN
///         + version not supported
ZE_DLLEXPORT ze_result_t ZE_APICALL
zexGetCommandGraphProcAddrTable(
    ze_api_version_t version,               ///< [in] API version requested
    zex_command_graph_dditable_t *pDdiTable ///< [in,out] pointer to table of DDI function pointers
);

///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for zexGetCommandGraphProcAddrTable
typedef ze_result_t(ZE_APICALL *zex_pfnGetCommandGraphProcAddrTable_t)(
    ze_api_version_t,
    zex_command_graph_dditable_t *);

typedef ze_result_t(ZE_APICALL *zex_pfnCommandQueueExecuteCommandGraphs_t)(
    ze_command_queue_handle_t hCommandQueue,
    uint32_t numCommandGraphs,
    zex_command_graph_handle_t *phCommandGraphs,
    ze_fence_handle_t hFence);

typedef struct _zex_command_queue_dditable_t {
    zex_pfnCommandQueueExecuteCommandGraphs_t pfnExecuteCommandGraphs;
} zex_command_queue_dditable_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Exported function for filling application's CommandQueue table
///        with current process' addresses
///
/// @returns
///     - ::ZE_RESULT_SUCCESS
///     - ::ZE_RESULT_ERROR_INVALID_ARGUMENT
///         + invalid value for version
///         + nullptr for pDdiTable
///     - ::ZE_RESULT_ERROR_UNKNOWN
///         + version not supported
ZE_DLLEXPORT ze_result_t ZE_APICALL
zexGetCommandQueueProcAddrTable(
    ze_api_version_t version,               ///< [in] API version requested
    zex_command_queue_dditable_t *pDdiTable ///< [in,out] pointer to table of DDI function pointers
);

///////////////////////////////////////////////////////////////////////////////
/// @brief Function-pointer for zexGetCommandQueueProcAddrTable
typedef ze_result_t(ZE_APICALL *zex_pfnGetCommandQueueProcAddrTable_t)(
    ze_api_version_t,
    zex_command_queue_dditable_t *);

///////////////////////////////////////////////////////////////////////////////
/// @brief Container for all DDI tables
typedef struct _zex_dditable_t {
    zex_global_dditable_t global;
    zex_command_list_dditable_t commandList;
    zex_command_graph_dditable_t commandGraph;
    zex_command_queue_dditable_t commandQueue;
} zex_dditable_t;

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // _ZEX_DDI_H
