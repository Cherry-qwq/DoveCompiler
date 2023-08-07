#include <fstream>
#include <sstream>
#include <string>

#include "AsmGen.h"
#include "IR/BasicBlock.h"
#include "IR/ControlFlow.h"
namespace asmgen
{

    void AsmGenerator::generate()
    {
        genCompUnit(comp_unit_);
    }

    void AsmGenerator::genCompUnit(std::shared_ptr<ir::CompUnit> node)
    {
        auto global_objects = node->getGlobalObjects();
        for(auto global_object : *global_objects)
        {
            if(global_object.getValue()->getValueType() == ir::ValueType::Function)
            {
                auto function = std::dynamic_pointer_cast<ir::Function>(global_object.getValue());
                s_ << function->getName() << ":" << std::endl;
                auto basic_blocks = function->getBasicBlocks();
                for(auto basic_block : *basic_blocks)
                {
                    s_ << basic_block->getName() << ":" << std::endl;
                    auto instructions = basic_block->getInstructions();
                }
            }else if(global_object.getValue()->getValueType() == ir::ValueType::Allocate){

            }
        }
    }

    std::string AsmGenerator::exportToString()
    {
        return s_.str();
    }

    void AsmGenerator::exportToFile(std::string path)
    {
        std::ofstream outfile;
        outfile.open("a.s", std::ios::out | std::ios::trunc);
        outfile.close();
    }
}