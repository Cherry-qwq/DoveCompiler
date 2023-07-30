
#include "BasicBlock.h"
#include "ControlFlow.h"
namespace ir{
  std::shared_ptr<JPLabel> BasicBlock::getLabel(){
    return jplabel_;
  };
}