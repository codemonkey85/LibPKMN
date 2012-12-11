#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <map>
#include <pkmnsim/base_pkmn.hpp>
#include <sqlitecpp/SQLiteCPP.h>
#include <stdexcept>
#include <vector>

namespace po = boost::program_options;
typedef vector<base_pkmn>::iterator pkmn_iter;
using namespace std;

struct stat
{
    string stat_name;
    string pkmn_name;
    int stat_value;

    stat()
    {
        pkmn_name = "Missingno.";
        stat_value = -1;
    }

    stat(string sn)
    {
        stat_name = sn;
        pkmn_name = "Missingno.";
        stat_value = -1;
    }
};

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
        ("evolved", "Only check fully evolved Pokemon (unimplemented)")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help") > 0)
    {
        cout << "\nSearch Type Combo - " << desc << endl;
        return EXIT_FAILURE;
    }
    bool lax = vm.count("lax");
    if(type1 == "None")
    {
        cout << "\nSpecify a type with type1=(type)." << endl;
        cout << "\nSearch Type Combo - " << desc << endl;
        return EXIT_FAILURE;
    }

    stat highest_stats[6];
    highest_stats[0] = stat("HP");
    highest_stats[1] = stat("Attack");
    highest_stats[2] = stat("Defense");
    highest_stats[3] = stat("Speed");
    highest_stats[4] = stat("Special Attack");
    highest_stats[5] = stat("Special Defense");
    stat lowest_stats[6];

    vector<base_pkmn> pkmn_vector = get_pkmn_of_type(type1,type2,lax);
    if(pkmn_vector.begin() == pkmn_vector.end())
    {
        cout << "\nNo Pokemon of specified type.\n";
        return EXIT_FAILURE;
    }

    for(pkmn_iter pi = pkmn_vector.begin(); pi != pkmn_vector.end(); ++pi)
    {
        int* pkmn_stats = pi->get_base_stats();

        for(int i = 0; i < 6; i++)
        {
            if(highest_stats[i].pkmn_name == "Missingno." or highest_stats[i].stat_value < pkmn_stats[i])
            {
                highest_stats[i].pkmn_name = pi->get_display_name();
                highest_stats[i].stat_value = pkmn_stats[i];
            }
            if(lowest_stats[i].pkmn_name == "Missingno." or lowest_stats[i].stat_value > pkmn_stats[i])
            {
                lowest_stats[i].pkmn_name = pi->get_display_name();
                lowest_stats[i].stat_value = pkmn_stats[i];
            }
        }
    }

    string type_str;
    if(lax and type2 == "None") type_str = str(boost::format("%s (any second type)") % type1);
    else type_str = str(boost::format("%s/%s") % type1 % type2);
    cout << boost::format("Stat analysis for Pokemon of type(s): %s\n\n") % type_str;

    for(int i = 0; i < 6; i++)
    {
        cout << boost::format("%s\n") % highest_stats[i].stat_name;
        cout << boost::format(" * Highest: %s (%d)\n") % highest_stats[i].pkmn_name % highest_stats[i].stat_value;
        cout << boost::format(" * Lowest: %s (%d)\n\n") % lowest_stats[i].pkmn_name % lowest_stats[i].stat_value;
    }

    return EXIT_SUCCESS;
}
