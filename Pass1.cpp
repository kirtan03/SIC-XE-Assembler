#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<cstdio>
#include<map>
#include<vector>

#include "control_section.cpp"
#include "utils.cpp"

using namespace std;

vector<csect> csections;

ifstream myfile;
ofstream error;
ofstream intermediate;

void pass1(){
    cout << "Running Pass1" << endl;
    vector<string> input;
    string line;
    myfile.open("input.txt");
    if(!myfile.is_open()){
        cout<<"ERROR: File not found"<<endl;
        exit(0);
    }
    error.open("error.txt");
    if(!error.is_open()){
        cout<<"ERROR: File not found"<<endl;
        exit(0);
    }
    while(getline(myfile,line)){
        if(line!="")
            input.push_back(line);
    }
    int locctr=0;
    vector<instruction> instructions;
    vector<pair<string,info_literal>> literals;
    for(int i=0;i<input.size();i++){
        int count=0;
        vector<string> words = get_words(input[i]);
        string label = words[0];
        string mnemonic = words[1];
        string operand = words[2];
        instruction inst(label,mnemonic,operand);
        if(csections.empty()){
            csect section = csect(label,locctr);
            csections.push_back(section);
        }
        if(!csections.back().instructions.empty()){
            inst.address = csections.back().instructions.back().address;
        }
        else{
            inst.address = locctr;
        }
        csections.back().instructions.push_back(inst);
        inst = csections.back().instructions.back();
        if(mnemonic=="START"){
            if(is_hex(operand)){
                locctr = str_to_hex(operand);
            }
            else{
                error << "ERROR: Invalid operand for START" << endl;
                exit(0);
            }
            csections.back().instructions.back().address = locctr;
            csections.back().start = locctr;
        }
        else if(mnemonic=="END"){
            csections.back().instructions.pop_back();
            while(!literals.empty()){
                pair<string,info_literal> lit = literals.back();
                literals.pop_back();
                lit.second.address = locctr;
                csections.back().instructions.push_back(instruction("*",lit.first,""));
                csections.back().instructions.back().address = locctr;
                csections.back().lit_tab[lit.first] = lit.second;
                locctr += lit.second.length;
            }
            csections.back().end=locctr;
            csections.back().instructions.push_back(inst);
            break;
        }
        else if(mnemonic=="BYTE"){
            csections.back().instructions.back().address = locctr;
            if(operand[0]=='X'){
                locctr += (operand.length()-2)/2;
                csections.back().instructions.back().length = (operand.length()-2)/2;
            }
            else{
                locctr += operand.length()-3;
                csections.back().instructions.back().length = operand.length()-3;
            }
        }
        else if(mnemonic=="WORD"){
            csections.back().instructions.back().address = locctr;
            csections.back().instructions.back().length = 3;
            locctr += 3;
        }
        else if(mnemonic=="RESW"){
            csections.back().instructions.back().address = locctr;
            if(is_number(operand)){
                csections.back().instructions.back().length = 3*str_to_dec(operand);
                locctr += 3*str_to_dec(operand);
            }
            else{
                error<<"Line "<<i+1<<" : Invalid Operand For RESW Statement";
                exit(0);
            }
        }
        else if(mnemonic=="RESB"){
            csections.back().instructions.back().address = locctr;
            if(is_number(operand)){
                csections.back().instructions.back().length = str_to_dec(operand);
                locctr += str_to_dec(operand);
            }
            else{
                error<<"Line "<<i+1<<" : Invalid Operand For RESB Statement";
                exit(0);
            }
        }
        else if(mnemonic=="BASE"){
            csections.back().valid_base=1;
            continue;
        }
        else if(mnemonic=="NOBASE"){
            csections.back().valid_base=0;
            continue;
        }
        else if(mnemonic=="LTORG"){
            while(!literals.empty()){
                pair<string,info_literal> lit = literals.back();
                literals.pop_back();
                lit.second.address = locctr;
                csections.back().instructions.push_back(instruction("*",lit.first,""));
                csections.back().instructions.back().address = locctr;
                csections.back().instructions.back().length = lit.second.length;
                csections.back().lit_tab[lit.first] = lit.second;
                locctr += lit.second.length;
            }
            continue;
        }
        else if(mnemonic=="EQU"){
            vector<string> params = find_parameters(operand);
            int plus=0,minus=0;
            int value=0;
            for(int j=0;j<params.size();j++){
                if(params[j][0]=='+'){
                    if(params[j].substr(1,params[j].length()-1)=="*"){
                        plus++;
                        value += locctr;
                    }
                    else if(is_number(params[j].substr(1,params[j].length()-1))){
                        value += str_to_dec(params[j].substr(1,params[j].length()-1));
                    }
                    else if(csections.back().sym_tab.find(params[j].substr(1,params[j].length()-1))==csections.back().sym_tab.end()){
                        error<<"Line "<<i+1<<" : Symbol Not Found : "<<(params[j].substr(1,params[j].length()-1));
                        exit(0);
                    }
                    else{
                        if(!csections.back().sym_tab[params[j].substr(1,params[j].length()-1)].absolute){
                            plus++;
                        }
                        value += csections.back().sym_tab[params[j].substr(1,params[j].length()-1)].address;
                    }
                }
                else if(params[j][0]=='-'){
                    if(params[j].substr(1,params[j].length()-1)=="*"){
                        minus++;
                        value -= locctr;
                    }
                    else if(is_number(params[j].substr(1,params[j].length()-1))){
                            value -= str_to_dec(params[j].substr(1,params[j].length()-1));
                    }
                    else if(csections.back().sym_tab.find(params[j].substr(1,params[j].length()-1))==csections.back().sym_tab.end()){
                        error<<"Line "<<i+1<<" : Symbol Not Found : "<<(params[j].substr(1,params[j].length()-1));
                        exit(0);
                    }
                    else{
                        if(!csections.back().sym_tab[params[j].substr(1,params[j].length()-1)].absolute){
                            minus++;
                        }
                        value -= csections.back().sym_tab[params[j].substr(1,params[j].length()-1)].address;
                    }
                }
            }
            if((plus-minus)!=0 && (plus-minus)!=1){
                error<<"Line "<<i+1<<" : Invalid Expression For EQU Statement";
                exit(0);
            }
            if(label!="" && csections.back().sym_tab.find(label)==csections.back().sym_tab.end()){
                csections.back().sym_tab[label] = info_symbol(value);
            }
            else if(label!="" && csections.back().sym_tab.find(label)!=csections.back().sym_tab.end()){
                error<<"ERROR: Duplicate Symbol"<<endl;
                exit(0);
            }
            if((plus-minus)==1){
                csections.back().sym_tab[label].absolute = false;
            }
            else{
                csections.back().sym_tab[label].absolute = true;
            }
            continue;
        }
        else if(mnemonic=="ORG"){
            continue;
        }
        else if(mnemonic=="CSECT"){
            while(!literals.empty()){
                pair<string,info_literal> lit = literals.back();
                literals.pop_back();
                lit.second.address = locctr;
                csections.back().instructions.push_back(instruction("*",lit.first,""));
                csections.back().instructions.back().address = locctr;
                csections.back().instructions.back().length = lit.second.length;
                csections.back().lit_tab[lit.first] = lit.second;
                locctr += lit.second.length;
            }
            csections.back().end=locctr;
            csect section = csect(label,0);
            csections.push_back(section);
            locctr=0;
            continue;
        }
        else if(mnemonic=="EXTDEF"){
            continue;
        }
        else if(mnemonic=="EXTREF"){
            vector<string> refs = split_comma(operand);
            for(string s:refs){
                csections.back().ref.push_back(s);
            }
            continue;
        }
        else if(mnemonic==""){
            continue;
        }
        else if(mnemonic[0]=='+'){
            csections.back().instructions.back().address = locctr;
            csections.back().instructions.back().length = 4;
            locctr += 4;
        }
        else if(label =="."){
            csections.back().instructions.pop_back();
            continue;
        }
        else if(OPTAB.find(mnemonic)!=OPTAB.end()){
            csections.back().instructions.back().address = locctr;
            csections.back().instructions.back().length = OPTAB[mnemonic].format;
            locctr += OPTAB[mnemonic].format;
        }
        else{
            error<<"Line "<<i+1<<" in "<<csections.back().name<<" : Invalid Operation Code: " << mnemonic << "\n";
            exit(0);
        }
        if(label!="" && csections.back().instructions.back().address!=-0x1){    //condition for symbol                
            if(label!="" && csections.back().sym_tab.find(label)==csections.back().sym_tab.end()){
                csections.back().sym_tab[label] = info_symbol(csections.back().instructions.back().address);
            }
            else if(label!="" && csections.back().sym_tab.find(label)!=csections.back().sym_tab.end()){
                error<<"ERROR: Duplicate Symbol"<<endl;
                exit(0);
            }
        }
        if(operand[0]=='='){
            if(csections.back().lit_tab.find(operand)==csections.back().lit_tab.end()){
                csections.back().lit_tab[operand] = info_literal();
                if(operand[1]=='X'){
                    csections.back().lit_tab[operand].length = (operand.length()-3)/2;
                    if(is_hex(operand.substr(3,operand.length()-4))){
                        csections.back().lit_tab[operand].value = str_to_hex(operand.substr(3,operand.length()-4));
                    }
                    else{
                        error<<"Line "<<i+1<<" in "<<csections.back().name<<" : Invalid Hexadecimal Literal: " << operand << "\n";
                        exit(0);
                    }
                }
                else{
                    csections.back().lit_tab[operand].length = operand.length()-4;
                    csections.back().lit_tab[operand].value = str_to_ascii(operand.substr(3,operand.length()-4));
                }
                literals.push_back({operand,csections.back().lit_tab[operand]});
            }
        }
    }
    error.close();
    // print symtab in sym_tab.txt
    ofstream symtab("sym_tab.txt");
    for(csect c:csections){
        symtab<<"Symbol Table for "<<c.name<<'\n';
        symtab<<"\tSymbol\tAddress\tRel/Abs\n";
        for(auto it = c.sym_tab.begin(); it!=c.sym_tab.end(); it++){
            symtab<<'\t'<<it->first<<"\t";
            if(it->first.length()<3)
                symtab<<"\t";
            symtab<<to_hex(it->second.address,6)<<"\t";
            if(it->second.absolute){
                symtab<<"Abs"<<"\n";
            }
            else{
                symtab<<"Rel\n";
            }
        }
        symtab<<'\n';
    }
    symtab.close();
    // print lit_tab in lit_tab.txt
    ofstream lit_tab("lit_tab.txt");
    for(csect c:csections){
        lit_tab<<"Literal Table for "<<c.name<<'\n';
        lit_tab<<"\tLiteral\tAddress\tLength\tValue\n";
        for(auto it = c.lit_tab.begin(); it!=c.lit_tab.end(); it++){
            lit_tab<<'\t'<<it->first<<"\t";
            if(it->first.length()<4)
                lit_tab<<"\t";
            lit_tab<<to_hex(it->second.address,6)<<"\t"<<it->second.length;
            if(it->second.length<100)
                lit_tab<<"\t";
            lit_tab<<"\t"<<it->second.value<<"\n";
        }
        lit_tab<<'\n';
    }
    lit_tab.close();
    intermediate.open("intermediate.txt");
    int N = csections.size();

    // print instructions and their address in intermediate.txt
    for(int _i=0;_i<N;_i++){
        intermediate<<csections[_i].name<<"\n";
        for(int j=0;j<csections[_i].instructions.size();j++){
            instruction inst = csections[_i].instructions[j];
            intermediate<<pad(to_hex(inst.address,6),6) << " "<<pad(inst.label,6)<<" "<<pad(inst.mnemonic,6)<<" "<<pad(inst.operand,6)<<" ";           
        }
    }
    intermediate.close();
    cout<<"Pass1 Executed Successfully\n";
}
