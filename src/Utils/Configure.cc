#include "Configure.h"
#include "CONSTANTS.h"
#include <memory>
#include <argparse.h>

namespace config
{

	std::unique_ptr<Config> up;
	Config &init(int argc, char **argv)
	{
		up = std::make_unique<Config>();
		auto arg_parser = argparse::ArgumentParser(CONSTANTS::kProgramName);
		arg_parser.add_argument("srcFilePath")
				.help("Source File.")
				.required();
		arg_parser.add_argument("-o")
				.help("Compiled File Destination.")
				.required();

		try
		{
			arg_parser.parse_args(argc, argv);
			up->src_file_path = arg_parser.get("srcFilePath");
			up->dst_file_path = arg_parser.get("-o");
		}
		catch (const std::runtime_error &err)
		{
			std::cerr << err.what() << std::endl;
			std::cerr << arg_parser << std::endl;
			std::exit(1);
		}
		return *up;
	}
	Config &get()
	{
		return *up;
	}
}