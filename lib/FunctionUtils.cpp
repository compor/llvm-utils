//
//
//

#include "FunctionUtils.hpp"

#include "llvm/IR/Function.h"
// using llvm::Function

#include "llvm/IR/Module.h"
// using llvm::Module

#include "llvm/IR/Instructions.h"
// using llvm::AllocaInst
// using llvm::LoadInst
// using llvm::StoreInst

namespace commoditas {

void DemoteFunctionArgs(llvm::Function &F) {
  auto as = F.getParent()->getDataLayout().getAllocaAddrSpace();

  for (auto &Arg : F.args()) {
    auto *FirstI = F.begin()->getFirstNonPHIOrDbgOrLifetime();

    auto *ArgTy = Arg.getType();
    auto *AI = new llvm::AllocaInst(ArgTy, as, "farg");
    auto *LI = new llvm::LoadInst(AI, "load_farg");
    auto *SI = new llvm::StoreInst(&Arg, AI);

    LI->insertAfter(FirstI);
    SI->insertAfter(FirstI);
    AI->insertAfter(FirstI);

    Arg.replaceAllUsesWith(LI);
    // restore the store instruction use after replacing
    SI->setOperand(0, &Arg);
  }

  return;
}

} // namespace commoditas
