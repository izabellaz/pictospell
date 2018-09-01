#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include "Soundex.h"

using namespace std; 



string Soundex(string name){
    string output; 
    string final; 
    
    for(int i = 0; i < name.length(); ++i){
        char c = name[i];
        if(i == 0 ){
            output = output + c; 
        }
        if(c == 'e' || c == 'h' || c == 'a' || c == 'i' || c == 'o' || c == 'u' || c == 'w' || c == 'y'){
            continue; 
        }
        if(c == 'b' || c == 'f' || c == 'p' || c == 'v'){
            output = output + '1'; 
        }
        if(c == 'c' || c == 'g' || c == 'j' || c == 'k' || c == 'q' || c == 's' || c == 'x' || c == 'z'){
            output = output + '2'; 
        }
        if(c == 'd' || c == 't'){
            output = output + '3'; 
        }
        if(c == 'l'){
            output = output + '4'; 
        }
        if(c == 'm' || c == 'n'){
            output = output + '5'; 
        }
        if(c == 'r'){
            output = output + '6'; 
        }
    }
    for(int j = 1; j < output.length(); ++j){
        if(output[j] == output[j-1]){
            continue; 
        }
        final = final + output[j-1]; 
    }
    final = final + output[output.length() - 1];
    
    return final; 
}

void generate(unordered_multimap<string, string>& lookup, string filename){
    //unordered_multimap  == linked hash table 
    // hash function = Soundex
    // process the dictionary file to build the hash 
    // given a misspelled word hash and add to the list of edits all matching soundexs
    ifstream iFS (filename); // c++ 11 only 

    string curr; 

    while(iFS >> curr){
        string sword = Soundex(curr); 

        pair<string, string> p (sword, curr); 

        lookup.insert(p); 
    }
}

vector<string> matching(unordered_multimap<string, string>& lookup, string word){
    vector<string> thing; 
    string ans; 

    word = Soundex(word); 

    pair<unordered_multimap<string, string>::iterator, unordered_multimap<string, string>::iterator> p; 
    p = lookup.equal_range(word);

    for(unordered_multimap<string, string>::iterator it = p.first; it != p.second; ++it){
        thing.push_back(it->second); 
        // cout << it->second << endl; 
    }

    return thing; 
}

// int main(){

//     string filename = "Dictionary.txt"; 
    
//     unordered_multimap<string, string> lookup; // may or may not work?

//     generate(lookup, filename); 

//     string name = "hello"; 
    
//     name = Soundex(name);

//     vector<string> v = matching(lookup, name); 

    
//     cout << "The Soundex is: " << name << endl; 
    
//     return 0; 
// }