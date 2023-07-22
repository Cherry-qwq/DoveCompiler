#include <fstream>
#include <sstream>
#include <string>

#include "AsmGen.h"

namespace asmgen
{


  void AsmGenerator::generate()
  {
    

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