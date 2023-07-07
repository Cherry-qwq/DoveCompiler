#include "configure.h"
#include "CONSTANTS.h"
#include <memory>
#include <argparse.h>

namespace config
{

	std::unique_ptr<Config> up;
	Config &init(int argc, char **argv)
	{
		up = std::make_unique<Config>();
		auto argParser = argparse::ArgumentParser program(CONSTANTS::program_name);
		argParser.add_argument("srcFilePath")
				.help("Source File.")
				.required();
		argParser.add_argument("-o")
				.help("Compiled File Destination.")
				.required();

		try
		{
			argParser.parse_args(argc, argv);
			up->srcFilePath = argParser.get("srcFilePath");
			up->dstFilePath = argParser.get("-o");
		}
		catch (const std::runtime_error &err)
		{
			std::cerr << err.what() << std::endl;
			std::cerr << argParser << std::endl;
			std::exit(1);
		}
		return *up;
	}
	Config &get()
	{
		return *up;
	}
}