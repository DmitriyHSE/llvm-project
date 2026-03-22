#ifndef LLVM_TRANSFORMS_UTILS_TEST_H
#define LLVM_TRANSFORMS_UTILS_TEST_H

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"

namespace llvm {

class MyInlinePass : public PassInfoMixin<MyInlinePass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_TEST_H
