#include "clang/AST/Attr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

using namespace clang;

namespace {

class MarkUnusedVisitor : public RecursiveASTVisitor<MarkUnusedVisitor> {
public:
  explicit MarkUnusedVisitor(ASTContext &Context) {}

  bool VisitVarDecl(VarDecl *VD) {
    if (VD && !VD->hasAttr<UnusedAttr>() && !VD->isUsed(false)) {
      VD->addAttr(UnusedAttr::CreateImplicit(VD->getASTContext(),
                                             VD->getLocation()));
    }
    return true;
  }
};

class MarkUnusedConsumer : public ASTConsumer {
public:
  explicit MarkUnusedConsumer(ASTContext &Context) : Visitor(Context) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  MarkUnusedVisitor Visitor;
};

class MarkUnusedAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                  llvm::StringRef) override {
    return std::make_unique<MarkUnusedConsumer>(CI.getASTContext());
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    return true;
  }

  PluginASTAction::ActionType getActionType() override {
    return AddAfterMainAction;
  }
};

} // namespace

static FrontendPluginRegistry::Add<MarkUnusedAction>
    X("mark-unused", "Mark unused variables and parameters with [[maybe_unused]].");
