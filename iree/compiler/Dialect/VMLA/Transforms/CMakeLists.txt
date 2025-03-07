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

iree_add_all_subdirs()

iree_cc_library(
  NAME
    Transforms
  HDRS
    "Passes.h"
  SRCS
    "Conversion.cpp"
    "Passes.cpp"
    "PreConversionLowering.cpp"
    "UnrollReductions.cpp"
  DEPS
    LLVMSupport
    MLIRIR
    MLIRPass
    MLIRStandard
    MLIRSupport
    MLIRTransforms
    iree::compiler::Dialect::HAL::IR::HALDialect
    iree::compiler::Dialect::IREE::Transforms
    iree::compiler::Dialect::Shape::IR
    iree::compiler::Dialect::Shape::Transforms
    iree::compiler::Dialect::VMLA::Conversion
    iree::compiler::Dialect::VMLA::Conversion::HALToVMLA
    iree::compiler::Dialect::VMLA::Conversion::HLOToVMLA
    iree::compiler::Dialect::VMLA::Conversion::StandardToVMLA
    iree::compiler::Dialect::VMLA::IR
    iree::compiler::Dialect::VMLA::IR::VMLADialect
    tensorflow::mlir_hlo
  PUBLIC
)
