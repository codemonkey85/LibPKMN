#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <pkmnsim/base_pkmn.hpp>
#include <sqlitecpp/SQLiteCPP.h>
#include <stdexcept>
#include <vector>

namespace po = boost::program_options;
typedef vector<base_pkmn>::iterator pkmn_iter;
using namespace std;

int main(int argc, char *argv[])
{
    //Taking in user options
    string type1;
    string type2;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "Display this help message.")
        ("type1", po::value<string>(&type1)->default_value("None"), "Type 1 to search for")
        ("type2", po::value<string>(&type2)->default_value("None"), "Type 2 to search for, default=None")
        ("lax", "If only specifying one type, specify '--lax' to include any type combination with that type")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help") > 0) cout << "Search Type Combo - " << desc << endl;
    bool lax = vm.count("lax");
    if(type1 == "None")
    {
        cout << "Specify a type with type1=(type)." << endl;
        return EXIT_FAILURE;
    }

    vector<base_pkmn> pkmn_vector = get_pkmn_of_type(type1,type2,lax);
    for(pkmn_iter pi = pkmn_vector.begin(); pi != pkmn_vector.end(); ++pi) cout << pi->get_display_name() << endl;

    return EXIT_SUCCESS;
}
