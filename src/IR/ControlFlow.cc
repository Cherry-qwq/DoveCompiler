
#include "BasicBlock.h"

namespace ir
{

  std::string Function::dump(DumpHelper &helper) const
  {
    std::string output = "Function " + getName() + ": " + getType()->dump();
    helper.add(output);
    helper.indent();
    for (auto basic_block : basic_blocks_)
    {
      basic_block->dump(helper);
    }
    helper.unindent();
    return output;
  }
  void Function::addBasicBlock(std::shared_ptr<BasicBlock> basic_block)
  {
    basic_blocks_.push_back(basic_block);
  }
  std::shared_ptr<std::vector<std::shared_ptr<BasicBlock>>> Function::getBasicBlocks()
  {
    return std::make_shared<std::vector<std::shared_ptr<BasicBlock>>>(basic_blocks_);
  }

}