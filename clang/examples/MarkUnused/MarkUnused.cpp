#include "clang/AST/Attr.h"
#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/ASTConsumer.h"

using namespace clang;

namespace {

class MarkUnusedVisitor : public RecursiveASTVisitor<MarkUnusedVisitor> {
public:
    bool VisitVarDecl(VarDecl *VD) {
        if (!VD->hasAttr<UnusedAttr>() && !VD->isUsed(false)) {
            VD->addAttr(UnusedAttr::CreateImplicit(VD->getASTContext(),
                                                   VD->getLocation()));
            VD->setIsUsed();
        }
        return true;
    }
};

class MarkUnusedConsumer : public ASTConsumer {
public:
    void HandleTranslationUnit(ASTContext &Context) override {
        MarkUnusedVisitor Visitor;
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
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