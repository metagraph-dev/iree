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

#include "iree/compiler/Dialect/VM/IR/VMOps.h"
#include "iree/compiler/Dialect/VM/Transforms/Passes.h"
#include "mlir/IR/Attributes.h"
#include "mlir/IR/SymbolTable.h"
#include "mlir/Pass/Pass.h"

namespace mlir {
namespace iree_compiler {
namespace IREE {
namespace VM {

class MarkPublicSymbolsExportedPass
    : public PassWrapper<MarkPublicSymbolsExportedPass,
                         OperationPass<mlir::ModuleOp>> {
 public:
  void runOnOperation() override {
    for (auto funcOp : getOperation().getOps<mlir::FuncOp>()) {
      if (funcOp.isPublic()) {
        funcOp->setAttr("iree.module.export", UnitAttr::get(&getContext()));
      }
    }
  }
};

std::unique_ptr<OperationPass<mlir::ModuleOp>>
createMarkPublicSymbolsExportedPass() {
  return std::make_unique<MarkPublicSymbolsExportedPass>();
}

static PassRegistration<MarkPublicSymbolsExportedPass> pass(
    "iree-vm-mark-public-symbols-exported",
    "Sets public visiblity symbols to have the iree.module.export attribute");

}  // namespace VM
}  // namespace IREE
}  // namespace iree_compiler
}  // namespace mlir
