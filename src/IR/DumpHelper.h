#pragma once

#include <string>
#include <memory>
#include <map>
#include <sstream>

#include "Value.h"


namespace ir
{
  
  class DumpHelper
  {
  public:
    DumpHelper(){
      output_<<"DumpHelper initialized\n"<<std::endl;
    };
    std::string dump(){
      return output_.str();
    };
    
  protected:
    std::stringstream output_;
  };
}