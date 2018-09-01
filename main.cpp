#include "ed.h"
// #include "Soundex.h"
#include "n-gram.h"
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <curses.h>

using namespace std; 


std::string default_console = "\033[m";
std::string color = "\033[35m";


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
    final.resize(4);
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



double calc(string before, string wrong, string correct, model& data){
    double num = ((-log2(data.prob_of_sentence(before))+(-log2(exp(-editDistDP(wrong, correct, wrong.length(), correct.length()))))));
    // cout << before << " " << wrong << " replaced with " << correct << endl; 
    // cout << num << endl; 
    return num;
}

unordered_map<int, string> generateCorrections(string word, unordered_map<int, string>& results) {
  // unordered_map<int,string> results; 
  int c = 0; 
  vector<char> alphabet;
  // cout << "Before alpha assign" << endl; 
  for (unsigned i = 0; i < 26; i++)
  {
    string temp = "";
    char a = ('a' + i);
    alphabet.push_back(a);
    // cout << alphabet.at(i) << endl; 
  }
  // cout << "After Alpha assign" << endl;  

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
      results[c] = newWord; 
      c++;  

    }
  }

  // cout << "AFTER adding" << endl; 

  // //Removing any one character from the word. CORRECT
  if(word.length() > 1){
    for(int i = 0; i < word.length(); ++i){
      string newWord = word;
      newWord.erase(newWord.begin() + i);
      results[c] = newWord; 
      c++;
      // cout << newWord << endl; 
      // cout << i << endl; 
    }
  }

  // cout << "After removal" << endl; 

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

      results[c] = newWord;
      c++;
      // cout << newWord << endl; 
    }
  }

  // cout << "After Transopsing" << endl; 

  //Substituting any character in the word with another character. CORRECT
  for(int i = 0; i < word.length(); ++i){
    for(int j = 0; j < alphabet.size(); ++j){
      string newWord = word;
      newWord.at(i) = alphabet.at(j);

      results[c] = newWord;
      c++;
      // cout << newWord << endl;
    }
  }

  // cout << "After substitution" << endl; 


  return results;
}

void add(string& sentence, model& data, vector<pair<double, string> >& edits, WINDOW * win, map<string, int>& words){
    // wprintw(win, "HELLOOO\n"); 
    // wprintw(win, sentence.c_str()); 
    istringstream SSin (sentence); 
    string s;
    vector<string> fulls; 
    while(SSin >> s){
      fulls.push_back(s); 
    }
    // wprintw(win, "hello\n");
    map<string, vector<Node> >::iterator it = data.maps.at(0).begin();
    vector<string> before;
    string b = ""; 
    string word;
    int vsize = 0; 
    istringstream SS (sentence); 
    // wprintw(win, "BEFORE WHILE\n"); 

    while((SS >> word) && (word != "ENDOFFILE")){

        wrefresh(win);
        if(words.find(word) == words.end()){

            unordered_multimap<string, string> lookup;

            // all words that exsist and are within - edit distance from the mispelled word
            generate(lookup, "Dictionary.txt"); 
            string sword;
            sword = Soundex(word);

            // all words with the same soundex
            vector<string> f = matching(lookup, word); 

            for(int e = 0; e < f.size(); ++e){
              double num0 = calc(b, word, f.at(e), data); 
              pair<double, std::string> p (num0, f.at(e)); 
              edits.push_back(p); 
            }

            // wprintw(win, "after calculated\n"); 

            unordered_map<int, string> ds; 
            generateCorrections(word, ds);
            // cout << ds.size() << endl; 
            // int s = ds.size(); 
            unordered_map<int, string> copy = ds; // edit distance 1
            for(int i = 0; i < copy.size(); ++i){ // generates edit distance 2
              // cout << "    " << copy.size() << endl; 
              generateCorrections(copy.at(i), ds);
              // if(i>=323){
              //   cout << "SHOULD BE DONE" << endl;  
              // }
            }
             
            // cout << ds.size() << endl;  
            vector<Node> v = data.next(before);
   
            vsize = v.size(); 
 
            for(int i = 0; i < v.size(); ++i) {
              if(words.find(v.at(i).s) != words.end()){
                  double num = calc(b, word, v.at(i).s, data); 
                  pair<double, std::string> p (num, v.at(i).s); 
                  edits.push_back(p); 
              } 
            }
            for(int j = 0; j < ds.size(); ++j) {
                if(words.find(ds.at(j)) != words.end()){
                    double num1 = calc(b, word, ds.at(j), data); 
                    pair<double, std::string> p (num1, ds.at(j)); 
                    edits.push_back(p); 
                }
            }
            wrefresh(win);
        } 
        int ans = 0;
        if(!edits.empty()){
          wrefresh(win);
            std::sort(edits.begin(), edits.end(), [](const std::pair<double, string> &left, const std::pair<double, string> &right) {
            return left.first < right.first;
            });
            start_color(); 
            init_pair(2, COLOR_MAGENTA, COLOR_WHITE);
            mvwprintw(win,0,0, b.c_str());
            wprintw(win, " ");
            wattron(win, COLOR_PAIR(2));
            wprintw(win, word.c_str());
            wattroff(win, COLOR_PAIR(2)); 
            wprintw(win, " ");
            wrefresh(win);

            for(int i = before.size() + 1; i < fulls.size(); ++i){
              wprintw(win, fulls.at(i).c_str());
              wprintw(win, " "); 
            }
            wprintw(win, "\nPlease select a number: \n"); 
            wrefresh(win); 
            for(int y = 0; y < 5; ++y){
              wprintw(win, "%d. ", y + 1);
              wprintw(win, edits.at(y).second.c_str());
              wprintw(win, "\n");
              wrefresh(win); 
            }

             
            wprintw(win, "6. Skip\n"); 
            wprintw(win, "7. Other: \n"); 
            wprintw(win, "8. Quit\n"); 
            wmove(win, 12, 0);// maybe needed? 
            wrefresh(win);

            wmove(win, 12, 0); 
            ans = getch();
            ans = ans - '0'; 
            wprintw(win, "%d\n", ans);
            wrefresh(win);  
            if(ans == 8){
              sentence = ""; 
              int j; 
              for(j = 0; j < before.size(); ++j){
                  sentence += (before.at(j) + " ");
              }
              for(int i = j; i < fulls.size(); ++i){
                  sentence += (fulls.at(i)) + " "; 
              }
              return; 
            }
            if(ans == 7){

              wprintw(win, "Enter the correct replacement: "); 
              wmove(win, 12, 0);
            wrefresh(win);
              wrefresh(win);
              string replacement = ""; 
              int ch = getch();
              while ( ch != '\n' )
              {
                  replacement.push_back( ch );
                  ch = getch();
  
              }
              wrefresh(win);

              b += " " + replacement; 
            }

            if(ans == 6){
              b += " " + word; 
            }

            if(ans != 7 && ans != 6){
              ans = ans - 1; 
            }

            wrefresh(win); 
        }
        
        if(!edits.empty() && (ans != 7 && ans != 6)){
            before.push_back(edits.at(ans).second); 
            b += " " + edits.at(ans).second;
        }
        else if (ans != 7 && ans != 6){
            before.push_back(word); 
            if(b == ""){
                b += word; 
            }
            else{
                b += " " + word;
            }
        }
        wprintw(win, "\n");
        edits.clear(); 
        wrefresh(win);

    }
    sentence = ""; 
    for(int j = 0; j < before.size(); ++j){
        sentence += (before.at(j) + " ");
    }
    wrefresh(win); 
}
void clean_string(string &s) {
  int len = s.length();
  for (int i = 0; i < len; ++i) {
    if (ispunct(s[i])){
      s.erase(i--, 1);
      len = s.size();
    }
    s[i] = (tolower(s[i])); 
  }
}


int main() {

    vector<pair<double, string> > edits; 

    string s; 
    string final; 


    model data; 
    ifstream iFS3 ("out3_.txt");
    ifstream iFS2 ("bigramIn.txt");
    ifstream iFS1 ("w1_.txt");
    ifstream iFS ("Dictionary.txt"); 
    map<string, int> words; 
    string g; 
    for(int i = 0; i < 62444; ++i){
      iFS >> g; 
      words[g] = 1; 
    }

    data.load(iFS3, iFS2, iFS1);

    iFS1.close();
    iFS2.close();
    iFS3.close();

    initscr(); 
    noecho(); 



    int height, width, x_start, y_start; 
    height = 20; 
    width = 100; 
    x_start = x_start = 0; 

    
    WINDOW *win = newwin(height, width, y_start, x_start); 

    wrefresh(win);
    refresh();  
      
    ifstream inFS; 
    
    inFS.open("misspeeling.txt"); 
    if (!inFS.is_open())
    {
        wprintw(win, "Error opening file \n");
        return 1;
    }


    wrefresh(win);

    while (getline(inFS, s))
    { 
        clean_string(s); 
        add(s, data, edits, win, words);
        final += s; 
        wrefresh(win);

    }
 
    wrefresh(win);
    wprintw(win, final.c_str()); 
    wrefresh(win);

    int c = getch(); 

    endwin(); 
    
    return 0; 
}
