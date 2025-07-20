#include <getopt.h>

#include <boost/program_options.hpp>
#include <iostream>

#include "parser/parser.h"
#include "state/machine.h"
#include "utils.h"

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    Parser p;
    Machine m;

    po::options_description desc("A simple regex engine written in cpp.");

    desc.add_options()("help,h", "Prints these lines.")("pattern,p", "Your pattern here.")(
        "target,t", "Your target here.")("file,f", "input file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    } else if (vm.count("target") == 1 && vm.count("pattern") == 1) {
        Str pattern = vm["pattern"].as<Str>();
        Str target  = vm["target"].as<Str>();

        aqueue parsed = p.parse(pattern);
        svect res     = m.marche(&parsed, target);

        std::cout << "Matched Strings" << std::endl;
        for (auto matched : res) {
            std::cout << matched << std::endl;
        }

    } else if (vm.count("file") == 1 && vm.count("pattern") == 1) {
        Str pattern = vm["pattern"].as<Str>();
        Str target  = vm["file"].as<Str>();

        aqueue parsed = p.parse(pattern);
        svect res     = m.marche(&parsed, target);

        std::cout << "Matched Strings" << std::endl;
        for (auto matched : res) {
            std::cout << matched << std::endl;
        }

    } else {
        std::cout << desc << "\n";
    }
    return 0;
}
