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

#ifndef _ZEX_COMMON_H
#define _ZEX_COMMON_H
#if defined(__cplusplus)
#pragma once
#endif
#include <level_zero/ze_api.h>

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
/// @brief Handle of command list object
typedef ze_command_list_handle_t zex_command_list_handle_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Handle of driver's command graph object
typedef struct _zex_command_graph_handle_t *zex_command_graph_handle_t;

///////////////////////////////////////////////////////////////////////////////
/// @brief Forward-declare zex_command_graph_desc_t
typedef struct _ze_command_list_desc_t zex_command_graph_desc_t;

enum class ALU_REG : uint32_t {
    GPR0 = 0,
    GPR0_1 = 1,
    GPR1 = 2,
    GPR1_1 = 3,
    GPR2 = 4,
    GPR2_1 = 5,
    GPR3 = 6,
    GPR3_1 = 7,
    GPR4 = 8,
    GPR4_1 = 9,
    GPR5 = 10,
    GPR5_1 = 11,
    GPR6 = 12,
    GPR6_1 = 13,
    GPR7 = 14,
    GPR7_1 = 15,
    GPR8 = 16,
    GPR8_1 = 17,
    GPR9 = 18,
    GPR9_1 = 19,
    GPR10 = 20,
    GPR10_1 = 21,
    GPR11 = 22,
    GPR11_1 = 23,
    GPR12 = 24,
    GPR12_1 = 25,
    GPR13 = 26,
    GPR13_1 = 27,
    GPR14 = 28,
    GPR14_1 = 29,
    GPR15 = 30,
    GPR15_1 = 31,
    GPR_MAX = 32,
    PREDICATE1 = 33,
    REG_MAX = 34,
    CONST0 = 35,
    CONST1 = 36,
    NONE = 37,
    MAX = 38
};
enum class ALU_OP_TYPE : uint32_t {
    ADD = 0x100,
    SUB = 0x101,
    AND = 0x102,
    OR = 0x103,
    XOR = 0x104,
    STORE = 0x180,
    STOREINV = 0x580,
    LOAD = 0x080,
    LOAD0 = 0x081,
    LOAD1 = 0x481,
    LOADINV = 0x480,
    NOOP = 0x0,
    MAX
};
enum class ALU_FLAG : uint32_t {
    ACC = 0x31,
    ZF = 0x32,
    CF = 0x33,
    NACC = ACC + 0x100,
    NZF = ZF + 0x100,
    NCF = CF + 0x100,
    MAX
};
typedef struct _zex_alu_operation_t {
    ALU_OP_TYPE opType;
    ALU_REG regDest;
    ALU_REG regSource1;
    ALU_REG regSource2;
    ALU_FLAG flag;
} zex_alu_operation_t;
enum class COMMANDGRAPH_TYPE : uint32_t {
    COMMAND,
    CONDITION,
    OR,
    START,
    MAX
};

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // _ZEX_COMMON_H
