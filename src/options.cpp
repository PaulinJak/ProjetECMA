//options file
#ifndef CPP_OPTIONS
#define CPP_OPTIONS

#include "options.hpp"

#include <boost/program_options.hpp>
using namespace boost::program_options;

#include <iostream>
using namespace std;

namespace po = boost::program_options;

// Declare the supported options.
po::options_description desc("Allowed options");

desc.add_options()
    ("help", "produce help message")
("instanceFile", std::string, "path of instance.dat file ")
;

#endif
