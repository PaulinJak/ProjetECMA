//options file
#ifndef HPP_OPTIONS
#define HPP_OPTIONS

using namespace std;

class option{
private :
  string option_name;
  string option_description;
public :
  string option_value;
  
}

// Declare the supported options.
void init(int argc, char* argv[]){

po::options_description desc("Allowed options");
desc.add_options()
    ("help", "produce help message")
  ("instanceFile", po::value< vector<string> >(), "input instance filename");


po::variables_map vm;
po::store(po::parse_command_line(argc, argv, desc), vm);
po::notify(vm);    

if (vm.count("help")) {
    cout << desc << "\n";
    }

/*if (vm.count("instanceFile")) {
    cout << "Instance input given by file" 
 << vm["instance"].as<string>() << ".\n";
} else {
    cout << "No instance given.\n";
    }*/


}
#endif
