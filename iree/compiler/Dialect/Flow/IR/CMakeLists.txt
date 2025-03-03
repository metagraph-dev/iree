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

iree_add_all_subdirs()

file(GLOB _GLOB_X_TD LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS *.td)
iree_cc_library(
  NAME
    IR
  HDRS
    "FlowDialect.h"
    "FlowEnums.h.inc"
    "FlowOpInterface.h.inc"
    "FlowOpUtils.h"
    "FlowOps.h"
    "FlowOps.h.inc"
    "FlowTypes.h"
  SRCS
    "FlowDialect.cpp"
    "FlowEnums.cpp.inc"
    "FlowOpFolders.cpp"
    "FlowOpInterface.cpp.inc"
    "FlowOpUtils.cpp"
    "FlowOps.cpp"
    "FlowOps.cpp.inc"
    "FlowTypes.cpp"
  DEPS
    LLVMSupport
    MLIRIR
    MLIRParser
    MLIRSideEffectInterfaces
    MLIRStandard
    MLIRSupport
    MLIRTransformUtils
    iree::compiler::Dialect::IREE::IR
    iree::compiler::Dialect::Shape::IR
  PUBLIC
)

iree_tablegen_library(
  NAME
    FlowEnumsGen
  TD_FILE
    "FlowBase.td"
  OUTS
    -gen-enum-decls FlowEnums.h.inc
    -gen-enum-defs FlowEnums.cpp.inc
)

iree_tablegen_library(
  NAME
    FlowOpInterfaceGen
  TD_FILE
    "FlowBase.td"
  OUTS
    -gen-op-interface-decls FlowOpInterface.h.inc
    -gen-op-interface-defs FlowOpInterface.cpp.inc
)

iree_tablegen_library(
  NAME
    FlowOpsGen
  TD_FILE
    "FlowOps.td"
  OUTS
    -gen-op-decls FlowOps.h.inc
    -gen-op-defs FlowOps.cpp.inc
)

iree_tablegen_doc(
  NAME
    FlowDialectDocGen
  TD_FILE
    "FlowOps.td"
  OUTS
    -gen-dialect-doc FlowDialect.md
)
