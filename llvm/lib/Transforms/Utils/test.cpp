#include "llvm/Transforms/Utils/test.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Debug.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/DenseMap.h"

using namespace llvm;

#define DEBUG_TYPE "my-inline"

static bool isInlineableFunction(Function &F) {
  return F.getReturnType()->isVoidTy() && F.arg_empty();
}

PreservedAnalyses MyInlinePass::run(Function &F, FunctionAnalysisManager &AM) {
  bool Changed = false;
  SmallVector<CallInst*, 8> CallsToInline;

  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      if (auto *CI = dyn_cast<CallInst>(&I)) {
        Function *Callee = CI->getCalledFunction();
        if (Callee && isInlineableFunction(*Callee) && !Callee->isDeclaration()) {
          CallsToInline.push_back(CI);
        }
      }
    }
  }

  for (CallInst *CI : CallsToInline) {
    Function *Callee = CI->getCalledFunction();
    if (!Callee) continue;

    Instruction *InsertPos = CI;

    DenseMap<const Value*, Value*> VMap;

    for (BasicBlock &CalleeBB : *Callee) {
      for (Instruction &I : CalleeBB) {
        if (isa<ReturnInst>(I)) continue;

        Instruction *NewInst = I.clone();

        for (unsigned i = 0; i < NewInst->getNumOperands(); ++i) {
          Value *Operand = NewInst->getOperand(i);
          if (VMap.count(Operand)) {
            NewInst->setOperand(i, VMap[Operand]);
          }
        }

        NewInst->insertBefore(InsertPos);

        VMap[&I] = NewInst;
      }
    }

    CI->eraseFromParent();
    Changed = true;
  }

  if (Changed) {
    verifyFunction(F);
    return PreservedAnalyses::none();
  }
  return PreservedAnalyses::all();
}
