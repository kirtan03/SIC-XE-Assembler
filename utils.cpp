#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<cstdio>
#include<map>
#include<vector>

using namespace std;

// function to convert decimal string to integer
int str_to_dec(string s){
    if(s[0]=='-'){
        int ans = 0;
        int b = 1;
        for (int i = s.length() - 1; i >= 1; i--) {
            ans += (s[i] - '0') * b;
            b *= 10;
        }
        return -ans;
    }
    int ans = 0;
    int b = 1;
    for (int i = s.length() - 1; i >= 0; i--) {
        ans += (s[i] - '0') * b;
        b *= 10;
    }
    return ans;
}

// function to convert hexadecimal string to integer
int str_to_hex(string s){
    if(s[0]=='-'){
        int ans = 0;
        int b = 1;
        for (int i = s.length() - 1; i >= 1; i--) {
            if(s[i]>='0' && s[i]<='9'){
                ans += (s[i] - '0') * b;
            }
            else if(s[i]>='A' && s[i]<='F'){
                ans += (s[i] - 'A' + 10) * b;
            }
            else if(s[i]>='a' && s[i]<='f'){
                ans += (s[i] - 'a' + 10) * b;
            }
            b *= 16;
        }
        return -ans;
    }
    int ans = 0;
    int b = 1;
    for (int i = s.length() - 1; i >= 0; i--) {
        if(s[i]>='0' && s[i]<='9'){
            ans += (s[i] - '0') * b;
        }
        else if(s[i]>='A' && s[i]<='F'){
            ans += (s[i] - 'A' + 10) * b;
        }
        else if(s[i]>='a' && s[i]<='f'){
            ans += (s[i] - 'a' + 10) * b;
        }
        b *= 16;
    }
    return ans;
}

// function to find if a string is a valid decimal number
bool is_number(string str){
    if(str.length()==0) return 0;
    for(int i = 0; i < str.length(); i++){
        if(!isdigit(str[i])) return 0;
    }
    return 1;
}

// function to determine if a string is a valid hexadecimal number
bool is_hex(string str){
    if(str.length()==0) return 0;
    for(int i = 0; i < str.length(); i++){
        if(!isdigit(str[i]) && !(str[i] >= 'A' && str[i] <= 'F') && !(str[i] >= 'a' && str[i]<='f')) return 0;
    }
    return 1;
}

// Function to convert string to ASCII integer
int str_to_ascii(string s)
{
    int ans = 0;
    int b = 1;
    for (int i = s.length()-1; i >= 0; i--) {
        char ch = s[i];
        int tmp = (int)ch;
        ans += tmp * b;
        b *= 256;
    }
    return ans;
}

// function that get 3 parameters from an instruction line
vector<string> get_words(string line){
    vector<string> words(3,"");
    string word="";
    int ind=0;
    if(line[0]==' '){
        ind++;
    }
    for(int i=0;i<line.length();i++){
        if(line[i]==' ' && word!=""){
            words[ind] = word;
            word="";
            ind++;
            if(ind==3){
                return words;
            }
        }
        else if(line[i]!=' '){
            word+=line[i];
        }
    }
    words[ind] = word;
    return words;
}

// function that finds parameters in operand of equ statement and gives error
vector<string> find_parameters(string s){
    int n=s.length();
    vector<string> ans;
    string curr="+";
    for(int i=0;i<n;i++){
        if(s[i]=='+' || s[i]=='-'){
            ans.push_back(curr);
            curr="";
        }
        curr += s[i];
    }
    ans.push_back(curr);
    return ans;
}

// function to convert integer to hexadecimal string of given length
string to_hex(int num, int len){
    string hex = "";
    if(num<0){
            map<int, char> m;
     
        char digit = '0';
        char c = 'A';
     
        for (int i = 0; i <= 15; i++) {
            if (i < 10) {
                m[i] = digit++;
            }
            else {
                m[i] = c++;
            }
        }
        string res = "";
        if (!num) {
            return "0";
        }
        if (num > 0) {
            while (num) {
                res = m[num % 16] + res;
                num /= 16;
            }
        }
        else {
            unsigned int n = num;
            while (n) {
                res = m[n % 16] + res;
                n /= 16;
            }
        }
        res = res.substr(res.length()-len,len);
        return res;
    }
    while(num){
        int rem = num % 16;
        if(rem < 10){
            hex = (char)(rem + '0')+hex;
        }
        else{
            hex = (char)(rem - 10 + 'A')+hex;
        }
        num /= 16;
    }
    while(hex.length() < len){
        hex = '0'+hex;
    }
    return hex;
}

// function to pad a string with spaces to make it of given length
string pad(string s, int len){
    while(s.length() < len){
        s = s+' ';
    }
    return s;
}

// function to split a string according to comma if present
vector<string> split_comma(string str){
    vector<string> v;
    string temp = "";
    for(int i = 0; i < str.length(); i++){
        if(str[i] == ','){
            v.push_back(temp);
            temp = "";
        }
        else{
            temp += str[i];
        }
    }
    v.push_back(temp);
    return v;
}
