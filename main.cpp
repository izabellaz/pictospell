#include "ed.h"
// #include "Soundex.h"
#include "n-gram.h"
#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <unordered_map>
#include <curses.h>

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



double calc(vector<string> before, string wrong, string correct, model& data){
    before.push_back(correct);
    int y = 1; 
    // cout << before.size() << "  " << correct << endl; 
    // cout << "Prob of Sentence Before: " <<  data.prob_of_sentence(before) << endl; 
    // cout << "Edit Distance: " << editDistDP(wrong, correct, wrong.length(), correct.length()) << endl; 
    // cout << num << endl; 
    double num = (data.prob_of_sentence(before)+(-log2(exp(-editDistDP(wrong, correct, wrong.length(), correct.length())))));
    // cout << " " << wrong << " replaced with " << correct << endl; 
    // cout << num << endl; 
    //double num = (data.prob_of_sentence(before)- log2(data.getn_p(correct)));
    //double num = (editDistDP(wrong, correct, wrong.length(), correct.length()) * data.getn_p(correct));
    // //num = -log2(num);
    //num = num + log2(editDistDP(wrong, correct, wrong.length(), correct.length())); 
    // cout << wrong << " replaced with " << correct << endl; 
    
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

//void add(string& sentence, model& data, vector<pair<double, string> >& edits, map<string, int>& words){

void add(string& sentence, model& data, vector<pair<double, string> >& edits, map<string, int>& words, map<string, string> e){
    // wprintw(win, "HELLOOO\n"); 
    //wprintw(win, sentence.c_str()); 
    ///cout << sentence << endl; 
  // cout << "3" << endl; 
    istringstream SSin (sentence); 
    string s;
    vector<string> fulls; 
    // cout << "5" << endl; 
    while(SSin >> s){
      fulls.push_back(s); 
    }
    // wprintw(win, "hello\n");
    map<string, vector<Node> >::iterator it = data.maps.at(0).begin();
    // cout << "6" << endl; 
    vector<string> before;
    string b = ""; 
    string word;
    int vsize = 0; 
    istringstream SS (sentence); 
    // wprintw(win, "BEFORE WHILE\n"); 
    // cout << "7" << endl; 
    while((SS >> word) && (word != "ENDOFFILE")){

        //wrefresh(win);
        if(words.find(word) == words.end()){
          // cout << "8 - WORD WRONG" << endl;

            unordered_multimap<string, string> lookup;

            // all words that exsist and are within - edit distance from the mispelled word
            generate(lookup, "Dictionary.txt"); 
            string sword;
            sword = Soundex(word);
            // cout << "9" << endl; 

            // all words with the same soundex
            vector<string> f = matching(lookup, word); 
            cout << f.size() << endl; 

            for(int ze = 0; ze < f.size(); ++ze){
              if(words.find(f.at(ze)) != words.end()){
                // cout << "Found " << endl; 
                double num0 = calc(before, word, f.at(ze), data); 
                pair<double, std::string> p (num0, f.at(ze)); 
                // cout << "pair contructed " << endl; 
                edits.push_back(p); 
                // cout << edits.size() << endl; 
              }
              // cout << ze << endl; 
            }
            // cout << "10" << endl; 

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
            // cout << "11" << endl; 
              
            // cout << ds.size() << endl;  
            vector<Node> v = data.next(before);
   
            vsize = v.size(); 
            //cout << vsize << endl; 
 
            for(int i = 0; i < v.size(); ++i) {
              if(words.find(v.at(i).s) != words.end()){
                  double num = calc(before, word, v.at(i).s, data); 
                  //cout << v.at(i).s << " " << num << endl; 
                  pair<double, std::string> p (num, v.at(i).s); 
                  edits.push_back(p); 
              } 
            }
            // cout << "12" << endl; 
            for(int j = 0; j < ds.size(); ++j) {
                if(words.find(ds.at(j)) != words.end()){
                    double num1 = calc(before, word, ds.at(j), data); 
                    //cout << "ED" << endl; 
                    //cout << ds.at(j) << " " << num1 << endl;
                    pair<double, std::string> p (num1, ds.at(j)); 
                    edits.push_back(p); 
                }
            }
            //wrefresh(win);
        } 
        int ans = 0;
        if(!edits.empty()){
          //wrefresh(win);
            std::sort(edits.begin(), edits.end(), [](const std::pair<double, string> &left, const std::pair<double, string> &right) {
            return left.first < right.first;
            });

            //start_color(); 
            //init_pair(2, COLOR_MAGENTA, COLOR_WHITE);
            //mvwprintw(win,0,0, b.c_str());
            cout << b; 
            //wprintw(win, " ");
            cout << " ";
            //wattron(win, COLOR_PAIR(2));
            //wprintw(win, word.c_str());
           cout << word; 
            //wattroff(win, COLOR_PAIR(2)); 
            //wprintw(win, " ");
           cout << " "; 
            //wrefresh(win);

            for(int i = before.size() + 1; i < fulls.size(); ++i){
              //wprintw(win, fulls.at(i).c_str());
              cout << fulls.at(i); 
              //wprintw(win, " "); 
              cout << " "; 
            }
            //wprintw(win, "\nPlease select a number: \n"); 
            //cout << endl << "Please select a number: " << endl; 
            //wrefresh(win); 
            // cout << "PRINTING" << endl << word << endl;
            string st = ""; 
            string tem = ""; 
            cout << endl; 
            edits.erase( unique( edits.begin(), edits.end() ), edits.end() );
            //cout << "\U0001F5E3" << endl; 
            for(int y = 0; y < 5; ++y){
                cout << y + 1 << ". "; 
                st = edits.at(y).second;
                if(e.find(st) != e.end()){
                  string code = e.find(st)->second; 
                  //cout << code << endl; 
                  cout << st + " " + code; 
                  //cout << st + " " + "\U0001F5E3" << endl;
                }
                else
                  cout << st; 

                cout << endl;  
            }

            cout << "6. Skip" << endl << "7. Other: " << endl << "8. Quit" << endl; 
              // wprintw(win, "6. Skip\n"); 
              // wprintw(win, "7. Other: \n"); 
              // wprintw(win, "8. Quit\n"); 
            //wmove(win, 12, 0);// maybe needed? 
            //wrefresh(win);

            //wmove(win, 12, 0); 
            // ans = getch();
            // ans = ans - '0'; 
            cin >> ans; 
            //wprintw(win, "%d\n", ans);
            //wrefresh(win);  
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

              //wprintw(win, "Enter the correct replacement: "); 
              cout << "Enter the correct replacement: " << endl; 
              //wmove(win, 12, 0);
              //wrefresh(win);
              //wrefresh(win);
              string replacement = ""; 
              cin >> replacement; 
              // int ch = getch();
              // while ( ch != '\n' )
             // {
               //   replacement.push_back( ch );
              //     //ch = getch();
                  // cin >> 
  // 
             // }
              //wrefresh(win);

              b += " " + replacement; 
              before.push_back(replacement);
            }

            if(ans == 6){
              b += " " + word; 
              before.push_back(word);
            }

            if(ans != 7 && ans != 6){
              ans = ans - 1; 
            }

           // wrefresh(win); 
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
        //wprintw(win, "\n");
        cout << endl; 
        edits.clear(); 
        //wrefresh(win);

    }
    sentence = ""; 
    for(int j = 0; j < before.size(); ++j){
        sentence += (before.at(j) + " ");
    }
    cout << sentence << endl; 
    //wrefresh(win); 
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

  map<string, string> e; 
e["happy"] = "\U0001F600";
e["grinning"] = "\U0001F600";
e["beaming"] = "\U0001F601";
e["smile"] = "\U0001F601";
e["funny"] = "\U0001F602";
e["laughing"] = "\U0001F602";
e["sweat"] = "\U0001F605";
e["sweating"] = "\U0001F605";
e["squint"] = "\U0001F606";
e["squinting"] = "\U0001F606";
e["wink"] = "\U0001F609";
e["winking"] = "\U0001F609";
e["blushing"] = "\U0001F60A";
e["blush"] = "\U0001F60A";
e["yum"] = "\U0001F60B";
e["savor"] = "\U0001F60B";
e["cool"] = "\U0001F60E";
e["sunglasses"] = "\U0001F60E";
e["kiss"] = "\U0001F618";
e["kisses"] = "\U0001F618";
e["hugging"] = "\U0001F917";
e["hug"] = "\U0001F917";
e["wow"] = "\U0001F929";
e["amazed"] = "\U0001F929";
e["think"] = "\U0001F914";
e["thinking"] = "\U0001F914";
e["neutral"] = "\U0001F610";
e["neutral"] = "\U0001F610";
e["expressionless"] = "\U0001F611";
e["bland"] = "\U0001F611";
e["smirk"] = "\U0001F60F";
e["smirking"] = "\U0001F60F";
e["persevering"] = "\U0001F623";
e["persever"] = "\U0001F623";
e["secret"] = "\U0001F910";
e["secretive"] = "\U0001F910";
e["hushed"] = "\U0001F62F";
e["hush"] = "\U0001F62F";
e["sleepy"] = "\U0001F62A";
e["sleepless"] = "\U0001F62A";
e["tired"] = "\U0001F62B";
e["tiring"] = "\U0001F62B";
e["sleep"] = "\U0001F634";
e["sleeping"] = "\U0001F634";
e["relieved"] = "\U0001F60C";
e["relieving"] = "\U0001F60C";
e["drool"] = "\U0001F924";
e["drooling"] = "\U0001F924";
e["unamused"] = "\U0001F612";
e["unamusing"] = "\U0001F612";
e["downcast"] = "\U0001F613";
e["downcasting"] = "\U0001F613";
e["pensive"] = "\U0001F614";
e["pensing"] = "\U0001F614";
e["confused"] = "\U0001F615";
e["confusing"] = "\U0001F615";
e["upside-down"] = "\U0001F643";
e["fliped"] = "\U0001F643";
e["slight"] = "\U0001F641";
e["slightly"] = "\U0001F641";
e["confounded"] = "\U0001F616";
e["confound"] = "\U0001F616";
e["disappointed"] = "\U0001F61E";
e["disappoint"] = "\U0001F61E";
e["worry"] = "\U0001F61F";
e["worried"] = "\U0001F61F";
e["tear"] = "\U0001F622";
e["tearing"] = "\U0001F622";
e["cry"] = "\U0001F62D";
e["crying"] = "\U0001F62D";
e["anguish"] = "\U0001F627";
e["anguished"] = "\U0001F627";
e["fear"] = "\U0001F628";
e["fearful"] = "\U0001F628";
e["weary"] = "\U0001F629";
e["wearier"] = "\U0001F629";
e["exploding"] = "\U0001F92F";
e["explode"] = "\U0001F92F";
e["grimacing"] = "\U0001F62C";
e["grimace"] = "\U0001F62C";
e["anxious"] = "\U0001F630";
e["anxiously"] = "\U0001F630";
e["scream"] = "\U0001F631";
e["screaming"] = "\U0001F631";
e["hot"] = "\U0001F975";
e["warm"] = "\U0001F975";
e["cold"] = "\U0001F976";
e["chilly"] = "\U0001F976";
e["flushed"] = "\U0001F633";
e["flush"] = "\U0001F633";
e["zany"] = "\U0001F92A";
e["silly"] = "\U0001F92A";
e["dizzy"] = "\U0001F635";
e["dead"] = "\U0001F635";
e["pout"] = "\U0001F621";
e["pouting"] = "\U0001F621";
e["angry"] = "\U0001F620";
e["mad"] = "\U0001F620";
e["cursing"] = "\U0001F92C";
e["curse"] = "\U0001F92C";
e["mask"] = "\U0001F637";
e["medical"] = "\U0001F637";
e["fever"] = "\U0001F912";
e["thermometer"] = "\U0001F912";
e["bandage"] = "\U0001F915";
e["hurt"] = "\U0001F915";
e["nausea"] = "\U0001F922";
e["nauseated"] = "\U0001F922";
e["vomit"] = "\U0001F92E";
e["barf"] = "\U0001F92E";
e["sneezing"] = "\U0001F927";
e["sneeze"] = "\U0001F927";
e["halo"] = "\U0001F607";
e["angel"] = "\U0001F607";
e["cowboy"] = "\U0001F920";
e["cowgirl"] = "\U0001F920";
e["party"] = "\U0001F973";
e["partying"] = "\U0001F973";
e["woozy"] = "\U0001F974";
e["hazy"] = "\U0001F974";
e["plead"] = "\U0001F97A";
e["pleading"] = "\U0001F97A";
e["lie"] = "\U0001F925";
e["lying"] = "\U0001F925";
e["shush"] = "\U0001F92B";
e["shushing"] = "\U0001F92B";
e["nerd"] = "\U0001F913";
e["geek"] = "\U0001F913";
e["devil"] = "\U0001F608";
e["devilish"] = "\U0001F608";
e["clown"] = "\U0001F921";
e["circus"] = "\U0001F921";
e["ogre"] = "\U0001F479";
e["monster"] = "\U0001F479";
e["goblin"] = "\U0001F47A";
e["ugly"] = "\U0001F47A";
e["skull"] = "\U0001F480";
e["bone"] = "\U0001F480";
e["ghost"] = "\U0001F47B";
e["invisible"] = "\U0001F47B";
e["alien"] = "\U0001F47D";
e["UFO"] = "\U0001F47D";
e["robot"] = "\U0001F916";
e["robotics"] = "\U0001F916";
e["poop"] = "\U0001F4A9";
e["crap"] = "\U0001F4A9";
e["see-no-evil"] = "\U0001F648";
e["see-no-evil"] = "\U0001F648";
e["hear-no-evil"] = "\U0001F649";
e["hear-no-evil"] = "\U0001F649";
e["speak-no-evil"] = "\U0001F64A";
e["speak-no-evil"] = "\U0001F64A";
e["baby"] = "\U0001F476";
e["infant"] = "\U0001F476";
e["child"] = "\U0001F9D2";
e["children"] = "\U0001F9D2";
e["boy"] = "\U0001F466";
e["son"] = "\U0001F466";
e["girl"] = "\U0001F467";
e["daughter"] = "\U0001F467";
e["adult"] = "\U0001F9D1";
e["mature"] = "\U0001F9D1";
e["man"] = "\U0001F468";
e["male"] = "\U0001F468";
e["woman"] = "\U0001F469";
e["female"] = "\U0001F469";
e["old"] = "\U0001F9D3";
e["elder"] = "\U0001F9D3";
e["grandfather"] = "\U0001F474";
e["pop"] = "\U0001F474";
e["grandma"] = "\U0001F475";
e["granny"] = "\U0001F475";
e["doctor"] = "\U0001F469";
e["nurse"] = "\U0001F469";
e["student"] = "\U0001F469";
e["graduation"] = "\U0001F469";
e["teacher"] = "\U0001F469";
e["professor"] = "\U0001F469";
e["judge"] = "\U0001F469";
e["law"] = "\U0001F469";
e["farm"] = "\U0001F469";
e["garden"] = "\U0001F469";
e["cook"] = "\U0001F469";
e["chef"] = "\U0001F469";
e["mechanic"] = "\U0001F469";
e["technician"] = "\U0001F469";
e["factory"] = "\U0001F469";
e["manufacturing"] = "\U0001F469";
e["office"] = "\U0001F469";
e["desk"] = "\U0001F469";
e["scientist"] = "\U0001F469";
e["scientists"] = "\U0001F469";
e["technologist"] = "\U0001F469";
e["computerscienctist"] = "\U0001F469";
e["singer"] = "\U0001F469";
e["rockstar"] = "\U0001F469";
e["artist"] = "\U0001F469";
e["painter"] = "\U0001F469";
e["pilot"] = "\U0001F469";
e["captain"] = "\U0001F469";
e["astronaut"] = "\U0001F469";
e["astronauts"] = "\U0001F469";
e["firefighter"] = "\U0001F469";
e["firefighters"] = "\U0001F469";
e["police"] = "\U0001F46E";
e["officer"] = "\U0001F46E";
e["detective"] = "\U0001F575";
e["investigator"] = "\U0001F575";
e["guard"] = "\U0001F482";
e["security"] = "\U0001F482";
e["construction"] = "\U0001F477";
e["contractor"] = "\U0001F477";
e["prince"] = "\U0001F934";
e["king"] = "\U0001F934";
e["princess"] = "\U0001F478";
e["queen"] = "\U0001F478";
e["headscarf"] = "\U0001F9D5";
e["hijab"] = "\U0001F9D5";
e["bearded"] = "\U0001F9D4";
e["beard"] = "\U0001F9D4";
e["tuxedo"] = "\U0001F935";
e["suit"] = "\U0001F935";
e["bride"] = "\U0001F470";
e["veil"] = "\U0001F470";
e["pregnant"] = "\U0001F930";
e["gravid"] = "\U0001F930";
e["breast-feed"] = "\U0001F931";
e["lactate"] = "\U0001F931";
e["Santa"] = "\U0001F385";
e["clause"] = "\U0001F385";
e["supervillain"] = "\U0001F9B9";
e["evil"] = "\U0001F9B9";
e["magician"] = "\U0001F9D9";
e["mage"] = "\U0001F9D9";
e["fairy"] = "\U0001F9DA";
e["pixie"] = "\U0001F9DA";
e["vampire"] = "\U0001F9DB";
e["vampires"] = "\U0001F9DB";
e["elf"] = "\U0001F9DD";
e["sprite"] = "\U0001F9DD";
e["genie"] = "\U0001F9DE";
e["genies"] = "\U0001F9DE";
e["zombie"] = "\U0001F9DF";
e["zombies"] = "\U0001F9DF";
e["no"] = "\U0001F645";
e["never"] = "\U0001F645";
e["sassy"] = "\U0001F481";
e["sass"] = "\U0001F481";
e["bowing"] = "\U0001F647";
e["bow"] = "\U0001F647";
e["facepalming"] = "\U0001F926";
e["facepalm"] = "\U0001F926";
e["shrugging"] = "\U0001F937";
e["shrug"] = "\U0001F937";
e["massage"] = "\U0001F486";
e["massages"] = "\U0001F486";
e["haircut"] = "\U0001F487";
e["haircuts"] = "\U0001F487";
e["walking"] = "\U0001F6B6";
e["walk"] = "\U0001F6B6";
e["run"] = "\U0001F3C3";
e["running"] = "\U0001F3C3";
e["dancing"] = "\U0001F483";
e["dance"] = "\U0001F483";
e["steamy"] = "\U0001F9D6";
e["steam"] = "\U0001F9D6";
e["climbing"] = "\U0001F9D7";
e["climb"] = "\U0001F9D7";
e["yoga"] = "\U0001F9D8";
e["zen"] = "\U0001F9D8";
e["bath"] = "\U0001F6C0";
e["bathing"] = "\U0001F6C0";
e["sleeping"] = "\U0001F6CC";
e["sleeps"] = "\U0001F6CC";
e["speak"] = "\U0001F5E3";
e["talk"] = "\U0001F5E3";
e["say"] = "\U0001F5E3";
e["said"] = "\U0001F5E3";
e["fencing"] = "\U0001F93A";
e["swording"] = "\U0001F93A";
e["horse"] = "\U0001F3C7";
e["racing"] = "\U0001F3C7";
e["snowboarder"] = "\U0001F3C2";
e["snowboard"] = "\U0001F3C2";
e["surfing"] = "\U0001F3C4";
e["surf"] = "\U0001F3C4";
e["rowing"] = "\U0001F6A3";
e["row"] = "\U0001F6A3";
e["swim"] = "\U0001F3CA";
e["swimming"] = "\U0001F3CA";
e["hand"] = "\U0001F590";
e["fingers"] = "\U0001F590";
e["biking"] = "\U0001F6B4";
e["pedaling"] = "\U0001F6B4";
e["sunglasses"] = "\U0001F576";
e["sunglass"] = "\U0001F576";
e["cartwheel"] = "\U0001F938";
e["cartwheels"] = "\U0001F938";
e["wrestling"] = "\U0001F93C";
e["wrestle"] = "\U0001F93C";
e["handball"] = "\U0001F93E";
e["handballing"] = "\U0001F93E";
e["juggling"] = "\U0001F939";
e["juggle"] = "\U0001F939";
e["selfies"] = "\U0001F933";
e["selfie"] = "\U0001F933";
e["flexing"] = "\U0001F4AA";
e["biceps"] = "\U0001F4AA";
e["leg"] = "\U0001F9B5";
e["legs"] = "\U0001F9B5";
e["toes"] = "\U0001F9B6";
e["foot"] = "\U0001F9B6";
e["left"] = "\U0001F448";
e["point"] = "\U0001F448";
e["shopping"] = "\U0001F6CD";
e["bags"] = "\U0001F6CD";
e["peppers"] = "\U0001F336";
e["pepper"] = "\U0001F336";
e["world"] = "\U0001F5FA";
e["map"] = "\U0001F5FA";
e["webs"] = "\U0001F578";
e["web"] = "\U0001F578";
e["camp"] = "\U0001F3D5";
e["camping"] = "\U0001F3D5";
e["islands"] = "\U0001F3DD";
e["island"] = "\U0001F3DD";
e["plates"] = "\U0001F37D";
e["plate"] = "\U0001F37D";
e["deserts"] = "\U0001F3DC";
e["desert"] = "\U0001F3DC";
e["parks"] = "\U0001F3DE";
e["park"] = "\U0001F3DE";
e["chipmunks"] = "\U0001F43F";
e["chipmunk"] = "\U0001F43F";
e["beach"] = "\U0001F3D6";
e["beachs"] = "\U0001F3D6";
e["houses"] = "\U0001F3D8";
e["house"] = "\U0001F3D8";
e["city"] = "\U0001F3D9";
e["cityscape"] = "\U0001F3D9";
e["motorways"] = "\U0001F6E3";
e["motorway"] = "\U0001F6E3";
e["stadiums"] = "\U0001F3DF";
e["stadium"] = "\U0001F3DF";
e["railway"] = "\U0001F6E4";
e["track"] = "\U0001F6E4";
e["construction"] = "\U0001F3D7";
e["construction"] = "\U0001F3D7";
e["buildings"] = "\U0001F3DB";
e["building"] = "\U0001F3DB";
e["spiders"] = "\U0001F577";
e["spider"] = "\U0001F577";
e["doves"] = "\U0001F54A";
e["dove"] = "\U0001F54A";
e["right"] = "\U0001F449";
e["pointing"] = "\U0001F449";
e["fuck"] = "\U0001F595";
e["fucked"] = "\U0001F595";
e["down"] = "\U0001F447";
e["low"] = "\U0001F447";
e["crossed"] = "\U0001F91E";
e["cross"] = "\U0001F91E";
e["call"] = "\U0001F919";
e["call"] = "\U0001F919";
e["ok"] = "\U0001F44C";
e["okay"] = "\U0001F44C";
e["good"] = "\U0001F44D";
e["thumb"] = "\U0001F44D";
e["bad"] = "\U0001F44E";
e["bad"] = "\U0001F44E";
e["punch"] = "\U0001F44A";
e["hit"] = "\U0001F44A";
e["wave"] = "\U0001F44B";
e["waving"] = "\U0001F44B";
e["clapping"] = "\U0001F44F";
e["clap"] = "\U0001F44F";
e["praising"] = "\U0001F64C";
e["praise"] = "\U0001F64C";
e["palms"] = "\U0001F932";
e["palm"] = "\U0001F932";
e["pray"] = "\U0001F64F";
e["please"] = "\U0001F64F";
e["handshakes"] = "\U0001F91D";
e["handshake"] = "\U0001F91D";
e["nail"] = "\U0001F485";
e["polish"] = "\U0001F485";
e["ear"] = "\U0001F442";
e["ears"] = "\U0001F442";
e["noses"] = "\U0001F443";
e["nose"] = "\U0001F443";
e["footprint"] = "\U0001F463";
e["footprints"] = "\U0001F463";
e["see"] = "\U0001F440";
e["eye"] = "\U0001F440";
e["brains"] = "\U0001F9E0";
e["brain"] = "\U0001F9E0";
e["bones"] = "\U0001F9B4";
e["bone"] = "\U0001F9B4";
e["teeth"] = "\U0001F9B7";
e["tooth"] = "\U0001F9B7";
e["tongues"] = "\U0001F445";
e["tongue"] = "\U0001F445";
e["mouths"] = "\U0001F444";
e["mouth"] = "\U0001F444";
e["cupid"] = "\U0001F498";
e["cupids"] = "\U0001F498";
e["beating"] = "\U0001F493";
e["loving"] = "\U0001F493";
e["broken"] = "\U0001F494";
e["heartbroken"] = "\U0001F494";
e["heart"] = "\U0001F495";
e["hearts"] = "\U0001F495";
e["sparkling"] = "\U0001F496";
e["sparkle"] = "\U0001F496";
e["blue"] = "\U0001F499";
e["blue"] = "\U0001F499";
e["green"] = "\U0001F49A";
e["green"] = "\U0001F49A";
e["yellow"] = "\U0001F49B";
e["yellow"] = "\U0001F49B";
e["orange"] = "\U0001F9E1";
e["orange"] = "\U0001F9E1";
e["purple"] = "\U0001F49C";
e["purple"] = "\U0001F49C";
e["black"] = "\U0001F5A4";
e["rotten"] = "\U0001F5A4";
e["revolving"] = "\U0001F49E";
e["revolve"] = "\U0001F49E";
e["letters"] = "\U0001F48C";
e["letter"] = "\U0001F48C";
e["sleep"] = "\U0001F4A4";
e["zzz"] = "\U0001F4A4";
e["bombs"] = "\U0001F4A3";
e["bomb"] = "\U0001F4A3";
e["collide"] = "\U0001F4A5";
e["collision"] = "\U0001F4A5";
e["water"] = "\U0001F4A6";
e["droplets"] = "\U0001F4A6";
e["dash"] = "\U0001F4A8";
e["dashing"] = "\U0001F4A8";
e["thoughts"] = "\U0001F4AD";
e["thought"] = "\U0001F4AD";
e["goggles"] = "\U0001F97D";
e["goggles"] = "\U0001F97D";
e["lab"] = "\U0001F97C";
e["lab"] = "\U0001F97C";
e["tie"] = "\U0001F454";
e["necktie"] = "\U0001F454";
e["shirt"] = "\U0001F455";
e["t-shirt"] = "\U0001F455";
e["denim"] = "\U0001F456";
e["jeans"] = "\U0001F456";
e["scarfs"] = "\U0001F9E3";
e["scarf"] = "\U0001F9E3";
e["glove"] = "\U0001F9E4";
e["gloves"] = "\U0001F9E4";
e["coats"] = "\U0001F9E5";
e["coat"] = "\U0001F9E5";
e["sock"] = "\U0001F9E6";
e["socks"] = "\U0001F9E6";
e["dresses"] = "\U0001F457";
e["dress"] = "\U0001F457";
e["kimonos"] = "\U0001F458";
e["kimono"] = "\U0001F458";
e["bikinis"] = "\U0001F459";
e["bikini"] = "\U0001F459";
e["clothes"] = "\U0001F45A";
e["clothes"] = "\U0001F45A";
e["purses"] = "\U0001F45B";
e["purse"] = "\U0001F45B";
e["handbag"] = "\U0001F45C";
e["handbags"] = "\U0001F45C";
e["clutch"] = "\U0001F45D";
e["clutch"] = "\U0001F45D";
e["backpacks"] = "\U0001F392";
e["backpack"] = "\U0001F392";
e["shoes"] = "\U0001F45F";
e["shoe"] = "\U0001F45F";
e["flat"] = "\U0001F97F";
e["flats"] = "\U0001F97F";
e["heels"] = "\U0001F460";
e["high-heeled"] = "\U0001F460";
e["sandals"] = "\U0001F461";
e["sandal"] = "\U0001F461";
e["oil"] = "\U0001F6E2";
e["oil"] = "\U0001F6E2";
e["boots"] = "\U0001F462";
e["boot"] = "\U0001F462";
e["crowns"] = "\U0001F451";
e["crown"] = "\U0001F451";
e["sunhat"] = "\U0001F452";
e["hat"] = "\U0001F452";
e["tophat"] = "\U0001F3A9";
e["hats"] = "\U0001F3A9";
e["ship"] = "\U0001F6F3";
e["ships"] = "\U0001F6F3";
e["graduation"] = "\U0001F393";
e["graduation"] = "\U0001F393";
e["boats"] = "\U0001F6E5";
e["boat"] = "\U0001F6E5";
e["cap"] = "\U0001F9E2";
e["baseballcap"] = "\U0001F9E2";
e["beads"] = "\U0001F4FF";
e["bead"] = "\U0001F4FF";
e["lipgloss"] = "\U0001F484";
e["lipstick"] = "\U0001F484";
e["rings"] = "\U0001F48D";
e["ring"] = "\U0001F48D";
e["gem"] = "\U0001F48E";
e["diamond"] = "\U0001F48E";
e["monkey"] = "\U0001F435";
e["monkeys"] = "\U0001F435";
e["goirllas"] = "\U0001F98D";
e["gorilla"] = "\U0001F98D";
e["dogs"] = "\U0001F436";
e["dog"] = "\U0001F436";
e["poodles"] = "\U0001F429";
e["poodle"] = "\U0001F429";
e["wolfs"] = "\U0001F43A";
e["wolf"] = "\U0001F43A";
e["foxes"] = "\U0001F98A";
e["fox"] = "\U0001F98A";
e["raccoons"] = "\U0001F99D";
e["raccoon"] = "\U0001F99D";
e["cats"] = "\U0001F431";
e["cat"] = "\U0001F431";
e["kitten"] = "\U0001F408";
e["kittens"] = "\U0001F408";
e["lions"] = "\U0001F981";
e["lion"] = "\U0001F981";
e["tigers"] = "\U0001F42F";
e["tiger"] = "\U0001F42F";
e["leopards"] = "\U0001F406";
e["leopard"] = "\U0001F406";
e["horses"] = "\U0001F434";
e["horse"] = "\U0001F434";
e["unicorns"] = "\U0001F984";
e["unicorn"] = "\U0001F984";
e["zebras"] = "\U0001F993";
e["zebra"] = "\U0001F993";
e["deer"] = "\U0001F98C";
e["deer"] = "\U0001F98C";
e["cows"] = "\U0001F42E";
e["cow"] = "\U0001F42E";
e["oxs"] = "\U0001F402";
e["ox"] = "\U0001F402";
e["buffalos"] = "\U0001F403";
e["buffalo"] = "\U0001F403";
e["calf"] = "\U0001F404";
e["calves"] = "\U0001F404";
e["pigglet"] = "\U0001F437";
e["pig"] = "\U0001F437";
e["pigs"] = "\U0001F416";
e["pigglets"] = "\U0001F416";
e["boars"] = "\U0001F417";
e["boar"] = "\U0001F417";
e["rams"] = "\U0001F40F";
e["ram"] = "\U0001F40F";
e["ewes"] = "\U0001F411";
e["ewe"] = "\U0001F411";
e["goats"] = "\U0001F410";
e["goat"] = "\U0001F410";
e["camels"] = "\U0001F42A";
e["camel"] = "\U0001F42A";
e["llamas"] = "\U0001F999";
e["llama"] = "\U0001F999";
e["giraffes"] = "\U0001F992";
e["giraffe"] = "\U0001F992";
e["elephants"] = "\U0001F418";
e["elephant"] = "\U0001F418";
e["rhinoceroses"] = "\U0001F98F";
e["rhinoceros"] = "\U0001F98F";
e["hippopotamuses"] = "\U0001F99B";
e["hippopotamus"] = "\U0001F99B";
e["mice"] = "\U0001F42D";
e["mouse"] = "\U0001F42D";
e["rats"] = "\U0001F400";
e["rat"] = "\U0001F400";
e["hamsters"] = "\U0001F439";
e["hamster"] = "\U0001F439";
e["rabbits"] = "\U0001F430";
e["rabbit"] = "\U0001F430";
e["hedgehogs"] = "\U0001F994";
e["hedgehog"] = "\U0001F994";
e["bats"] = "\U0001F987";
e["bat"] = "\U0001F987";
e["bears"] = "\U0001F43B";
e["bear"] = "\U0001F43B";
e["koalas"] = "\U0001F428";
e["koala"] = "\U0001F428";
e["pandas"] = "\U0001F43C";
e["panda"] = "\U0001F43C";
e["kangaroos"] = "\U0001F998";
e["kangaroo"] = "\U0001F998";
e["badgers"] = "\U0001F9A1";
e["badger"] = "\U0001F9A1";
e["paws"] = "\U0001F43E";
e["paw"] = "\U0001F43E";
e["turkey"] = "\U0001F983";
e["turkeys"] = "\U0001F983";
e["hen"] = "\U0001F414";
e["chicken"] = "\U0001F414";
e["roosters"] = "\U0001F413";
e["rooster"] = "\U0001F413";
e["chicks"] = "\U0001F423";
e["chick"] = "\U0001F423";
e["birds"] = "\U0001F426";
e["bird"] = "\U0001F426";
e["penguins"] = "\U0001F427";
e["penguin"] = "\U0001F427";
e["eagles"] = "\U0001F985";
e["eagle"] = "\U0001F985";
e["ducks"] = "\U0001F986";
e["duck"] = "\U0001F986";
e["swans"] = "\U0001F9A2";
e["swan"] = "\U0001F9A2";
e["owls"] = "\U0001F989";
e["owl"] = "\U0001F989";
e["peacocks"] = "\U0001F99A";
e["peacock"] = "\U0001F99A";
e["parrots"] = "\U0001F99C";
e["parrot"] = "\U0001F99C";
e["frogs"] = "\U0001F438";
e["frog"] = "\U0001F438";
e["alligator"] = "\U0001F40A";
e["crocodile"] = "\U0001F40A";
e["tortoise"] = "\U0001F422";
e["turtle"] = "\U0001F422";
e["lizards"] = "\U0001F98E";
e["lizard"] = "\U0001F98E";
e["snakes"] = "\U0001F40D";
e["snake"] = "\U0001F40D";
e["dragons"] = "\U0001F409";
e["dragon"] = "\U0001F409";
e["dino"] = "\U0001F995";
e["sauropod"] = "\U0001F995";
e["dinosaur"] = "\U0001F996";
e["T-Rex"] = "\U0001F996";
e["whales"] = "\U0001F433";
e["whale"] = "\U0001F433";
e["whales"] = "\U0001F40B";
e["whale"] = "\U0001F40B";
e["dolphins"] = "\U0001F42C";
e["dolphin"] = "\U0001F42C";
e["fishs"] = "\U0001F41F";
e["fish"] = "\U0001F41F";
e["blowfishs"] = "\U0001F421";
e["blowfish"] = "\U0001F421";
e["sharks"] = "\U0001F988";
e["shark"] = "\U0001F988";
e["octopi"] = "\U0001F419";
e["octopus"] = "\U0001F419";
e["shells"] = "\U0001F41A";
e["shell"] = "\U0001F41A";
e["crabs"] = "\U0001F980";
e["crab"] = "\U0001F980";
e["lobsters"] = "\U0001F99E";
e["lobster"] = "\U0001F99E";
e["shrimps"] = "\U0001F990";
e["shrimp"] = "\U0001F990";
e["squids"] = "\U0001F991";
e["squid"] = "\U0001F991";
e["snails"] = "\U0001F40C";
e["snail"] = "\U0001F40C";
e["butterflies"] = "\U0001F98B";
e["butterfly"] = "\U0001F98B";
e["bugs"] = "\U0001F41B";
e["bug"] = "\U0001F41B";
e["ants"] = "\U0001F41C";
e["ant"] = "\U0001F41C";
e["bees"] = "\U0001F41D";
e["honeybee"] = "\U0001F41D";
e["ladybug"] = "\U0001F41E";
e["ladybugs"] = "\U0001F41E";
e["crickets"] = "\U0001F997";
e["cricket"] = "\U0001F997";
e["scorpions"] = "\U0001F982";
e["scorpion"] = "\U0001F982";
e["mosquito"] = "\U0001F99F";
e["mosquito"] = "\U0001F99F";
e["microbes"] = "\U0001F9A0";
e["microbe"] = "\U0001F9A0";
e["bouquets"] = "\U0001F490";
e["bouquet"] = "\U0001F490";
e["blossoms"] = "\U0001F338";
e["blossom"] = "\U0001F338";
e["flowers"] = "\U0001F4AE";
e["flower"] = "\U0001F4AE";
e["roses"] = "\U0001F339";
e["rose"] = "\U0001F339";
e["wilt"] = "\U0001F940";
e["wilted"] = "\U0001F940";
e["hibiscus"] = "\U0001F33A";
e["hibiscus"] = "\U0001F33A";
e["sunflowers"] = "\U0001F33B";
e["sunflower"] = "\U0001F33B";
e["tulips"] = "\U0001F337";
e["tulip"] = "\U0001F337";
e["seedlings"] = "\U0001F331";
e["seedling"] = "\U0001F331";
e["evergreen"] = "\U0001F332";
e["tree"] = "\U0001F332";
e["deciduous"] = "\U0001F333";
e["deciduous"] = "\U0001F333";
e["palms"] = "\U0001F334";
e["palm"] = "\U0001F334";
e["cacti"] = "\U0001F335";
e["cactus"] = "\U0001F335";
e["sheaf"] = "\U0001F33E";
e["sheafs"] = "\U0001F33E";
e["herbs"] = "\U0001F33F";
e["herb"] = "\U0001F33F";
e["clovers"] = "\U0001F340";
e["clover"] = "\U0001F340";
e["maple"] = "\U0001F341";
e["maples"] = "\U0001F341";
e["leaf"] = "\U0001F343";
e["leaves"] = "\U0001F343";
e["grape"] = "\U0001F347";
e["grapes"] = "\U0001F347";
e["melons"] = "\U0001F348";
e["melon"] = "\U0001F348";
e["watermelons"] = "\U0001F349";
e["watermelon"] = "\U0001F349";
e["tangerines"] = "\U0001F34A";
e["tangerine"] = "\U0001F34A";
e["lemons"] = "\U0001F34B";
e["lemon"] = "\U0001F34B";
e["bananas"] = "\U0001F34C";
e["banana"] = "\U0001F34C";
e["pineapples"] = "\U0001F34D";
e["pineapple"] = "\U0001F34D";
e["mangos"] = "\U0001F96D";
e["mango"] = "\U0001F96D";
e["apple"] = "\U0001F34E";
e["apples"] = "\U0001F34E";
e["pears"] = "\U0001F350";
e["pear"] = "\U0001F350";
e["peachs"] = "\U0001F351";
e["peach"] = "\U0001F351";
e["cherry"] = "\U0001F352";
e["cherries"] = "\U0001F352";
e["strawberries"] = "\U0001F353";
e["strawberry"] = "\U0001F353";
e["kiwis"] = "\U0001F95D";
e["kiwi"] = "\U0001F95D";
e["tomatos"] = "\U0001F345";
e["tomato"] = "\U0001F345";
e["coconuts"] = "\U0001F965";
e["coconut"] = "\U0001F965";
e["avocados"] = "\U0001F951";
e["avocado"] = "\U0001F951";
e["eggplants"] = "\U0001F346";
e["eggplant"] = "\U0001F346";
e["potatos"] = "\U0001F954";
e["potato"] = "\U0001F954";
e["carrots"] = "\U0001F955";
e["carrot"] = "\U0001F955";
e["corns"] = "\U0001F33D";
e["corn"] = "\U0001F33D";
e["cucumbers"] = "\U0001F952";
e["cucumber"] = "\U0001F952";
e["lettuce"] = "\U0001F96C";
e["greens"] = "\U0001F96C";
e["broccoli"] = "\U0001F966";
e["broccoli"] = "\U0001F966";
e["mushrooms"] = "\U0001F344";
e["mushroom"] = "\U0001F344";
e["peanuts"] = "\U0001F95C";
e["peanut"] = "\U0001F95C";
e["chestnuts"] = "\U0001F330";
e["chestnut"] = "\U0001F330";
e["breads"] = "\U0001F35E";
e["bread"] = "\U0001F35E";
e["croissants"] = "\U0001F950";
e["croissant"] = "\U0001F950";
e["baguette"] = "\U0001F956";
e["baguettes"] = "\U0001F956";
e["pretzels"] = "\U0001F968";
e["pretzel"] = "\U0001F968";
e["bagels"] = "\U0001F96F";
e["bagel"] = "\U0001F96F";
e["pancake"] = "\U0001F95E";
e["pancakes"] = "\U0001F95E";
e["cheeses"] = "\U0001F9C0";
e["cheese"] = "\U0001F9C0";
e["meat"] = "\U0001F356";
e["meats"] = "\U0001F356";
e["poultry"] = "\U0001F357";
e["poultries"] = "\U0001F357";
e["steak"] = "\U0001F969";
e["steaks"] = "\U0001F969";
e["bacons"] = "\U0001F953";
e["bacon"] = "\U0001F953";
e["hamburgers"] = "\U0001F354";
e["hamburger"] = "\U0001F354";
e["fries"] = "\U0001F35F";
e["fry"] = "\U0001F35F";
e["pizzas"] = "\U0001F355";
e["pizza"] = "\U0001F355";
e["hotdog"] = "\U0001F32D";
e["hotdogs"] = "\U0001F32D";
e["sandwichs"] = "\U0001F96A";
e["sandwich"] = "\U0001F96A";
e["tacos"] = "\U0001F32E";
e["taco"] = "\U0001F32E";
e["burritos"] = "\U0001F32F";
e["burrito"] = "\U0001F32F";
e["flatbreads"] = "\U0001F959";
e["flatbread"] = "\U0001F959";
e["eggs"] = "\U0001F95A";
e["egg"] = "\U0001F95A";
e["cooked"] = "\U0001F373";
e["cooking"] = "\U0001F373";
e["pan"] = "\U0001F958";
e["pans"] = "\U0001F958";
e["pot"] = "\U0001F372";
e["pots"] = "\U0001F372";
e["bowls"] = "\U0001F963";
e["bowl"] = "\U0001F963";
e["salad"] = "\U0001F957";
e["salads"] = "\U0001F957";
e["popcorns"] = "\U0001F37F";
e["popcorn"] = "\U0001F37F";
e["salts"] = "\U0001F9C2";
e["salt"] = "\U0001F9C2";
e["rice"] = "\U0001F35A";
e["rice"] = "\U0001F35A";
e["spaghetti"] = "\U0001F35D";
e["spaghetti"] = "\U0001F35D";
e["sweetpotato"] = "\U0001F360";
e["sweetpotatos"] = "\U0001F360";
e["sushi"] = "\U0001F363";
e["sushi"] = "\U0001F363";
e["shrimps"] = "\U0001F364";
e["shrimp"] = "\U0001F364";
e["mooncake"] = "\U0001F96E";
e["mooncakes"] = "\U0001F96E";
e["dumplings"] = "\U0001F95F";
e["dumpling"] = "\U0001F95F";
e["fortunecookies"] = "\U0001F960";
e["fortunecookie"] = "\U0001F960";
e["takeout"] = "\U0001F961";
e["takeout"] = "\U0001F961";
e["icecream"] = "\U0001F366";
e["dessert"] = "\U0001F366";
e["shavedice"] = "\U0001F367";
e["shavedice"] = "\U0001F367";
e["doughnuts"] = "\U0001F369";
e["doughnut"] = "\U0001F369";
e["cookies"] = "\U0001F36A";
e["cookie"] = "\U0001F36A";
e["birthday"] = "\U0001F382";
e["cake"] = "\U0001F382";
e["shortcakes"] = "\U0001F370";
e["shortcake"] = "\U0001F370";
e["cupcakes"] = "\U0001F9C1";
e["cupcake"] = "\U0001F9C1";
e["pies"] = "\U0001F967";
e["pie"] = "\U0001F967";
e["chocolate"] = "\U0001F36B";
e["chocolate"] = "\U0001F36B";
e["candies"] = "\U0001F36C";
e["candy"] = "\U0001F36C";
e["lollipops"] = "\U0001F36D";
e["lollipop"] = "\U0001F36D";
e["custards"] = "\U0001F36E";
e["custard"] = "\U0001F36E";
e["honey"] = "\U0001F36F";
e["honey"] = "\U0001F36F";
e["bottle"] = "\U0001F37C";
e["bottles"] = "\U0001F37C";
e["milk"] = "\U0001F95B";
e["milks"] = "\U0001F95B";
e["tea"] = "\U0001F375";
e["teacup"] = "\U0001F375";
e["sake"] = "\U0001F376";
e["sake"] = "\U0001F376";
e["wine"] = "\U0001F377";
e["glass"] = "\U0001F377";
e["cocktail"] = "\U0001F378";
e["cocktails"] = "\U0001F378";
e["beers"] = "\U0001F37A";
e["beer"] = "\U0001F37A";
e["clinking"] = "\U0001F942";
e["clink"] = "\U0001F942";
e["chopstick"] = "\U0001F962";
e["chopsticks"] = "\U0001F962";
e["fork"] = "\U0001F374";
e["knife"] = "\U0001F374";
e["spoons"] = "\U0001F944";
e["spoon"] = "\U0001F944";
e["knives"] = "\U0001F52A";
e["knives"] = "\U0001F52A";
e["amphora"] = "\U0001F3FA";
e["amphora"] = "\U0001F3FA";
e["globe"] = "\U0001F30D";
e["globes"] = "\U0001F30D";
e["compasses"] = "\U0001F9ED";
e["compass"] = "\U0001F9ED";
e["volcanos"] = "\U0001F30B";
e["volcano"] = "\U0001F30B";
e["brick"] = "\U0001F9F1";
e["bricks"] = "\U0001F9F1";
e["offices"] = "\U0001F3E2";
e["office"] = "\U0001F3E2";
e["post"] = "\U0001F3E4";
e["postoffice"] = "\U0001F3E4";
e["hospitals"] = "\U0001F3E5";
e["hospital"] = "\U0001F3E5";
e["banks"] = "\U0001F3E6";
e["bank"] = "\U0001F3E6";
e["hotels"] = "\U0001F3E8";
e["hotel"] = "\U0001F3E8";
e["stores"] = "\U0001F3EA";
e["store"] = "\U0001F3EA";
e["schools"] = "\U0001F3EB";
e["school"] = "\U0001F3EB";
e["factories"] = "\U0001F3ED";
e["factory"] = "\U0001F3ED";
e["castles"] = "\U0001F3F0";
e["castle"] = "\U0001F3F0";
e["weddings"] = "\U0001F492";
e["wedding"] = "\U0001F492";
e["Tokyo"] = "\U0001F5FC";
e["tower"] = "\U0001F5FC";
e["NY"] = "\U0001F5FD";
e["Liberty"] = "\U0001F5FD";
e["mosque"] = "\U0001F54C";
e["mosques"] = "\U0001F54C";
e["synagogues"] = "\U0001F54D";
e["synagogue"] = "\U0001F54D";
e["kaaba"] = "\U0001F54B";
e["kaabas"] = "\U0001F54B";
e["foggy"] = "\U0001F301";
e["foggy"] = "\U0001F301";
e["night"] = "\U0001F303";
e["stars"] = "\U0001F303";
e["sunrise"] = "\U0001F305";
e["sunrises"] = "\U0001F305";
e["dusk"] = "\U0001F306";
e["dusk"] = "\U0001F306";
e["sunsets"] = "\U0001F307";
e["sunset"] = "\U0001F307";
e["bridge"] = "\U0001F309";
e["bridges"] = "\U0001F309";
e["milkyway"] = "\U0001F30C";
e["space"] = "\U0001F30C";
e["carousel"] = "\U0001F3A0";
e["carousel"] = "\U0001F3A0";
e["ferriswheel"] = "\U0001F3A1";
e["ferriswheels"] = "\U0001F3A1";
e["roller"] = "\U0001F3A2";
e["coaster"] = "\U0001F3A2";
e["barber"] = "\U0001F488";
e["pole"] = "\U0001F488";
e["circus"] = "\U0001F3AA";
e["tent"] = "\U0001F3AA";
e["locomotive"] = "\U0001F682";
e["locomotives"] = "\U0001F682";
e["railwaycar"] = "\U0001F683";
e["railway"] = "\U0001F683";
e["trains"] = "\U0001F686";
e["train"] = "\U0001F686";
e["metros"] = "\U0001F687";
e["metro"] = "\U0001F687";
e["stations"] = "\U0001F689";
e["station"] = "\U0001F689";
e["trams"] = "\U0001F68A";
e["tram"] = "\U0001F68A";
e["monorails"] = "\U0001F69D";
e["monorail"] = "\U0001F69D";
e["busses"] = "\U0001F68C";
e["bus"] = "\U0001F68C";
e["trolleybusses"] = "\U0001F68E";
e["trolleybus"] = "\U0001F68E";
e["minibusses"] = "\U0001F690";
e["minibus"] = "\U0001F690";
e["ambulances"] = "\U0001F691";
e["ambulance"] = "\U0001F691";
e["fireengine"] = "\U0001F692";
e["firetruck"] = "\U0001F692";
e["policecar"] = "\U0001F693";
e["policecars"] = "\U0001F693";
e["taxi"] = "\U0001F695";
e["taxi"] = "\U0001F695";
e["car"] = "\U0001F697";
e["automobile"] = "\U0001F697";
e["tractor"] = "\U0001F69C";
e["tractors"] = "\U0001F69C";
e["bicycle"] = "\U0001F6B2";
e["bicycles"] = "\U0001F6B2";
e["scooter"] = "\U0001F6F4";
e["scooters"] = "\U0001F6F4";
e["skateboard"] = "\U0001F6F9";
e["skateboards"] = "\U0001F6F9";
e["lights"] = "\U0001F6A8";
e["light"] = "\U0001F6A8";
e["traffic"] = "\U0001F6A5";
e["traffic"] = "\U0001F6A5";
e["stop"] = "\U0001F6D1";
e["sign"] = "\U0001F6D1";
e["construction"] = "\U0001F6A7";
e["construction"] = "\U0001F6A7";
e["canoe"] = "\U0001F6F6";
e["canoes"] = "\U0001F6F6";
e["speedboat"] = "\U0001F6A4";
e["speedboats"] = "\U0001F6A4";
e["airplane"] = "\U0001F6E9";
e["airplanes"] = "\U0001F6E9";
e["takeoff"] = "\U0001F6EB";
e["takeoffs"] = "\U0001F6EB";
e["landing"] = "\U0001F6EC";
e["landings"] = "\U0001F6EC";
e["seats"] = "\U0001F4BA";
e["seat"] = "\U0001F4BA";
e["helicopter"] = "\U0001F681";
e["helicopters"] = "\U0001F681";
e["cableways"] = "\U0001F6A0";
e["cableway"] = "\U0001F6A0";
e["tramway"] = "\U0001F6A1";
e["tramway"] = "\U0001F6A1";
e["satellites"] = "\U0001F6F0";
e["satellite"] = "\U0001F6F0";
e["rockets"] = "\U0001F680";
e["rocket"] = "\U0001F680";
e["saucers"] = "\U0001F6F8";
e["saucer"] = "\U0001F6F8";
e["bag"] = "\U0001F9F3";
e["luggage"] = "\U0001F9F3";
e["time"] = "\U0001F55B";
e["clock"] = "\U0001F55B";
e["moon"] = "\U0001F315";
e["moons"] = "\U0001F315";
e["crescent"] = "\U0001F319";
e["crescents"] = "\U0001F319";
e["thermometer"] = "\U0001F321";
e["temperature"] = "\U0001F321";
e["sun"] = "\U0001F31E";
e["suns"] = "\U0001F31E";
e["glowing"] = "\U0001F31F";
e["star"] = "\U0001F31F";
e["overcast"] = "\U0001F325";
e["cloudy"] = "\U0001F325";
e["rainy"] = "\U0001F327";
e["rain"] = "\U0001F327";
e["snowy"] = "\U0001F328";
e["snow"] = "\U0001F328";
e["lightning"] = "\U0001F329";
e["lightning"] = "\U0001F329";
e["cyclones"] = "\U0001F300";
e["cyclone"] = "\U0001F300";
e["rainbows"] = "\U0001F308";
e["rainbow"] = "\U0001F308";
e["fires"] = "\U0001F525";
e["fire"] = "\U0001F525";
e["waters"] = "\U0001F4A7";
e["droplet"] = "\U0001F4A7";
e["wave"] = "\U0001F30A";
e["waves"] = "\U0001F30A";
e["pumpkin"] = "\U0001F383";
e["jack-o-lantern"] = "\U0001F383";
e["Christmas"] = "\U0001F384";
e["christmas"] = "\U0001F384";
e["firework"] = "\U0001F386";
e["fireworks"] = "\U0001F386";
e["sparkler"] = "\U0001F387";
e["sparklers"] = "\U0001F387";
e["firecracker"] = "\U0001F9E8";
e["firecrackers"] = "\U0001F9E8";
e["balloon"] = "\U0001F388";
e["balloons"] = "\U0001F388";
e["party"] = "\U0001F389";
e["popper"] = "\U0001F389";
e["confetti"] = "\U0001F38A";
e["suprise"] = "\U0001F38A";
e["pine"] = "\U0001F38D";
e["pines"] = "\U0001F38D";
e["doll"] = "\U0001F38E";
e["dolls"] = "\U0001F38E";
e["carp"] = "\U0001F38F";
e["streamer"] = "\U0001F38F";
e["windchime"] = "\U0001F390";
e["chime"] = "\U0001F390";
e["envelopes"] = "\U0001F9E7";
e["envelope"] = "\U0001F9E7";
e["bows"] = "\U0001F380";
e["ribbon"] = "\U0001F380";
e["wrapped"] = "\U0001F381";
e["gift"] = "\U0001F381";
e["admission"] = "\U0001F39F";
e["tickets"] = "\U0001F39F";
e["ticket"] = "\U0001F3AB";
e["ticket"] = "\U0001F3AB";
e["military"] = "\U0001F396";
e["medals"] = "\U0001F396";
e["award"] = "\U0001F3C6";
e["trophy"] = "\U0001F3C6";
e["sports"] = "\U0001F3C5";
e["medal"] = "\U0001F3C5";
e["gold"] = "\U0001F947";
e["gold"] = "\U0001F947";
e["siliver"] = "\U0001F948";
e["siliver"] = "\U0001F948";
e["bronze"] = "\U0001F949";
e["bronze"] = "\U0001F949";
e["softballs"] = "\U0001F94E";
e["softball"] = "\U0001F94E";
e["basketball"] = "\U0001F3C0";
e["basketballs"] = "\U0001F3C0";
e["volleyball"] = "\U0001F3D0";
e["volleyballs"] = "\U0001F3D0";
e["football"] = "\U0001F3C8";
e["footballs"] = "\U0001F3C8";
e["rugby"] = "\U0001F3C9";
e["rugby"] = "\U0001F3C9";
e["tennis"] = "\U0001F3BE";
e["tennis"] = "\U0001F3BE";
e["frisbee"] = "\U0001F94F";
e["disc"] = "\U0001F94F";
e["bowling"] = "\U0001F3B3";
e["bowlings"] = "\U0001F3B3";
e["cricket"] = "\U0001F3CF";
e["cricket"] = "\U0001F3CF";
e["hockey"] = "\U0001F3D2";
e["puck"] = "\U0001F3D2";
e["lacrosse"] = "\U0001F94D";
e["lacrosse"] = "\U0001F94D";
e["ping"] = "\U0001F3D3";
e["pong"] = "\U0001F3D3";
e["badminton"] = "\U0001F3F8";
e["badminton"] = "\U0001F3F8";
e["boxing"] = "\U0001F94A";
e["boxing"] = "\U0001F94A";
e["goal"] = "\U0001F945";
e["net"] = "\U0001F945";
e["fishing"] = "\U0001F3A3";
e["fishing"] = "\U0001F3A3";
e["ski"] = "\U0001F3BF";
e["skis"] = "\U0001F3BF";
e["sleds"] = "\U0001F6F7";
e["sled"] = "\U0001F6F7";
e["direct"] = "\U0001F3AF";
e["bullseye"] = "\U0001F3AF";
e["pool"] = "\U0001F3B1";
e["8ball"] = "\U0001F3B1";
e["crystal"] = "\U0001F52E";
e["future"] = "\U0001F52E";
e["controller"] = "\U0001F3AE";
e["videogame"] = "\U0001F3AE";
e["joystick"] = "\U0001F579";
e["joystick"] = "\U0001F579";
e["gamble"] = "\U0001F3B0";
e["gambling"] = "\U0001F3B0";
e["dice"] = "\U0001F3B2";
e["die"] = "\U0001F3B2";
e["puzzle"] = "\U0001F9E9";
e["jigsaw"] = "\U0001F9E9";
e["teddy"] = "\U0001F9F8";
e["bear"] = "\U0001F9F8";
e["jokers"] = "\U0001F0CF";
e["joker"] = "\U0001F0CF";
e["performing"] = "\U0001F3AD";
e["theader"] = "\U0001F3AD";
e["framed"] = "\U0001F5BC";
e["picture"] = "\U0001F5BC";
e["paint"] = "\U0001F3A8";
e["palette"] = "\U0001F3A8";
e["threads"] = "\U0001F9F5";
e["thread"] = "\U0001F9F5";
e["knitting"] = "\U0001F9F6";
e["yarn"] = "\U0001F9F6";
e["muted"] = "\U0001F507";
e["mute"] = "\U0001F507";
e["quite"] = "\U0001F508";
e["speaker"] = "\U0001F508";
e["loud"] = "\U0001F50A";
e["loud"] = "\U0001F50A";
e["loudspeaker"] = "\U0001F4E2";
e["loudspeakers"] = "\U0001F4E2";
e["megaphone"] = "\U0001F4E3";
e["megaphones"] = "\U0001F4E3";
e["horns"] = "\U0001F4EF";
e["horn"] = "\U0001F4EF";
e["bell"] = "\U0001F514";
e["bells"] = "\U0001F514";
e["scilent"] = "\U0001F515";
e["scilence"] = "\U0001F515";
e["music"] = "\U0001F3BC";
e["musical"] = "\U0001F3BC";
e["keys"] = "\U0001F3B6";
e["notes"] = "\U0001F3B6";
e["studio"] = "\U0001F399";
e["microphones"] = "\U0001F399";
e["slider"] = "\U0001F39A";
e["slider"] = "\U0001F39A";
e["control"] = "\U0001F39B";
e["knobs"] = "\U0001F39B";
e["microphone"] = "\U0001F3A4";
e["microphone"] = "\U0001F3A4";
e["headphone"] = "\U0001F3A7";
e["headphones"] = "\U0001F3A7";
e["radio"] = "\U0001F4FB";
e["radios"] = "\U0001F4FB";
e["saxophone"] = "\U0001F3B7";
e["jazz"] = "\U0001F3B7";
e["guitar"] = "\U0001F3B8";
e["guitars"] = "\U0001F3B8";
e["keyboard"] = "\U0001F3B9";
e["piano"] = "\U0001F3B9";
e["trumpet"] = "\U0001F3BA";
e["trumpets"] = "\U0001F3BA";
e["violin"] = "\U0001F3BB";
e["violins"] = "\U0001F3BB";
e["drum"] = "\U0001F941";
e["drums"] = "\U0001F941";
e["phones"] = "\U0001F4F1";
e["phone"] = "\U0001F4F1";
e["telephone"] = "\U0001F4DE";
e["telephones"] = "\U0001F4DE";
e["pagers"] = "\U0001F4DF";
e["pager"] = "\U0001F4DF";
e["fax"] = "\U0001F4E0";
e["faxing"] = "\U0001F4E0";
e["batteries"] = "\U0001F50B";
e["battery"] = "\U0001F50B";
e["electric"] = "\U0001F50C";
e["plug"] = "\U0001F50C";
e["laptop"] = "\U0001F4BB";
e["computer"] = "\U0001F4BB";
e["desktop"] = "\U0001F5A5";
e["computers"] = "\U0001F5A5";
e["print"] = "\U0001F5A8";
e["printer"] = "\U0001F5A8";
e["trackball"] = "\U0001F5B2";
e["trackballs"] = "\U0001F5B2";
e["disk"] = "\U0001F4BD";
e["disks"] = "\U0001F4BD";
e["floppydisk"] = "\U0001F4BE";
e["floppy"] = "\U0001F4BE";
e["dvds"] = "\U0001F4C0";
e["dvd"] = "\U0001F4C0";
e["abacus"] = "\U0001F9EE";
e["abacus"] = "\U0001F9EE";
e["movie"] = "\U0001F3A5";
e["camera"] = "\U0001F3A5";
e["film"] = "\U0001F39E";
e["films"] = "\U0001F39E";
e["projector"] = "\U0001F4FD";
e["projectors"] = "\U0001F4FD";
e["action"] = "\U0001F3AC";
e["acting"] = "\U0001F3AC";
e["TV"] = "\U0001F4FA";
e["television"] = "\U0001F4FA";
e["flash"] = "\U0001F4F8";
e["flash"] = "\U0001F4F8";
e["videocassette"] = "\U0001F4FC";
e["videocassettes"] = "\U0001F4FC";
e["magnifying"] = "\U0001F50D";
e["magnify"] = "\U0001F50D";
e["candle"] = "\U0001F56F";
e["candles"] = "\U0001F56F";
e["lightbulb"] = "\U0001F4A1";
e["idea"] = "\U0001F4A1";
e["flashlight"] = "\U0001F526";
e["flashlights"] = "\U0001F526";
e["lantern"] = "\U0001F3EE";
e["lanterns"] = "\U0001F3EE";
e["textbook"] = "\U0001F4D5";
e["textbooks"] = "\U0001F4D5";
e["reading"] = "\U0001F4D6";
e["read"] = "\U0001F4D6";
e["book"] = "\U0001F4D7";
e["book"] = "\U0001F4D7";
e["books"] = "\U0001F4DA";
e["books"] = "\U0001F4DA";
e["notebooks"] = "\U0001F4D3";
e["notebook"] = "\U0001F4D3";
e["ledger"] = "\U0001F4D2";
e["ledgers"] = "\U0001F4D2";
e["page"] = "\U0001F4C3";
e["pages"] = "\U0001F4C3";
e["scrolling"] = "\U0001F4DC";
e["scroll"] = "\U0001F4DC";
e["newspaper"] = "\U0001F4F0";
e["newspapers"] = "\U0001F4F0";
e["tab"] = "\U0001F4D1";
e["tabs"] = "\U0001F4D1";
e["bookmark"] = "\U0001F516";
e["bookmarks"] = "\U0001F516";
e["label"] = "\U0001F3F7";
e["labels"] = "\U0001F3F7";
e["money"] = "\U0001F4B0";
e["cash"] = "\U0001F4B0";
e["dollar"] = "\U0001F4B5";
e["dollars"] = "\U0001F4B5";
e["cards"] = "\U0001F4B3";
e["card"] = "\U0001F4B3";
e["receipts"] = "\U0001F9FE";
e["receipt"] = "\U0001F9FE";
e["mail"] = "\U0001F4E7";
e["e-mail"] = "\U0001F4E7";
e["outbox"] = "\U0001F4E4";
e["tray"] = "\U0001F4E4";
e["inbox"] = "\U0001F4E5";
e["inbox"] = "\U0001F4E5";
e["packages"] = "\U0001F4E6";
e["package"] = "\U0001F4E6";
e["mailbox"] = "\U0001F4EB";
e["mailboxs"] = "\U0001F4EB";
e["postbox"] = "\U0001F4EE";
e["postbox"] = "\U0001F4EE";
e["ballots"] = "\U0001F5F3";
e["ballot"] = "\U0001F5F3";
e["pen"] = "\U0001F58B";
e["pens"] = "\U0001F58B";
e["paintbrush"] = "\U0001F58C";
e["paintbrushs"] = "\U0001F58C";
e["crayons"] = "\U0001F58D";
e["crayon"] = "\U0001F58D";
e["memo"] = "\U0001F4DD";
e["memos"] = "\U0001F4DD";
e["briefcase"] = "\U0001F4BC";
e["briefcases"] = "\U0001F4BC";
e["file"] = "\U0001F4C1";
e["folder"] = "\U0001F4C1";
e["files"] = "\U0001F4C2";
e["folders"] = "\U0001F4C2";
e["index"] = "\U0001F5C2";
e["dividers"] = "\U0001F5C2";
e["calendars"] = "\U0001F4C5";
e["calendar"] = "\U0001F4C5";
e["notepads"] = "\U0001F5D2";
e["notepad"] = "\U0001F5D2";
e["chart"] = "\U0001F4C8";
e["charts"] = "\U0001F4C8";
e["clipboard"] = "\U0001F4CB";
e["clipboards"] = "\U0001F4CB";
e["pushpin"] = "\U0001F4CC";
e["pushpins"] = "\U0001F4CC";
e["paperclip"] = "\U0001F4CE";
e["paperclips"] = "\U0001F4CE";
e["straight"] = "\U0001F4CF";
e["ruler"] = "\U0001F4CF";
e["triangular"] = "\U0001F4D0";
e["angle"] = "\U0001F4D0";
e["cabinets"] = "\U0001F5C4";
e["cabinet"] = "\U0001F5C4";
e["trashcan"] = "\U0001F5D1";
e["wastebasket"] = "\U0001F5D1";
e["lock"] = "\U0001F512";
e["locked"] = "\U0001F512";
e["unlocked"] = "\U0001F513";
e["unlocked"] = "\U0001F513";
e["keys"] = "\U0001F511";
e["key"] = "\U0001F511";
e["hammers"] = "\U0001F528";
e["hammer"] = "\U0001F528";
e["daggers"] = "\U0001F5E1";
e["dagger"] = "\U0001F5E1";
e["gun"] = "\U0001F52B";
e["pistol"] = "\U0001F52B";
e["arrows"] = "\U0001F3F9";
e["arrow"] = "\U0001F3F9";
e["shields"] = "\U0001F6E1";
e["shield"] = "\U0001F6E1";
e["wrench"] = "\U0001F527";
e["wrenchs"] = "\U0001F527";
e["nut"] = "\U0001F529";
e["bolt"] = "\U0001F529";
e["clamp"] = "\U0001F5DC";
e["clamps"] = "\U0001F5DC";
e["linked"] = "\U0001F517";
e["link"] = "\U0001F517";
e["tools"] = "\U0001F9F0";
e["toolbox"] = "\U0001F9F0";
e["magnets"] = "\U0001F9F2";
e["magnet"] = "\U0001F9F2";
e["science"] = "\U0001F9EA";
e["tube"] = "\U0001F9EA";
e["petri"] = "\U0001F9EB";
e["germs"] = "\U0001F9EB";
e["genetics"] = "\U0001F9EC";
e["dna"] = "\U0001F9EC";
e["micro"] = "\U0001F52C";
e["microscope"] = "\U0001F52C";
e["telescopes"] = "\U0001F52D";
e["telescope"] = "\U0001F52D";
e["satellite"] = "\U0001F4E1";
e["antenna"] = "\U0001F4E1";
e["blood"] = "\U0001F489";
e["syringe"] = "\U0001F489";
e["medicine"] = "\U0001F48A";
e["pill"] = "\U0001F48A";
e["doors"] = "\U0001F6AA";
e["door"] = "\U0001F6AA";
e["beds"] = "\U0001F6CF";
e["bed"] = "\U0001F6CF";
e["couch"] = "\U0001F6CB";
e["lamp"] = "\U0001F6CB";
e["potty"] = "\U0001F6BD";
e["toilet"] = "\U0001F6BD";
e["showers"] = "\U0001F6BF";
e["shower"] = "\U0001F6BF";
e["tub"] = "\U0001F6C1";
e["bathtub"] = "\U0001F6C1";
e["lotion"] = "\U0001F9F4";
e["lotion"] = "\U0001F9F4";
e["safety"] = "\U0001F9F7";
e["pin"] = "\U0001F9F7";
e["sweeping"] = "\U0001F9F9";
e["broom"] = "\U0001F9F9";
e["bin"] = "\U0001F9FA";
e["basket"] = "\U0001F9FA";
e["paper"] = "\U0001F9FB";
e["papers"] = "\U0001F9FB";
e["soaps"] = "\U0001F9FC";
e["soap"] = "\U0001F9FC";
e["sponges"] = "\U0001F9FD";
e["sponge"] = "\U0001F9FD";
e["extinguisher"] = "\U0001F9EF";
e["extinguishers"] = "\U0001F9EF";
e["carts"] = "\U0001F6D2";
e["cart"] = "\U0001F6D2";
e["smoking"] = "\U0001F6AC";
e["cigarette"] = "\U0001F6AC";
e["moai"] = "\U0001F5FF";
e["moai"] = "\U0001F5FF";
e["ATM"] = "\U0001F3E7";
e["ATMs"] = "\U0001F3E7";
e["litter"] = "\U0001F6AE";
e["trash"] = "\U0001F6AE";
e["bathroom"] = "\U0001F6BB";
e["restroom"] = "\U0001F6BB";
e["passports"] = "\U0001F6C2";
e["passport"] = "\U0001F6C2";
e["customs"] = "\U0001F6C3";
e["customs"] = "\U0001F6C3";
e["baggage"] = "\U0001F6C4";
e["baggage"] = "\U0001F6C4";
e["crossing"] = "\U0001F6B8";
e["crossing"] = "\U0001F6B8";
e["wrong"] = "\U0001F6AB";
e["prohibited"] = "\U0001F6AB";
e["clockwise"] = "\U0001F503";
e["rotation"] = "\U0001F503";
e["counterclockwise"] = "\U0001F504";
e["counterclockwise"] = "\U0001F504";
e["BACK"] = "\U0001F519";
e["backwards"] = "\U0001F519";
e["END"] = "\U0001F51A";
e["ends"] = "\U0001F51A";
e["ON"] = "\U0001F51B";
e["on"] = "\U0001F51B";
e["SOON"] = "\U0001F51C";
e["soon"] = "\U0001F51C";
e["TOP"] = "\U0001F51D";
e["tops"] = "\U0001F51D";
e["church"] = "\U0001F6D0";
e["worship"] = "\U0001F6D0";
e["om"] = "\U0001F549";
e["om"] = "\U0001F549";
e["menorah"] = "\U0001F54E";
e["menorah"] = "\U0001F54E";
e["jewish"] = "\U0001F52F";
e["jewish"] = "\U0001F52F";
e["shuffle"] = "\U0001F500";
e["shuffling"] = "\U0001F500";
e["repeat"] = "\U0001F501";
e["repeats"] = "\U0001F501";
e["upwards"] = "\U0001F53C";
e["upward"] = "\U0001F53C";
e["downwards"] = "\U0001F53D";
e["downward"] = "\U0001F53D";
e["cinema"] = "\U0001F3A6";
e["cinemas"] = "\U0001F3A6";
e["dims"] = "\U0001F505";
e["dim"] = "\U0001F505";
e["brights"] = "\U0001F506";
e["bright"] = "\U0001F506";
e["antenna"] = "\U0001F4F6";
e["service"] = "\U0001F4F6";
e["vibrate"] = "\U0001F4F3";
e["vibration"] = "\U0001F4F3";
e["trident"] = "\U0001F531";
e["emblem"] = "\U0001F531";
e["ten"] = "\U0001F51F";
e["10"] = "\U0001F51F";
e["hundred"] = "\U0001F4AF";
e["100"] = "\U0001F4AF";
e["uppercase"] = "\U0001F520";
e["uppercases"] = "\U0001F520";
e["lowercases"] = "\U0001F521";
e["lowercase"] = "\U0001F521";
e["number"] = "\U0001F522";
e["numbers"] = "\U0001F522";
e["symbol"] = "\U0001F523";
e["symbols"] = "\U0001F523";
e["triangle"] = "\U0001F53A";
e["triangles"] = "\U0001F53A";
e["circles"] = "\U0001F534";
e["circle"] = "\U0001F534";
e["chequered"] = "\U0001F3C1";
e["flag"] = "\U0001F3C1";
e["surrender"] = "\U0001F3F3";
e["surrenders"] = "\U0001F3F3";

    vector<pair<double, string> > edits; 

    string s; 
    string final; 


    model data; 
    ifstream iFS3 ("out3.txt");
    ifstream iFS2 ("bigramIn.txt");
    ifstream iFS1 ("w1_.txt");
    ifstream iFS ("Dictionary.txt"); 
    //ifstream ifs ("emojis.txt");

    // map<string, string> emojis;
    // string a; 
    // string b; 
    // string code; 
    // while(ifs >> code >> a >> b){
    //   clean_string(a);
    //   clean_string(b); 
    //   emojis[a] = code; 
    //   emojis[b] = code; 
    //   code = "\\" + code + " "; 
    //   cout << code; 
    // }

    data.load(iFS3, iFS2, iFS1);

    iFS1.close();
    iFS2.close();
    iFS3.close();

    string g; 
    map<string, int> words; 
    while(iFS >> g){
      // cout << g << " ";
      clean_string(g);
      words[g] = 1; 
    }
    cout << endl << words.size() << endl; 

    // map<string, int> words; 
    // for(map<string,vector<Node> >::const_iterator i=data.maps.at(0).begin();i!=data.maps.at(0).end();++i) {
    //   string g = i->first;
    //   clean_string(g); 
    //   words[g] = 1; 

    // }

    // initscr(); 
    // noecho(); 



    // int height, width, x_start, y_start; 
    // height = 20; 
    // width = 100; 
    // x_start = x_start = 0; 

    
    // WINDOW *win = newwin(height, width, y_start, x_start); 

    // wrefresh(win);
    // refresh();  
    // map<string, int> words; 
    // string g; 
    // for(map<string,vector<Node> >::const_iterator i=data.maps.at(0).begin();i!=data.maps.at(0).end();++i) {
    //         g = (i->first); 
    //         words[g] = 1; 
    // }
      
    ifstream inFS; 
    
    inFS.open("misspeeling.txt"); 
    if (!inFS.is_open())
    {
        //wprintw(win, "Error opening file \n");
        cout << "Error opening file" << endl; 
        return 1;
    }


    //wrefresh(win);
    //cout << "\U0001F5E3" << endl; 
    while (getline(inFS, s))
    { 
      // cout <<  "1" << endl; 
        clean_string(s); 
        cout << s << endl; 
        add(s, data, edits, words, e);
        // cout << "4" << endl; 
        final += s; 
        // cout << "2" << endl; 
        //wrefresh(win);

    }
 
    //wrefresh(win);
    //wprintw(win, final.c_str());
    cout << final << endl;  
    //wrefresh(win);

    //endwin(); 
    
    return 0; 
}
