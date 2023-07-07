#include <iostream>
#include <antlr4-runtime.h>
#include <SysYLexer.h>
#include <SysYParser.h>

int main(int argc, char *argv[])
{
	config::init(argc, argv);
	std::cout << config::get() << std::endl;

	auto source = std::ifstream();
	source.open(config::get().srcFilePath);
	antlr4::ANTLRInputStream input(source);
	CodeParser::SysYLexer mLexer(&input);
	antlr4::CommonTokenStream tokens(&mLexer);
	CodeParser::SysYParser mParser(&tokens);
	mParser.setErrorHandler(std::make_shared<antlr4::BailErrorStrategy>());
	CodeParser::SysYParser::CompUnitContext *root = mParser.compUnit();

	//	frontend : ast => ir
	auto Visitor = CodeParser::VisitorImpl();
	// auto r = std::any_cast<std::shared_ptr<mir::CompUnitSsa>>(root->accept(&astVisitor));
	// auto dh = mir::DumpHelper();
	// println(r->dump(dh));

	std::cout << "Bye" << std::endl;
	return 0;
}