#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/format.hpp>
#include <pkmnsim/base_pkmn.hpp>

base_pkmn::base_pkmn(std::string name, int num, std::string spec, std::string t1, std::string t2, std::string a1, std::string a2, std::string a3,
                        double ht, double wt, double cm, double cf,
                        int bhp, int batk, int bdef, int bspd, int bsatk, int bsdef,
                        int ehp, int eatk, int edef, int espd, int esatk, int esdef, int exp)
{
    display_name = name;
    nat_pokedex_num = num;
    species = spec;
    type1 = t1;
    type2 = t2;
    ability1 = a1;
    ability2 = a2;
    ability3 = a3;
    height = ht;
    weight = wt;
    chance_male = cm;
    chance_female = cf;
    baseHP = bhp;
    baseATK = batk;
    baseDEF = bdef;
    baseSPD = bspd;
    baseSATK = bsatk;
    baseSDEF = bsdef;
    evHP = ehp;
    evATK = eatk;
    evDEF = edef;
    evSPD = espd;
    evSATK = esatk;
    evSDEF = esdef;
    exp_yield = exp;
}

void base_pkmn::print()
{
    std::cout << boost::format("%s (#%d)") % display_name % nat_pokedex_num << std::endl;
    if(type2 != "") std::cout << boost::format("Type: %s") % type1 << std::endl;
    else std::cout << boost::format("Type: %s/%s") % type1 % type2 << std::endl;
    std::cout << boost::format("Stats: %d,%d,%d,%d,%d,%d") %
                               baseHP % baseATK % baseDEF % baseSPD % baseSATK % baseSDEF << std::endl;
}

void base_pkmn::print_verbose()
{
    std::cout << boost::format("%s (#%d)") % display_name % nat_pokedex_num << std::endl;
    std::cout << boost::format("%s Pokemon") % species << std::endl;

    if(type2.empty()) std::cout << boost::format("Type: %s") % type1 << std::endl;
    else std::cout << boost::format("Type: %s/%s") % type1 % type2 << std::endl;

    if((ability2.empty()) && (ability3.empty())) std::cout << boost::format("Ability: %s") % ability1 << std::endl;
    else
    {
        std::cout << "Potential Abilities:" << std::endl;
        std::cout << boost::format(" - %s") % ability1 << std::endl;
        if(!ability2.empty()) std::cout << boost::format(" - %s") % ability2 << std::endl;
        if(!ability3.empty()) std::cout << boost::format(" - %s") % ability3 << std::endl;
    }

    std::cout << boost::format("%d m, %d kg") % height % weight << std::endl;
    if((chance_male + chance_female) == 0.0) std::cout << "Genderless" << std::endl;
    else std::cout << boost::format("%2.1f%s Male, %2.1f%s Female") % (chance_male*100) % "%" % (chance_female*100) % "%" << std::endl;

    std::cout << "Base Stats:" << std::endl;
    std::cout << boost::format(" - HP: %d") % baseHP << std::endl;
    std::cout << boost::format(" - Attack: %d") % baseATK << std::endl;
    std::cout << boost::format(" - Defense: %d") % baseDEF << std::endl;
    std::cout << boost::format(" - Speed: %d") % baseSPD << std::endl;
    std::cout << boost::format(" - Special Attack: %d") % baseSATK << std::endl;
    std::cout << boost::format(" - Special Defense: %d") % baseSDEF << std::endl;

    std::cout << "Effort Value Yields:" << std::endl;
    std::cout << boost::format(" - HP: %d") % evHP << std::endl;
    std::cout << boost::format(" - Attack: %d") % evATK << std::endl;
    std::cout << boost::format(" - Defense: %d") % evDEF << std::endl;
    std::cout << boost::format(" - Speed: %d") % evSPD << std::endl;
    std::cout << boost::format(" - Special Attack: %d") % evSATK << std::endl;
    std::cout << boost::format(" - Special Defense: %d") % evSDEF << std::endl;

    std::cout << boost::format("Experience Yield: %d") % exp_yield << std::endl;
}

std::string base_pkmn::get_display_name() {return display_name;}
int base_pkmn::get_nat_pokedex_num() {return nat_pokedex_num;}
std::string base_pkmn::get_species() {return species;}

std::string* base_pkmn::get_types()
{
    std::string *types = new std::string[2];
    types[0] = type1;
    types[1] = type2;
    return types;
}

std::string* base_pkmn::get_abilities()
{
    std::string *abilities = new std::string[3];
    abilities[0] = ability1;
    abilities[1] = ability2;
    abilities[2] = ability3;
    return abilities;
}

double base_pkmn::get_height() {return height;}
double base_pkmn::get_weight() {return weight;}
double base_pkmn::get_chance_male() {return chance_male;}
double base_pkmn::get_chance_female() {return chance_female;}

int* base_pkmn::get_base_stats()
{
    int *stats = new int[6];

    stats[0] = baseHP;
    stats[1] = baseATK;
    stats[2] = baseDEF;
    stats[3] = baseSPD;
    stats[4] = baseSATK;
    stats[5] = baseSDEF;

    return stats;
}

int* base_pkmn::get_ev_yields()
{
    int *yields = new int[7];

    yields[0] = evHP;
    yields[1] = evATK;
    yields[2] = evDEF;
    yields[3] = evSPD;
    yields[4] = evSATK;
    yields[5] = evSDEF;
    yields[6] = exp_yield;

    return yields;
}
//get_move_list

base_pkmn get_pokemon(std::string pkmn_name)
{
    boost::algorithm::to_lower(pkmn_name);

    std::map<std::string, base_pkmn> nat_pokedex = boost::assign::map_list_of
        //
        // Generation 1
        //
        ("bulbasaur", base_pkmn("Bulbasaur",1,"Seed", "Grass","Poison", "Overgrow","Chlorophyll","",
                                0.7,6.9, 0.875,0.125, 45,49,49,65,65,45, 0,0,0,1,0,0, 64)) //RBG,FRLG starter
        ("venusaur", base_pkmn("Venusaur",3,"Seed", "Grass","Poison", "Overgrow","Chlorophyll","",
                                2.0,100.0, 0.875,0.125, 80,82,83,100,100,80, 0,0,0,2,1,0, 236))
        ("charmander", base_pkmn("Charmander",4,"Lizard", "Fire","", "Blaze","Solar Power","",
                                 0.6,8.5, 0.875,0.125, 39,52,43,60,50,65, 0,0,0,0,0,1, 62)) //RBG,FRLG starter
        ("charizard", base_pkmn("Charizard",6,"Flame", "Fire","Flying", "Blaze","Solar Power","",
                                 1.7,90.5, 0.875,0.125, 78,84,78,109,89,100, 0,0,0,3,0,0, 240))
        ("squirtle", base_pkmn("Squirtle",7,"Tiny Turtle", "Water","", "Torrent","Rain Dish","",
                               0.5,9.0, 0.875,0.125, 44,48,65,50,64,43, 0,0,1,0,0,0, 63)) //RBG,FRLG starter
        ("blastoise", base_pkmn("Blastoise",9,"Shellfish", "Water","", "Torrent","Rain Dish","",
                               1.6,85.5, 0.875,0.125, 79,83,100,85,105,78, 0,0,0,0,3,0, 239))
        ("pikachu", base_pkmn("Pikachu",25,"Mouse", "Electric","", "Static","Lightningrod","",
                              0.4,6.0, 0.5,0.5, 35,55,30,50,40,90, 0,0,0,0,0,2, 82)) //Y starter
        ("raichu", base_pkmn("Raichu",26,"Mouse", "Electric","", "Static","Lightningrod","",
                              0.8,30.0, 0.5,0.5, 60,90,55,90,80,100, 0,0,0,0,0,3, 122))
        ("oddish", base_pkmn("Oddish",43,"Weed", "Grass","Poison", "Chlorophyll","Run Away","",
                                0.5,5.4, 0.5,0.5, 45,50,55,75,65,30, 0,0,0,1,0,0, 78))
        ("gloom", base_pkmn("Gloom",44,"Weed", "Grass","Poison", "Chlorophyll","Stench","",
                                0.8,8.6, 0.5,0.5, 60,65,70,85,75,40, 0,0,0,2,0,0, 132))
        ("vileplume", base_pkmn("Vileplume",45,"Flower", "Grass","Poison", "Chlorophyll","Effect Spore","",
                                1.2,18.6, 0.5,0.5, 75,80,85,100,90,50, 0,0,0,3,0,0, 184))
        ("venonat", base_pkmn("Venonat",48,"Insect", "Bug","Poison", "Compoundeyes","Tinted Lens","Run Away",
                              1.0,30.0, 0.5,0.5, 60,55,50,40,55,45, 0,0,0,0,1,0, 75))
        ("venomoth", base_pkmn("Venomoth",49,"Poison Moth", "Bug","Poison", "Shield Dust","Tinted Lens","Wonder Skin",
                              1.5,12.5, 0.5,0.5, 70,65,60,90,75,90, 0,0,0,1,0,1, 138))
        ("growlithe", base_pkmn("Growlithe",58,"Puppy", "Fire","", "Intimidate","Flash Fire","Justified",
                                 0.7,19.0, 0.75,0.25, 55,70,45,70,50,60, 0,1,0,0,0,0, 91))
        ("arcanine", base_pkmn("Arcanine",59,"Legendary", "Fire","", "Intimidate","Flash Fire","Justified",
                              1.9,155.0, 0.75,0.25, 90,110,80,100,80,95, 0,2,0,0,0,0, 213))
        ("abra", base_pkmn("Abra",63,"Psi", "Psychic","", "Synchronize","Inner Focus","Magic Guard",
                              0.9,19.5, 0.75,0.25, 25,20,15,105,55,90, 0,0,0,1,0,0, 75))
        ("kadabra", base_pkmn("Kadabra",64,"Psi", "Psychic","", "Synchronize","Inner Focus","Magic Guard",
                              1.3,56.5, 0.75,0.25, 40,35,30,120,70,105, 0,0,0,2,0,0, 145))
        ("alakazam", base_pkmn("Alakazam",65,"Psi", "Psychic","", "Synchronize","Inner Focus","Magic Guard",
                              1.5,48.0, 0.75,0.25, 55,50,45,135,85,120, 0,0,0,3,0,0, 186))
        ("machamp", base_pkmn("Machamp",68,"Superpower", "Fighting","", "Guts","No Guard","Steadfast",
                              1.6,130.0, 0.75,0.25, 90,130,80,65,85,55, 0,3,0,0,0,0, 193))
        ("bellsprout", base_pkmn("Bellsprout",69,"Flower", "Grass","Poison", "Chlorophyll","Gluttony","",
                                0.7,4.0, 0.5,0.5, 50,75,35,70,30,40, 0,1,0,0,0,0, 84))
        ("weepinbell", base_pkmn("Weepinbell",70,"Flycatcher", "Grass","Poison", "Chlorophyll","Gluttony","",
                                1.0,6.4, 0.5,0.5, 65,90,50,85,45,55, 0,2,0,0,0,0, 151))
        ("victreebel", base_pkmn("Victreebel",71,"Flycatcher", "Grass","Poison", "Chlorophyll","Gluttony","",
                                1.7,15.5, 0.5,0.5, 80,105,65,100,60,70, 0,3,0,0,0,0, 191))
        ("geodude", base_pkmn("Geodude",74,"Rock", "Rock","Ground", "Rock Head","Sturdy","Sand Veil",
                              0.4,20.0, 0.5,0.5, 40,80,100,30,30,20, 0,0,1,0,0,0, 73))
        ("graveler", base_pkmn("Graveler",75,"Rock", "Rock","Ground", "Rock Head","Sturdy","Sand Veil",
                              1.0,105.0, 0.5,0.5, 55,95,115,45,45,35, 0,0,2,0,0,0, 134))
        ("golem", base_pkmn("Golem",76,"Megaton", "Rock","Ground", "Rock Head","Sturdy","Sand Veil",
                              1.4,300.0, 0.5,0.5, 80,110,130,55,65,45, 0,0,3,0,0,0, 177))
        ("ponyta", base_pkmn("Ponyta",77,"Fire Horse", "Fire","", "Run Away","Flash Fire","Flame Body",
                                 1.0,30.0, 0.5,0.5, 50,85,55,65,65,90, 0,0,0,0,0,1, 152))
        ("rapidash", base_pkmn("Rapidash",78,"Fire Horse", "Fire","", "Run Away","Flash Fire","Flame Body",
                                 1.7,95.0, 0.5,0.5, 65,100,70,80,80,105, 0,0,0,0,0,2, 192))
        ("grimer", base_pkmn("Grimer",88,"Sludge", "Poison","", "Stench","Sticky Hold","Poison Touch",
                                0.9,30.0, 0.5,0.5, 80,80,50,40,50,25, 1,0,0,0,0,0, 90))
        ("muk", base_pkmn("Muk",89,"Sludge", "Poison","", "Stench","Sticky Hold","Poison Touch",
                                1.2,30.0, 0.5,0.5, 105,105,75,65,100,50, 1,1,0,0,0,0, 157))
        ("onix", base_pkmn("Onix",95,"Rock Snake", "Rock","Ground", "Rock Head","Sturdy","Weak Armor",
                              8.8,210.0, 0.5,0.5, 35,45,160,30,45,70, 0,0,1,0,0,0, 108))
        ("voltorb", base_pkmn("Voltorb",100,"Ball", "Electric","", "Soundproof","Static","Aftermath",
                              0.5,10.4, 0.0,0.0, 40,30,50,55,55,100, 0,0,0,0,0,1, 103))
        ("electrode", base_pkmn("Electrode",101,"Ball", "Electric","", "Soundproof","Static","Aftermath",
                              1.2,66.6, 0.0,0.0, 60,50,70,80,80,140, 0,0,0,0,0,2, 150))
        ("exeggutor", base_pkmn("Exeggutor",103,"Coconut", "Grass","Psychic", "Chlorophyll","Harvest","",
                              2.0,120.0, 0.5,0.5, 95,95,85,125,65,55, 0,0,0,2,0,0, 212))
        ("koffing", base_pkmn("Koffing",109,"Poison Gas", "Poison","", "Levitate","","",
                                0.6,1.0, 0.5,0.5, 40,65,95,60,45,35, 0,0,1,0,0,0, 114))
        ("weezing", base_pkmn("Weezing",110,"Poison Gas", "Poison","", "Levitate","","",
                                1.2,9.5, 0.5,0.5, 65,90,120,85,70,60, 0,0,2,0,0,0, 173))
        ("tangela", base_pkmn("Tangela",114,"Vine", "Grass","", "Chlorophyll","Leaf Guard","Regenerator",
                                1.0,35.0, 0.5,0.5, 65,55,115,100,40,60, 0,0,1,0,0,0, 166))
        ("staryu", base_pkmn("Staryu",120,"Star Shape", "Water","", "Illuminate","Natural Cure","Analytic",
                              0.8,34.5, 0.0,0.0, 30,45,55,70,55,85, 0,0,0,0,0,1, 106))
        ("starmie", base_pkmn("Starmie",121,"Mysterious", "Water","Psychic", "Illuminate","Natural Cure","Analytic",
                              1.1,80.0, 0.0,0.0, 60,75,85,100,85,115, 0,0,0,0,0,2, 207))
        ("mr. mime", base_pkmn("Mr. Mime",122,"Barrier", "Psychic","", "Soundproof","Filter","Technician",
                              1.3,54.5, 0.5,0.5, 40,45,65,100,120,90, 0,0,0,0,2,0, 136))
        ("gyarados", base_pkmn("Gyarados",130,"Atrocious", "Water","Flying", "Intimidate","Moxie","",
                              6.5,235.0, 0.5,0.5, 95,125,79,60,100,81, 0,2,0,0,0,0, 214))
        ("lapras", base_pkmn("Lapras",131,"Transport", "Water","Ice", "Water Absorb","Shell Armor","Hydration",
                              2.5,220.0, 0.5,0.5, 130,85,80,85,95,60, 2,0,0,0,0,0, 219))
        ("aerodactyl", base_pkmn("Aerodactyl",142,"Fossil", "Rock","Flying", "Rock Head","Pressure","Unnerve",
                              1.9,59.0, 0.875,0.125, 80,105,65,60,75,130, 0,0,0,0,0,2, 202))
        ("snorlax", base_pkmn("Snorlax",143,"Sleeping", "Normal","", "Immunity","Thick Fat","Gluttony",
                              2.1,460.0, 0.875,0.125, 160,110,65,65,110,30, 2,0,0,0,0,0, 154))
        //
        // Generation 2
        //
        ("chikorita", base_pkmn("Chikorita",152,"Leaf", "Grass","", "Overgrow","Leaf Guard","",
                                0.9,6.4, 0.875,0.125, 45,49,65,49,65,45, 0,0,0,0,1,0, 64)) //GSC,HGSS starter
        ("cyndaquil", base_pkmn("Cyndaquil",155,"Fire Mouse", "Fire","", "Blaze","Flash Fire","",
                                0.5,7.9, 0.875,0.125, 39,52,43,60,50,65, 0,0,0,0,0,1, 65)) //GSC,HGSS starter
        ("totodile", base_pkmn("Totodile",158,"Big Jaw", "Water","", "Torrent","Sheer Force","",
                               0.6,9.5, 0.875,0.125, 50,65,64,44,48,43, 0,1,0,0,0,0, 66)) //GSC,HGSS starter
        ("bellossom", base_pkmn("Bellossom",182,"Flower", "Grass","", "Chlorophyll","Healer","",
                                0.4,5.8, 0.5,0.5, 75,80,85,90,100,50, 0,0,0,0,3,0, 184))
        ("steelix", base_pkmn("Steelix",208,"Iron Snake", "Steel","Ground", "Rock Head","Sturdy","Sheer Force",
                              9.2,400.0, 0.5,0.5, 75,85,200,55,65,30, 0,0,2,0,0,0, 196))
        //
        // Generation 3
        //
        ("treecko", base_pkmn("Treecko",252,"Wood Gecko", "Grass","", "Overgrow","Unburden","",
                              0.5,5.0, 0.875,0.125, 40,45,35,65,55,70, 0,0,0,0,0,1, 65)) //RSE starter
        ("torchic", base_pkmn("Torchic",255,"Chick", "Fire","", "Blaze","Speed Boost","",
                              0.4,2.5, 0.875,0.125, 45,60,40,70,50,45, 0,0,0,1,0,0, 65)) //RSE starter
        ("mudkip", base_pkmn("Mudkip",258,"Mud Fish", "Water","", "Torrent","Damp","",
                             0.4,7.6, 0.875,0.125, 50,70,50,50,50,40, 0,1,0,0,0,0, 65)) //RSE starter
        //
        // Generation 4
        //
        ("turtwig", base_pkmn("Turtwig",387,"Tiny Leaf", "Grass","", "Overgrow","Shell Armor","",
                              0.4,10.2, 0.875,0.125, 55,68,64,45,55,31, 0,1,0,0,0,0, 64)) //DPP starter
        ("chimchar", base_pkmn("Chimchar",390,"Chimp", "Fire","", "Blaze","Iron Fist","",
                               0.5,6.2, 0.875,0.125, 44,58,44,58,44,61, 0,0,0,0,0,1, 65)) //DPP starter
        ("piplup", base_pkmn("Piplup",393,"Penguin", "Water","", "Torrent","Defiant","",
                             0.4,5.2, 0.875,0.125, 53,51,53,61,56,40, 0,0,0,1,0,0, 66)) //DPP starter
        ("mime jr.", base_pkmn("Mime Jr.",439,"Mime", "Psychic","", "Soundproof","Filter","Technician",
                              0.6,13.0, 0.5,0.5, 20,25,45,70,90,60, 0,0,0,0,1,0, 78))
        ("tangrowth", base_pkmn("Tangrowth",465,"Vine", "Grass","", "Chlorophyll","Leaf Guard","Regenerator",
                                2.0,128.6, 0.5,0.5, 100,100,125,110,50,50, 0,0,2,0,0,0, 211))
        //
        // Generation 5
        //
        ("snivy", base_pkmn("Snivy",495,"Grass Snake", "Grass","", "Overgrow","Contrary","",
                            0.6,8.1, 0.875,0.125, 45,45,55,45,55,63, 0,0,0,0,0,1, 28)) //BW,B2W2 starter
        ("tepig", base_pkmn("Tepig",498,"Fire Pig", "Fire","", "Blaze","Thick Fat","",
                        0.5,9.9, 0.875,0.125, 65,63,45,45,45,45, 1,0,0,0,0,0, 28)) //BW,B2W2 starter
        ("oshawatt", base_pkmn("Oshawatt",501,"Sea Otter", "Water","", "Torrent","Shell Armor","",
                               0.5,5.9, 0.875,0.125, 55,55,45,63,45,45, 0,0,0,1,0,0, 28)) //BW,B2W2 starter
    ;

    std::map<std::string, base_pkmn>::iterator iter = nat_pokedex.find(pkmn_name);
    if(iter != nat_pokedex.end()) return nat_pokedex[pkmn_name];
    else throw std::runtime_error("Pokemon not in National Pokedex.");
}
