#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>

using namespace std; 


vector<string> generateCorrections(string word) {
  vector<string> results; 
  vector<char> alphabet;
  cout << "Before alpha assign" << endl; 
  for (unsigned i = 0; i < 26; i++)
  {
    string temp = "";
    char a = ('a' + i);
    alphabet.push_back(a);
    // cout << alphabet.at(i) << endl; 
  }
  cout << "After Alpha assign" << endl;  

  //Adding any one character (from the alphabet) anywhere in the word. CORRECT
  for(int i = 0; i < word.length(); ++i){
    for(int j = 0; j < alphabet.size(); ++j){
      string newWord = word;
      char s = newWord.at(i); 
      // cout << s << " " << alphabet.at(j) << endl;
      string s1 = "";
      s1.push_back(alphabet.at(j)); 
      newWord.insert(i, s1);
      // cout << newWord << endl; 
      results.push_back(newWord);
    }
  }

  cout << "AFTER adding" << endl; 

  // //Removing any one character from the word. CORRECT
  if(word.length() > 1){
    for(int i = 0; i < word.length(); ++i){
      string newWord = word;
      newWord.erase(newWord.begin() + i);
      results.push_back(newWord);
      // cout << newWord << endl; 
      // cout << i << endl; 
    }
  }

  cout << "After removal" << endl; 

  // //Transposing (switching) the order of any two adjacent characters in a word. CORRECT
  if(word.length() > 1){
    for(int i = 0; i < word.length() - 1; ++i){
      string newWord = word;
      // string r = newWord.splice(i,1);
      // newWord.splice(i + 1, 0, r[0]);
      char r = newWord.at(i);
      // cout << r << endl;  
      char s = newWord.at(i + 1);
      // cout << s << endl;
      newWord.erase(newWord.begin() + i);
      newWord.erase(newWord.begin() + i); 
      // cout << newWord << endl; 
      string newS;
      newS.push_back(s);
      newS.push_back(r); 
      // cout << newS << endl; 
      newWord.insert(i, newS); 
      results.push_back(newWord);
      // cout << newWord << endl; 
    }
  }

  cout << "After Transopsing" << endl; 

  //Substituting any character in the word with another character. CORRECT
  for(int i = 0; i < word.length(); ++i){
    for(int j = 0; j < alphabet.size(); ++j){
      string newWord = word;
      newWord.at(i) = alphabet.at(j);
      results.push_back(newWord);
      // cout << newWord << endl;
    }
  }

  cout << "After substitution" << endl; 


  return results;
}


int main(){

  string s = "hello";
  
  cout << "HELLO" << endl; 

  vector<string> v; 
  v = generateCorrections(s); 

  

  // cout << "Before Print" << endl; 

  // for(int i = 0; i < v.size(); ++i){
  //   cout << v.at(i) << endl; 
  // }
  // cout << v.size() << endl; 


  return 0; 
}