#include<string>
#include<map>
#include<vector>
#include<utility>

#include "tables.cpp"

using namespace std;

struct instruction{
    int address;   
    string label;
    string mnemonic;
    string operand;
    string obj_code;
    int length;

    instruction(){
        this->address=-0x1;
        this->label="";
        this->mnemonic="";
        this->operand="";
        this->obj_code="";
        this->length=0;
    }

    instruction(string label,string mnemonic,string operand){
        this->address=-0x1;
        this->label=label;
        this->mnemonic=mnemonic;
        this->operand=operand;
        this->obj_code="";
        this->length=0;

    }
};

struct csect
{
    string name;
    int start;
    int end;
    bool valid_base;
    map<string,int> def;
    vector<string> ref;
    map<string, info_literal> lit_tab;  
	vector<info_mod> mod_record;  
    vector<instruction> instructions;
	map<string, info_symbol> sym_tab;
    csect(string name, int init_locctr){
        this->start=init_locctr;
        this->name=name;
        this->valid_base=0;
    }
};