/*
***********************************************
PKMDS Code Library - Gen V

Created by Michael Bond (aka Codemonkey85)
https://plus.google.com/116414067936940758871/

Feel free to use and reuse this code as you see fit, but I
implore you to always link back to me as the original creator.
***********************************************

Thanks to Alex "eevee" Munroe at http://veekun.com/
for his SQLite Pokedex database, which powers this software.

Thanks to the fine folks at SQLite.org for making it possible
to use the Pokedex database... the source files "sqlite3.c"
and "sqlite3.h" came from these people.

Thanks to those of Project Pokemon (http://projectpokemon.org/)
who have helped research and document the underlying structure
of Pokemon game save files.
*/
//http://www.projectpokemon.org/wiki/Pokemon_Black/White_NDS_Structure
#include "pkmds_g5.h"
byte getpkmshuffleindex(const uint32 pid){
	return ((pid & 0x3e000) >> 0xd) % 24;
};
byte getpkmshuffleindex(const pokemon_obj &pkm){
	return getpkmshuffleindex(pkm.block0.pid);
};
byte getpkmshuffleindex(const pokemon_obj *pkm){
	return getpkmshuffleindex(pkm->block0.pid);
};
void unshufflepkm(pokemon_obj &pkm)
{
	std::string shufforders[25] = 
	{
		"ABCD",
		"ABDC",
		"ACBD",
		"ADBC",
		"ACDB",
		"ADCB",
		"BACD",
		"BADC",
		"CABD",
		"DABC",
		"CADB",
		"DACB",
		"BCAD",
		"BDAC",
		"CBAD",
		"DBAC",
		"CDAB",
		"DCAB",
		"BCDA",
		"BDCA",
		"CBDA",
		"DBCA",
		"CDBA",
		"DCBA"
	};
	std::string shuffleorder = shufforders[getpkmshuffleindex(pkm)];
	byte* pkmpnt = reinterpret_cast<byte*>(&pkm);
	pkmblocka mblocka = pkm.blocka;
	pkmblockb mblockb = pkm.blockb;
	pkmblockc mblockc = pkm.blockc;
	pkmblockd mblockd = pkm.blockd;
	byte* oblocka = reinterpret_cast<byte*>(&mblocka);
	byte* oblockb = reinterpret_cast<byte*>(&mblockb);
	byte* oblockc = reinterpret_cast<byte*>(&mblockc);
	byte* oblockd = reinterpret_cast<byte*>(&mblockd);
	pkmpnt += 8;
	for(int i=0; i < 4; i++)
	{
		switch(shuffleorder[i])
		{
		case 'A':
			memcpy(pkmpnt,oblocka,32);
			break;
		case 'B':
			memcpy(pkmpnt,oblockb,32);
			break;
		case 'C':
			memcpy(pkmpnt,oblockc,32);
			break;
		case 'D':
			memcpy(pkmpnt,oblockd,32);
			break;
		}
		pkmpnt += 32;
	}
}
void shufflepkm(pokemon_obj &pkm)
{
	std::string shufforders[25] = 
	{
		"ABCD",
		"ABDC",
		"ACBD",
		"ACDB",
		"ADBC",
		"ADCB",
		"BACD",
		"BADC",
		"BCAD",
		"BCDA",
		"BDAC",
		"BDCA",
		"CABD",
		"CADB",
		"CBAD",
		"CBDA",
		"CDAB",
		"CDBA",
		"DABC",
		"DACB",
		"DBAC",
		"DBCA",
		"DCAB",
		"DCBA"
	};
	std::string shuffleorder = shufforders[getpkmshuffleindex(pkm)];
	byte* pkmpnt = reinterpret_cast<byte*>(&pkm);
	pkmblocka mblocka = pkm.blocka;
	pkmblockb mblockb = pkm.blockb;
	pkmblockc mblockc = pkm.blockc;
	pkmblockd mblockd = pkm.blockd;
	byte* oblocka = reinterpret_cast<byte*>(&mblocka);
	byte* oblockb = reinterpret_cast<byte*>(&mblockb);
	byte* oblockc = reinterpret_cast<byte*>(&mblockc);
	byte* oblockd = reinterpret_cast<byte*>(&mblockd);
	pkmpnt += 8;
	for(int i=0; i < 4; i++)
	{
		switch(shuffleorder[i])
		{
		case 'A':
			memcpy(pkmpnt,oblocka,32);
			break;
		case 'B':
			memcpy(pkmpnt,oblockb,32);
			break;
		case 'C':
			memcpy(pkmpnt,oblockc,32);
			break;
		case 'D':
			memcpy(pkmpnt,oblockd,32);
			break;
		}
		pkmpnt += 32;
	}
}
void pkmcrypt(pokemon_obj& pkm){

	pkmprng prng;
	prng.mseed = pkm.block0.checksum;

	uint16 * words = reinterpret_cast<uint16*>(&pkm);

	for(int i = 4; i < 68; i++)
	{
		words[i] = (words[i]) ^ (prng.nextnum() >> 0x10);
	};

};
void pkmcrypt(party_field& pkm, uint32 pid){

	pkmprng prng;
	prng.mseed = pid;

	uint16 * words = reinterpret_cast<uint16*>(&pkm);

	for(int i = 0x0; i < 0x2a; i++)
	{
		words[i] = (words[i]) ^ (prng.nextnum() >> 0x10);
	};

};
void pkmcrypt(party_field* pkm, uint32 pid){

	pkmprng prng;
	prng.mseed = pid;

	uint16 * words = reinterpret_cast<uint16*>(pkm);

	for(int i = 0x0; i < 0x2a; i++)
	{
		words[i] = (words[i]) ^ (prng.nextnum() >> 0x10);
	};

};
void encryptpkm(pokemon_obj& pkm){
	shufflepkm(pkm);
	pkmcrypt(pkm);
};
void decryptpkm(pokemon_obj& pkm){
	pkmcrypt(pkm);
	unshufflepkm(pkm);
};
void encryptpkm(party_pkm& pkm){
	shufflepkm(pkm.pkm_data);
	pkmcrypt(pkm.pkm_data);
	pkmcrypt(pkm.party_data,pkm.pkm_data.block0.pid);
};
void decryptpkm(party_pkm& pkm){
	pkmcrypt(pkm.pkm_data);
	pkmcrypt(pkm.party_data,pkm.pkm_data.block0.pid);
	unshufflepkm(pkm.pkm_data);
};
void encryptpkm(party_pkm* pkm){
	shufflepkm(pkm->pkm_data);
	pkmcrypt(&(pkm->pkm_data));
	pkmcrypt(&(pkm->party_data),pkm->pkm_data.block0.pid);
};
void decryptpkm(party_pkm* pkm){
	pkmcrypt(&(pkm->pkm_data));
	pkmcrypt(&(pkm->party_data),pkm->pkm_data.block0.pid);
	unshufflepkm(pkm->pkm_data);
};
void unshufflepkm(pokemon_obj *pkm)
{
	std::string shufforders[25] = 
	{
		"ABCD",
		"ABDC",
		"ACBD",
		"ADBC",
		"ACDB",
		"ADCB",
		"BACD",
		"BADC",
		"CABD",
		"DABC",
		"CADB",
		"DACB",
		"BCAD",
		"BDAC",
		"CBAD",
		"DBAC",
		"CDAB",
		"DCAB",
		"BCDA",
		"BDCA",
		"CBDA",
		"DBCA",
		"CDBA",
		"DCBA"
	};
	std::string shuffleorder = shufforders[getpkmshuffleindex(pkm)];
	byte* pkmpnt = reinterpret_cast<byte*>(pkm);
	pkmblocka mblocka = pkm->blocka;
	pkmblockb mblockb = pkm->blockb;
	pkmblockc mblockc = pkm->blockc;
	pkmblockd mblockd = pkm->blockd;
	byte* oblocka = reinterpret_cast<byte*>(&mblocka);
	byte* oblockb = reinterpret_cast<byte*>(&mblockb);
	byte* oblockc = reinterpret_cast<byte*>(&mblockc);
	byte* oblockd = reinterpret_cast<byte*>(&mblockd);
	pkmpnt += 8;
	for(int i=0; i < 4; i++)
	{
		switch(shuffleorder[i])
		{
		case 'A':
			memcpy(pkmpnt,oblocka,32);
			break;
		case 'B':
			memcpy(pkmpnt,oblockb,32);
			break;
		case 'C':
			memcpy(pkmpnt,oblockc,32);
			break;
		case 'D':
			memcpy(pkmpnt,oblockd,32);
			break;
		}
		pkmpnt += 32;
	}
}
void shufflepkm(pokemon_obj *pkm)
{
	std::string shufforders[25] = 
	{
		"ABCD",
		"ABDC",
		"ACBD",
		"ACDB",
		"ADBC",
		"ADCB",
		"BACD",
		"BADC",
		"BCAD",
		"BCDA",
		"BDAC",
		"BDCA",
		"CABD",
		"CADB",
		"CBAD",
		"CBDA",
		"CDAB",
		"CDBA",
		"DABC",
		"DACB",
		"DBAC",
		"DBCA",
		"DCAB",
		"DCBA"
	};
	std::string shuffleorder = shufforders[getpkmshuffleindex(pkm)];
	byte* pkmpnt = reinterpret_cast<byte*>(pkm);
	pkmblocka mblocka = pkm->blocka;
	pkmblockb mblockb = pkm->blockb;
	pkmblockc mblockc = pkm->blockc;
	pkmblockd mblockd = pkm->blockd;
	byte* oblocka = reinterpret_cast<byte*>(&mblocka);
	byte* oblockb = reinterpret_cast<byte*>(&mblockb);
	byte* oblockc = reinterpret_cast<byte*>(&mblockc);
	byte* oblockd = reinterpret_cast<byte*>(&mblockd);
	pkmpnt += 8;
	for(int i=0; i < 4; i++)
	{
		switch(shuffleorder[i])
		{
		case 'A':
			memcpy(pkmpnt,oblocka,32);
			break;
		case 'B':
			memcpy(pkmpnt,oblockb,32);
			break;
		case 'C':
			memcpy(pkmpnt,oblockc,32);
			break;
		case 'D':
			memcpy(pkmpnt,oblockd,32);
			break;
		}
		pkmpnt += 32;
	}
}
void pkmcrypt(pokemon_obj* pkm){

	pkmprng prng;
	prng.mseed = pkm->block0.checksum;

	uint16 * words = reinterpret_cast<uint16*>(pkm);

	for(int i = 4; i < 68; i++)
	{
		words[i] = (words[i]) ^ (prng.nextnum() >> 0x10);
	};

};
void encryptpkm(pokemon_obj* pkm){
	shufflepkm(pkm);
	pkmcrypt(pkm);
};
void decryptpkm(pokemon_obj* pkm){
	pkmcrypt(pkm);
	unshufflepkm(pkm);
};
uint16 getchecksum(bw2savblock_obj &block, const int start, const int length){
	byte* data = reinterpret_cast<byte*>(&block);
	int sum = 0xFFFF;
	for ( int i = start; i < start + length; i++ ){
		sum = (sum << 8) ^ SeedTable[ (byte)(data[i] ^ (byte)(sum>>8)) ];};
		return (uint16)sum;
};
void calcpartychecksum(bw2savblock_obj &block) // ,bool bw2)
{
	uint16 chk = getchecksum(block,0x18E00,0x534);
	byte* data = reinterpret_cast<byte*>(&block);
	memcpy(&*(data+0x19336),&chk,2);
};
void calcboxchecksum(bw2savblock_obj &block, int boxindex, bool bw2){
	int start = (0x400 + (boxindex * 0x1000));
	uint16 chk = getchecksum(block,start,boxsize);
	block.boxes[boxindex].checksum = chk;
	byte* data = reinterpret_cast<byte*>(&block);
	if (bw2) 
		memcpy(data + bw2chkcalcloc + 2 + (boxindex * 2), &chk, 2);
	else 
		memcpy(data + bwchkcalcloc + 2 + (boxindex * 2), &chk, 2);
};
void calcchecksum(bw2savblock_obj &block, int start, int length, int loc){
	byte* data = reinterpret_cast<byte*>(&block);
	int sum = 0xFFFF;
	for ( int i = start; i < start + length; i++ )
		sum = (sum << 8) ^ SeedTable[ (byte)(data[i] ^ (byte)(sum>>8)) ];
	uint16 chk = (uint16)sum;
	memcpy(&*(data+loc),&chk,2);
};
uint16 getchkfromsav(bw2savblock_obj &block, bool bw2){
	byte* data = reinterpret_cast<byte*>(&block);
	uint16 chk;
	if (bw2)
		chk = (data[bw2chkloc]) + (data[bw2chkloc+1] * 256);
	else
		chk = (data[bwchkloc]) + (data[bwchkloc+1] * 256);
	return chk;
};
void calcchecksum(pokemon_obj& pkm) // Calculates and assigns the checksum for the given Pokemon object.
{
	uint32 chk = 0;
	uint16* p = (uint16*)&pkm;
	for (uint16* i = p + 0x04; i < p + 0x43; i++)
	{
		chk += *i;
	};
	chk = chk & 0xffff;
	pkm.block0.checksum = chk;
};
uint16 getchecksum(bw2savblock_obj *block, const int start, const int length){
	byte* data = reinterpret_cast<byte*>(block);
	int sum = 0xFFFF;
	for ( int i = start; i < start + length; i++ ){
		sum = (sum << 8) ^ SeedTable[ (byte)(data[i] ^ (byte)(sum>>8)) ];};
		return (uint16)sum;
};
void calcpartychecksum(bw2savblock_obj *block) // ,bool bw2)
{
	uint16 chk = getchecksum(block,0x18E00,0x534);
	byte* data = reinterpret_cast<byte*>(block);
	memcpy(&*(data+0x19336),&chk,2);
};
void calcboxchecksum(bw2savblock_obj *block, int boxindex, bool bw2){
	int start = (0x400 + (boxindex * 0x1000));
	uint16 chk = getchecksum(block,start,boxsize);
	block->boxes[boxindex].checksum = chk;
	byte* data = reinterpret_cast<byte*>(block);
	if (bw2) 
		memcpy(data + bw2chkcalcloc + 2 + (boxindex * 2), &chk, 2);
	else 
		memcpy(data + bwchkcalcloc + 2 + (boxindex * 2), &chk, 2);
};
void calcchecksum(bw2savblock_obj *block, int start, int length, int loc){
	byte* data = reinterpret_cast<byte*>(block);
	int sum = 0xFFFF;
	for ( int i = start; i < start + length; i++ )
		sum = (sum << 8) ^ SeedTable[ (byte)(data[i] ^ (byte)(sum>>8)) ];
	uint16 chk = (uint16)sum;
	memcpy(&*(data+loc),&chk,2);
};
uint16 getchkfromsav(bw2savblock_obj *block, bool bw2){
	byte* data = reinterpret_cast<byte*>(block);
	uint16 chk;
	if (bw2)
		chk = (data[bw2chkloc]) + (data[bw2chkloc+1] * 256);
	else
		chk = (data[bwchkloc]) + (data[bwchkloc+1] * 256);
	return chk;
};
void calcchecksum(pokemon_obj* pkm) // Calculates and assigns the checksum for the given Pokemon object.
{
	uint32 chk = 0;
	uint16* p = (uint16*)&pkm;
	for (uint16* i = p + 0x04; i < p + 0x43; i++)
	{
		chk += *i;
	};
	chk = chk & 0xffff;
	pkm->block0.checksum = chk;
};
bool savisbw2(bw2sav_obj &sav)
{
	return (getchecksum(sav.cur,bw2chkcalcloc,bw2chkcalclen)) == (getchkfromsav(sav.cur,true));
}
bool savisbw2(bw2sav_obj *sav)
{
	return (getchecksum(sav->cur,bw2chkcalcloc,bw2chkcalclen)) == (getchkfromsav(sav->cur,true));
}
void fixsavchecksum(bw2sav_obj &sav)
{
	if(savisbw2(sav))
	{
		calcchecksum(sav.cur, bw2chkcalcloc, bw2chkcalclen, bw2chkloc);
	}
	else
	{
		calcchecksum(sav.cur, bwchkcalcloc, bwchkcalclen, bwchkloc);
	}
}
void fixsavchecksum(bw2sav_obj *sav)
{
	if(savisbw2(sav))
	{
		calcchecksum(sav->cur, bw2chkcalcloc, bw2chkcalclen, bw2chkloc);
	}
	else
	{
		calcchecksum(sav->cur, bwchkcalcloc, bwchkcalclen, bwchkloc);
	}
}
void write(const char* file_name, pokemon_obj& data) // Writes the given Pokemon data to the given file name.
{
	std::ofstream *out = new std::ofstream(file_name,std::ios::binary);
	out->write(reinterpret_cast<char*>(&data), sizeof(pokemon_obj));
	out->close();
	delete out;
	out = 0;
};
//void write(const wchar_t* file_name, pokemon_obj& data) // Writes the given Pokemon data to the given file name.
//{
//	std::ofstream out;
//	out.open(file_name,std::ios::binary);
//	out.write(reinterpret_cast<char*>(&data), sizeof(pokemon_obj));
//	out.close();
//};
void write(const char* file_name, pokemon_obj* data) // Writes the given Pokemon data to the given file name.
{
	std::ofstream *out = new std::ofstream(file_name,std::ios::binary);
	out->write(reinterpret_cast<char*>(data), sizeof(pokemon_obj));
	out->close();
	delete out;
	out = 0;
};
//void write(const wchar_t* file_name, pokemon_obj* data) // Writes the given Pokemon data to the given file name.
//{
//	std::ofstream out;
//	out.open(file_name,std::ios::binary);
//	out.write(reinterpret_cast<char*>(data), sizeof(pokemon_obj));
//	out.close();
//};
void write(const char* file_name, bw2sav_obj& data) //, int start, int length, int loc) //
{
	std::ofstream *out = new std::ofstream(file_name,std::ios::binary);
	out->write(reinterpret_cast<char*>(&data), sizeof(bw2sav_obj));
	out->close();
	delete out;
	out = 0;
};
void write(const char* file_name, bw2sav_obj *data) //, int start, int length, int loc) //
{
	std::ofstream *out = new std::ofstream(file_name,std::ios::binary);
	out->write(reinterpret_cast<char*>(data), sizeof(bw2sav_obj));
	out->close();
	delete out;
	out = 0;
};
void read(const char* file_name, pokemon_obj& data) // Reads the given file and assigns the data to the given Pokemon object.
{
	std::ifstream *in = new std::ifstream(file_name,std::ios::binary);
	in->read(reinterpret_cast<char*>(&data), sizeof(pokemon_obj));
	in->close();
	delete in;
	in = 0;
};
void read(const char* file_name, pokemon_obj *data) // Reads the given file and assigns the data to the given save file object.
{
	std::ifstream *in = new std::ifstream(file_name,std::ios::binary);
	in->read(reinterpret_cast<char*>(data), sizeof(pokemon_obj));
	in->close();
	delete in;
	in = 0;
};
void read(const char* file_name, bw2sav_obj& data) // Reads the given file and assigns the data to the given save file object.
{
	std::ifstream *in = new std::ifstream(file_name,std::ios::binary);
	in->read(reinterpret_cast<char*>(&data), sizeof(bw2sav_obj));
	in->close();
	delete in;
	in = 0;
};
void read(const char* file_name, bw2sav_obj *data) // Reads the given file and assigns the data to the given save file object.
{
	std::ifstream *in = new std::ifstream(file_name,std::ios::binary);
	in->read(reinterpret_cast<char*>(data), sizeof(bw2sav_obj));
	in->close();
	delete in;
	in = 0;
};
std::wstring getpkmnickname(const pokemon_obj &pkm)
{
	std::wstring nick = pkm.blockc.nickname;
	if(nick.find((wchar_t)0xffff))
	{
		nick = nick.substr(0,nick.find((wchar_t)0xffff));
	}
	return nick;
};
std::wstring getpkmotname(const pokemon_obj &pkm)
{
	std::wstring otname = pkm.blockd.otname;
	if(otname.find((wchar_t)0xffff))
	{
		otname = otname.substr(0,otname.find((wchar_t)0xffff));
	}
	return otname;
};
void setpkmnickname(pokemon_obj &pkm, wchar_t input[], int length){
	if(length > 8){length = 8;}
	memset(&pkm.blockc.nickname, '\0', 20);
	memcpy(&pkm.blockc.nickname,input,length*2);
	memset(&pkm.blockc.nickname[length], 0xffff, 2);
	//pkm.blockb.ivs.isnicknamed = 1;
}
void setpkmotname(pokemon_obj &pkm, wchar_t input[], int length){
	if(length > 8){length = 8;}
	memset(&pkm.blockd.otname, '\0', 20);
	memcpy(&pkm.blockd.otname,input,length*2);
	memset(&pkm.blockd.otname[length], 0xffff, 2);
}
std::wstring getpkmnickname(const pokemon_obj *pkm)
{
	std::wstring nick = pkm->blockc.nickname;
	if(nick.find((wchar_t)0xffff))
	{
		nick = nick.substr(0,nick.find((wchar_t)0xffff));
	}
	return nick;
};
std::wstring getpkmotname(const pokemon_obj *pkm)
{
	std::wstring otname = pkm->blockd.otname;
	if(otname.find((wchar_t)0xffff))
	{
		otname = otname.substr(0,otname.find((wchar_t)0xffff));
	}
	return otname;
};
void setpkmnickname(pokemon_obj *pkm, wchar_t input[], int length){
	if(length > 8){length = 8;}
	memset(&pkm->blockc.nickname, '\0', 20);
	memcpy(&pkm->blockc.nickname,input,length*2);
	memset(&pkm->blockc.nickname[length], 0xffff, 2);
	//pkm.blockb.ivs.isnicknamed = 1;
}
void setpkmotname(pokemon_obj *pkm, wchar_t input[], int length){
	if(length > 8){length = 8;}
	memset(&pkm->blockd.otname, '\0', 20);
	memcpy(&pkm->blockd.otname,input,length*2);
	memset(&pkm->blockd.otname[length], 0xffff, 2);
}
Genders::genders getpkmgender(const pokemon_obj &pkm)
{
	if (pkm.blockb.forms.female){return Genders::female;};
	if (pkm.blockb.forms.genderless){return Genders::genderless;};
	return Genders::male;
}
Genders::genders getpkmgender(const pokemon_obj *pkm)
{
	if (pkm->blockb.forms.female){return Genders::female;};
	if (pkm->blockb.forms.genderless){return Genders::genderless;};
	return Genders::male;
}
void setpkmgender(pokemon_obj &pkm, int gender)
{
	switch (gender)
	{
	case Genders::male:
		pkm.blockb.forms.female = false;
		pkm.blockb.forms.genderless = false;
		break;
	case Genders::female:
		pkm.blockb.forms.female = true;
		pkm.blockb.forms.genderless = false;
		break;
	case Genders::genderless:
		pkm.blockb.forms.female = false;
		pkm.blockb.forms.genderless = true;
		break;
	}
}
bool getpkmshiny(const pokemon_obj &pkm){
	uint32 p1, p2, E, F;

	p1 = (pkm.block0.pid & 0xFFFF0000) >> 16;
	p2 = pkm.block0.pid & 0xFFFF;
	E = pkm.blocka.tid ^ pkm.blocka.sid;
	F = p1 ^ p2;

	return (E ^ F) < 8;

};
bool pkmmetasegg(const pokemon_obj &pkm){
	return ((pkm.blockd.eggdate.year != 0) & (pkm.blockd.eggdate.month != 0) & (pkm.blockd.eggdate.day != 0));
};
void setpkmgender(pokemon_obj *pkm, int gender)
{
	switch (gender)
	{
	case Genders::male:
		pkm->blockb.forms.female = false;
		pkm->blockb.forms.genderless = false;
		break;
	case Genders::female:
		pkm->blockb.forms.female = true;
		pkm->blockb.forms.genderless = false;
		break;
	case Genders::genderless:
		pkm->blockb.forms.female = false;
		pkm->blockb.forms.genderless = true;
		break;
	}
}
bool getpkmshiny(const pokemon_obj *pkm){
	uint32 p1, p2, E, F;

	p1 = (pkm->block0.pid & 0xFFFF0000) >> 16;
	p2 = pkm->block0.pid & 0xFFFF;
	E = pkm->blocka.tid ^ pkm->blocka.sid;
	F = p1 ^ p2;

	return (E ^ F) < 8;

};
bool pkmmetasegg(const pokemon_obj *pkm){
	return ((pkm->blockd.eggdate.year != 0) & (pkm->blockd.eggdate.month != 0) & (pkm->blockd.eggdate.day != 0));
};
void swap_pkm(box_obj &frombox, const int fromslot, box_obj &tobox, const int toslot)
{
	pokemon_obj frompkm, topkm;
	frompkm = frombox.pokemon[fromslot];
	topkm = tobox.pokemon[toslot];
	frombox.pokemon[fromslot] = topkm;
	tobox.pokemon[toslot] = frompkm;
}
void put_pkm(box_obj &box, const int slot, pokemon_obj &pkm, const bool isencrypted)
{
	if(! isencrypted)
	{
		encryptpkm(pkm);
	}
	box.pokemon[slot] = pkm;
}
void swap_pkm(box_obj *frombox, const int fromslot, box_obj *tobox, const int toslot)
{
	pokemon_obj frompkm, topkm;
	frompkm = frombox->pokemon[fromslot];
	topkm = tobox->pokemon[toslot];
	frombox->pokemon[fromslot] = topkm;
	tobox->pokemon[toslot] = frompkm;
}
void put_pkm(box_obj *box, const int slot, pokemon_obj *pkm, const bool isencrypted)
{
	if(! isencrypted)
	{
		encryptpkm(pkm);
	}
	box->pokemon[slot] = *pkm;
}
void remove_pkm(box_obj &box, const int slot)
{
	pokemon_obj blank = {};
	encryptpkm(blank);
	box.pokemon[slot] = blank;
}
void remove_pkm(box_obj *box, const int slot)
{
	pokemon_obj blank = {};
	encryptpkm(blank);
	box->pokemon[slot] = blank;
}
void depositpkm(bw2savblock_obj * block, const int party_slot, box_obj * box, const int box_slot)
{
	// TODO: Deposit pokemon

}
double getpkmhappiness(const pokemon_obj &pkm)
{
	return pkm.blocka.tameness / 255;
}
double getpkmhappiness(const pokemon_obj *pkm)
{
	return pkm->blocka.tameness / 255;
}
std::string getgendername(const int gender)
{
	switch(gender)
	{
	case Genders::male:
		return "Male";
	case Genders::female:
		return "Female";
	case Genders::genderless:
		return "Genderless";
	default:
		return "Male";
	}
}
std::string getpkmotgendername(const pokemon_obj &pkm)
{
	return getgendername(pkm.blockd.metlevel_otgender.otgender);
}
int gethiddenpowerpower(const pokemon_obj &pkm)
{
	int power = 0;
	int u=0,v=0,w=0,x=0,y=0,z=0;
	if((pkm.blockb.ivs.hp % 4 == 2) | (pkm.blockb.ivs.hp % 4 == 3)){u = 1;}
	if((pkm.blockb.ivs.attack % 4 == 2) | (pkm.blockb.ivs.attack % 4 == 3)){v = 1;}
	if((pkm.blockb.ivs.defense % 4 == 2) | (pkm.blockb.ivs.defense % 4 == 3)){w = 1;}
	if((pkm.blockb.ivs.speed % 4 == 2) | (pkm.blockb.ivs.speed % 4 == 3)){x = 1;}
	if((pkm.blockb.ivs.spatk % 4 == 2) | (pkm.blockb.ivs.spatk % 4 == 3)){y = 1;}
	if((pkm.blockb.ivs.spdef % 4 == 2) | (pkm.blockb.ivs.spdef % 4 == 3)){z = 1;}
	power = (int)(floor((double)(((u + 2 * v + 4 * w + 8 * x + 16 * y + 32 * z) * 40 / 63) + 30)));
	return power;
}
int gethiddenpowertype(const pokemon_obj &pkm)
{
	int type = 0;
	int a=0,b=0,c=0,d=0,e=0,f=0;
	a = (int)(!(pkm.blockb.ivs.hp % 2 == 0));
	b = (int)(!(pkm.blockb.ivs.attack % 2 == 0));
	c = (int)(!(pkm.blockb.ivs.defense % 2 == 0));
	d = (int)(!(pkm.blockb.ivs.speed % 2 == 0));
	e = (int)(!(pkm.blockb.ivs.spatk % 2 == 0));
	f = (int)(!(pkm.blockb.ivs.spdef % 2 == 0));
	type = (int)(floor((double)((a + 2 * b + 4 * c + 8 * d + 16 * e + 32 * f) * 15 / 63)));
	return hiddenpowertypes[type];
}
std::string getpkmotgendername(const pokemon_obj *pkm)
{
	return getgendername(pkm->blockd.metlevel_otgender.otgender);
}
int gethiddenpowerpower(const pokemon_obj *pkm)
{
	int power = 0;
	int u=0,v=0,w=0,x=0,y=0,z=0;
	if((pkm->blockb.ivs.hp % 4 == 2) | (pkm->blockb.ivs.hp % 4 == 3)){u = 1;}
	if((pkm->blockb.ivs.attack % 4 == 2) | (pkm->blockb.ivs.attack % 4 == 3)){v = 1;}
	if((pkm->blockb.ivs.defense % 4 == 2) | (pkm->blockb.ivs.defense % 4 == 3)){w = 1;}
	if((pkm->blockb.ivs.speed % 4 == 2) | (pkm->blockb.ivs.speed % 4 == 3)){x = 1;}
	if((pkm->blockb.ivs.spatk % 4 == 2) | (pkm->blockb.ivs.spatk % 4 == 3)){y = 1;}
	if((pkm->blockb.ivs.spdef % 4 == 2) | (pkm->blockb.ivs.spdef % 4 == 3)){z = 1;}
	power = (int)(floor((double)(((u + 2 * v + 4 * w + 8 * x + 16 * y + 32 * z) * 40 / 63) + 30)));
	return power;
}
int gethiddenpowertype(const pokemon_obj *pkm)
{
	int type = 0;
	int a=0,b=0,c=0,d=0,e=0,f=0;
	a = (int)(!(pkm->blockb.ivs.hp % 2 == 0));
	b = (int)(!(pkm->blockb.ivs.attack % 2 == 0));
	c = (int)(!(pkm->blockb.ivs.defense % 2 == 0));
	d = (int)(!(pkm->blockb.ivs.speed % 2 == 0));
	e = (int)(!(pkm->blockb.ivs.spatk % 2 == 0));
	f = (int)(!(pkm->blockb.ivs.spdef % 2 == 0));
	type = (int)(floor((double)((a + 2 * b + 4 * c + 8 * d + 16 * e + 32 * f) * 15 / 63)));
	return hiddenpowertypes[type];
}
std::string getballname(const int ball)
{
	return ballnames[ball];
}
std::string getballname(const pokemon_obj &pkm)
{
	return getballname(pkm.blockd.ball);
}
std::string getballname(const pokemon_obj *pkm)
{
	return getballname(pkm->blockd.ball);
}
std::wstring getboxname(const bw2savblock_obj *block,int boxnum)
{
	std::wstring name = block->boxnames[boxnum];
	if(name.find((wchar_t)0xffff))
	{
		name = name.substr(0,name.find((wchar_t)0xffff));
	}
	return name;
};
std::wstring getboxname(const bw2savblock_obj &block,int boxnum)
{
	std::wstring name = block.boxnames[boxnum];
	if(name.find((wchar_t)0xffff))
	{
		name = name.substr(0,name.find((wchar_t)0xffff));
	}
	return name;
};
