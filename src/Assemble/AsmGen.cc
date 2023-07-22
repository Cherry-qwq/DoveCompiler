#include <fstream>
#include <sstream>
#include <string>

#include "AsmGen.h"

namespace asmgen
{
  

  std::string AsmGenerator::generate()
  {
    

    return s_.str();
  }

  void AsmGenerator::exportToFile(std::string path)
  {
    std::ofstream outfile;
    outfile.open("file.dat", std::ios::out | std::ios::trunc);
    outfile.close();
  }
}