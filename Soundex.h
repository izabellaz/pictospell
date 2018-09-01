#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <unordered_map>
#include <string>
#include <iostream>




std::string Soundex(string name); // takes string returns soundex // hash 
void generate(unordered_multimap<string, string>& lookup, string filename);
vector<string> matching(unordered_multimap<string, string>& lookup, string word);


#endif 