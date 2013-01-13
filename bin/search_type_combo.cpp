#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <map>
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/base_gen1pkmn.hpp>
#include <pkmnsim/base_gen2pkmn.hpp>
#include <stdexcept>
#include <vector>

using namespace std;
namespace po = boost::program_options;
typedef vector<base_pkmn>::iterator pkmn_iter;
typedef vector<base_gen1pkmn>::iterator gen1pkmn_iter;
typedef vector<base_gen2pkmn>::iterator gen2pkmn_iter;

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
    int gen;
    string type1;
    string type2;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "Display this help message.")
        ("gen", po::value<int>(&gen)->default_value(5), "Which generation to use")
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
    if(gen == 1) highest_stats[4] = stat("Special");
    else
    {
        highest_stats[4] = stat("Special Attack");
        highest_stats[5] = stat("Special Defense");
    }
    stat lowest_stats[6] = highest_stats;

    vector<base_gen1pkmn> gen1pkmn_vector;
    vector<base_gen2pkmn> gen2pkmn_vector;
    vector<base_pkmn> pkmn_vector;
    string type_str;

    int num_stats = 6;
    switch(gen)
    {
        case 1:
            //Generation 1

            num_stats = 5;

            get_gen1_pkmn_of_type(pkmn_vector, type1, type2, lax);
            break;

        case 2:
            //Generation 2

            get_gen2_pkmn_of_type(pkmn_vector, type1, type2, lax);
            break;

        default:
            //Generation 3-5

            get_pkmn_of_type(pkmn_vector, type1, type2, gen, lax);
            break;
    }
    if(pkmn_vector.size() == 0)
    {
        cout << boost::format("\nNo Pokemon of specified type in Generation %d.\n") % gen;
        return EXIT_FAILURE;
    }
    //pkmn_vector[0]->print();
    /*for(int i = 0; i < pkmn_vector.size(); i++)
    {
        cout << i << endl;
        //int* pkmn_stats = pkmn_vector[i].get_base_stats();
        for(int i = 0; i < num_stats; i++)
        {
            cout << highest_stats[i].stat_name << endl;
        }
    }*/

    /*switch(gen)
    {
        case 1:
            gen1pkmn_vector = get_gen1_pkmn_of_type(type1,type2,lax);
            if(gen1pkmn_vector.begin() == gen1pkmn_vector.end())
            {
                cout << "\nNo Pokemon of specified type.\n";
                return EXIT_FAILURE;
            }

            for(gen1pkmn_iter pi = gen1pkmn_vector.begin(); pi != gen1pkmn_vector.end(); ++pi)
            {
                int* pkmn_stats = pi->get_base_stats();

                for(int i = 0; i < 5; i++)
                {
                    if(gen1highest_stats[i].pkmn_name == "Missingno." or gen1highest_stats[i].stat_value < pkmn_stats[i])
                    {
                        gen1highest_stats[i].pkmn_name = pi->get_display_name();
                        gen1highest_stats[i].stat_value = pkmn_stats[i];
                    }
                    if(gen1lowest_stats[i].pkmn_name == "Missingno." or gen1lowest_stats[i].stat_value > pkmn_stats[i])
                    {
                        gen1lowest_stats[i].pkmn_name = pi->get_display_name();
                        gen1lowest_stats[i].stat_value = pkmn_stats[i];
                    }
                }
            }

            if(lax and type2 == "None") type_str = str(boost::format("%s (any second type)") % type1);
            else type_str = str(boost::format("%s/%s") % type1 % type2);
            cout << boost::format("Stat analysis for Pokemon of type(s): %s (Generation 1)\n\n") % type_str;

            for(int i = 0; i < 5; i++)
            {
                cout << boost::format("%s\n") % gen1highest_stats[i].stat_name;
                cout << boost::format(" * Highest: %s (%d)\n") % gen1highest_stats[i].pkmn_name % gen1highest_stats[i].stat_value;
                cout << boost::format(" * Lowest: %s (%d)\n\n") % gen1lowest_stats[i].pkmn_name % gen1lowest_stats[i].stat_value;
            }
            break;

        case 2:
            gen2pkmn_vector = get_gen2_pkmn_of_type(type1,type2,lax);
            if(gen2pkmn_vector.begin() == gen2pkmn_vector.end())
            {
                cout << "\nNo Pokemon of specified type.\n";
                return EXIT_FAILURE;
            }

            for(gen2pkmn_iter pi = gen2pkmn_vector.begin(); pi != gen2pkmn_vector.end(); ++pi)
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

            if(lax and type2 == "None") type_str = str(boost::format("%s (any second type)") % type1);
            else type_str = str(boost::format("%s/%s") % type1 % type2);
            cout << boost::format("Stat analysis for Pokemon of type(s): %s (Generation 2)\n\n") % type_str;

            for(int i = 0; i < 6; i++)
            {
                cout << boost::format("%s\n") % highest_stats[i].stat_name;
                cout << boost::format(" * Highest: %s (%d)\n") % highest_stats[i].pkmn_name % highest_stats[i].stat_value;
                cout << boost::format(" * Lowest: %s (%d)\n\n") % lowest_stats[i].pkmn_name % lowest_stats[i].stat_value;
            }
            break;

        default:
            pkmn_vector = get_pkmn_of_type(type1,type2,gen,lax);
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

            if(lax and type2 == "None") type_str = str(boost::format("%s (any second type)") % type1);
            else type_str = str(boost::format("%s/%s") % type1 % type2);
            cout << boost::format("Stat analysis for Pokemon of type(s): %s (Generation %d)\n\n") % type_str % gen;

            for(int i = 0; i < 6; i++)
            {
                cout << boost::format("%s\n") % highest_stats[i].stat_name;
                cout << boost::format(" * Highest: %s (%d)\n") % highest_stats[i].pkmn_name % highest_stats[i].stat_value;
                cout << boost::format(" * Lowest: %s (%d)\n\n") % lowest_stats[i].pkmn_name % lowest_stats[i].stat_value;
            }
    }*/

    return EXIT_SUCCESS;
}
