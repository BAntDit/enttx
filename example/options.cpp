//
// Created by bantdit on 1/1/19.
//

#include <boost/program_options.hpp>
#include "options.h"

namespace po = boost::program_options;

Options::Options(int argc, const char **argv) :
    initialEntitiesCount_{ 0 }
{
    po::options_description commandLineOptions{ u8"commandLineOptions" };

    commandLineOptions.add_options()
        (u8"help, h", u8"show help")
        (u8"init-entities-count, i", po::value<size_t>()->default_value(1000), "initial entities count");

    po::variables_map variables;

    if (argc > 0) {
        po::store(po::parse_command_line(argc, argv, commandLineOptions), variables);
    }

    if (variables.count("init-entities-count")) {
        initialEntitiesCount_ = variables["init-entities-count"].as<size_t>();
    }

    po::notify(variables);
}
