#include "clang/AST/Attr.h"
#include "clang/AST/Decl.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/ASTConsumer.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

namespace {

class MarkUnusedConsumer : public ASTConsumer {
public:
    bool HandleTopLevelDecl(DeclGroupRef DG) override {
    	for (Decl *D : DG) {
            if (VarDecl *VD = dyn_cast<VarDecl>(D)) {
                if (!VD->hasAttr<UnusedAttr>() && !VD->isUsed(false)) {
                    VD->addAttr(UnusedAttr::CreateImplicit(VD->getASTContext(),
                                                           VD->getLocation()));
                    VD->setIsUsed();
                }
            }
        }
        return true;
    }
};

class MarkUnusedAction : public PluginASTAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                    llvm::StringRef) override {
        return std::make_unique<MarkUnusedConsumer>();
    }

    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override {
        return true;
    }

    PluginASTAction::ActionType getActionType() override {
        return AddBeforeMainAction;
    }
};

} // namespace

static FrontendPluginRegistry::Add<MarkUnusedAction>
    X("mark-unused", "Mark unused variables and parameters with [[maybe_unused]].");