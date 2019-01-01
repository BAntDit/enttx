//
// Created by bantdit on 1/1/19.
//

#include <boost/program_options.hpp>
#include "options.h"

namespace po = boost::program_options;

Options::Options(int argc, const char **argv)
{
    po::options_description commandLineOptions{ u8"commandLineOptions" };

    commandLineOptions.add_options()(u8"help, h", u8"show help");

    po::variables_map variables;

    if (argc > 0) {
        po::store(po::parse_command_line(argc, argv, commandLineOptions), variables);
    }

    po::notify(variables);
}
