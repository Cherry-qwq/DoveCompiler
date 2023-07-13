#include <iostream>
#include <antlr4-runtime.h>
#include <SysYLexer.h>
#include <SysYParser.h>

#include "Utils/Configure.h"
#include "CodeParser/VisitorImpl.h"
int main(int argc, char *argv[])
{
	config::init(argc, argv);
	std::cout << config::get() << std::endl;

	auto source = std::ifstream();
	source.open(config::get().src_file_path);
	antlr4::ANTLRInputStream input(source);
	front::SysYLexer m_lexer(&input);
	antlr4::CommonTokenStream tokens(&m_lexer);
	front::SysYParser m_parser(&tokens);
	m_parser.setErrorHandler(std::make_shared<antlr4::BailErrorStrategy>());
	front::SysYParser::CompUnitContext *root = m_parser.compUnit();

	//	front : ast => ir
	auto visitor = front::VisitorImpl();
	auto irp = std::any_cast<std::shared_ptr<ir::CompUnit>>(root->accept(&visitor));
	auto dh = ir::DumpHelper();
	std::cout<<irp->dump(dh);

	std::cout << "Bye" << std::endl;
	return 0;
}