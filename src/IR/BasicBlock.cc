
#include "BasicBlock.h"
#include "ControlFlow.h"
namespace ir{
  std::shared_ptr<JPLabel> BasicBlock::getJPLabel(){
    return jplabel_;
  };
}