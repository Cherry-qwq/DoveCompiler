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
      output_ = std::stringstream();
      output_<<"DumpHelper initialized\n"<<std::endl;
    };
    
    std::string dump(){
      return output_.str();
    };
    
    void indent(){
      indent_++;
    };

    void unindent(){
      indent_--;
    };

    void add(std::string str){
      for(int i=0;i<indent_;i++){
        output_<<"\t";
      }
      output_<<str<<std::endl;
    };
    void sub(std::string str){
      for(int i=0;i<indent_;i++){
        output_<<"\t";
      }
      output_<<str<<std::endl;
    };
    void mul(std::string str){
      for(int i=0;i<indent_;i++){
        output_<<"\t";
      }
      output_<<str<<std::endl;
    };    
    void fsub(std::string str){
      for(int i=0;i<indent_;i++){
        output_<<"\t";
      }
      output_<<str<<std::endl;
    };
    void fmul(std::string str){
      for(int i=0;i<indent_;i++){
        output_<<"\t";
      }
      output_<<str<<std::endl;
    };
    void urem(std::string str){
      for(int i=0;i<indent_;i++){
        output_<<"\t";
      }
      output_<<str<<std::endl;
    }; 
    void frem(std::string str){
      for(int i=0;i<indent_;i++){
        output_<<"\t";
      }
      output_<<str<<std::endl;
    }; 
    void srem(std::string str){
      for(int i=0;i<indent_;i++){
        output_<<"\t";
      }
      output_<<str<<std::endl;
    };     
  protected:
    std::stringstream output_;
    int indent_ = 0;
  };
}