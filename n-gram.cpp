#include "n-gram.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <map>
#include <iterator>
#include <set>
#include <sstream>
#include <cmath>
#include <sstream>

using namespace std; 


// struct Node {
//   string s; 
//   double c;
//   Node(string x) : c(1), s(x) {}
// };

// class model{ 
//     private:
    // map<string, double> n; // unigram
    // map<string, vector<Node> > m; // bigram 
    // map<string, vector<Node> > o; // trigram
    // double l3; // scale rate for trigram
    // double l2; // scale rate for bigram
    // double l1; // scale rate for unigram
    // double lZ; // scale rate for non exsistant 
    
    void model::createkey(){
        key.resize(maps.at(0).size()); 
        int c = 0; 

        for(map<string,vector<Node> >::const_iterator i=maps.at(0).begin();i!=maps.at(0).end();++i) {
            key.at(c) = i->first; 
            keymap[i->first] = c; 
            // cout << i->first << ' ' << c; 
            c++; 

        }

        return; 
    }

    void model::populatekey(){
        ifstream iFS ("key.txt");
        int size = 0; 
        iFS >> size;
        int n;
        string s;
        key.resize(size); 
        for(int i = 0; i < size; ++i){
            iFS >> n >> s;
            key.at(n) = s;
            keymap[s] = n; 
            // cout << key.at(i).s << endl;
        }
    }

    vector<Node> model::next(vector<string> v) const { 
        // svector of srtrings for before
        string tri = ""; 
        string di = ""; 
        if(v.size() == 0){
            vector<Node> v;
            cout << "EXIT" << endl; 
            v.push_back(Node("", 0));
            return v; 
        }

        if(v.size() >= 2){
            tri = v.at(v.size()- 2) + " " + v.at(v.size() - 1); 
            //cout << "    TRI:: " << tri << endl; 
        }
        if(v.size() >= 1){
            di = v.at(v.size()-1); 
        }
        // pair<string, double> p;

        if(tri != "" && maps.at(2).find(tri) != maps.at(2).end()){
            // cout << maps.at(2).at("being a").at(0).s << endl; 
            //cout << "SIZE "<< maps.at(2).find(tri)->second.size() << endl; 
            //cout << "FOUND: in tri " << maps.at(2).find(tri)->second.at(1).s << endl;  
            return maps.at(2).find(tri)->second;
        }
        if(di != "" && maps.at(1).find(di) != maps.at(1).end()){
            cout << "FOUND: in di " << endl;  
            return maps.at(1).find(di)->second;
        }
        else{
            vector<Node> v;
            v.push_back(Node("", 0));
            return v; 
        }
    }

    pair<int, int> model::ints(string s) const{
        istringstream iSS (s);
        string one, two = "";
        iSS >> one >> two; 
        int one1 = keymap.at(one); 
        int two2 = keymap.at(two); 
        pair<int, int> ans (one1, two2);  

        // cout << ans.first << " " << ans.second << endl; 
        return ans; 
    }

    void model::save(std::ostream &os) const {

        ofstream oFS("key.txt");
        oFS << key.size() << endl; 
        for(int k = 0; k < key.size(); ++k){
            oFS << k << ' ' << key.at(k) << endl; 
        }
        cout << "Saved key" << endl; 
        oFS.close();

        os << lambdas.at(0) << ' ' << lambdas.at(1) << ' ' << lambdas.at(2) << ' ' << lambdas.at(3) << endl;
        os << maps.at(0).size() << endl;
        for(map<string,vector<Node> >::const_iterator i=maps.at(0).begin();i!=maps.at(0).end();++i) {
            os << keymap.at(i->first) << ' ' << i->second.at(0).c << endl;
            // os << i->first << ' ' << i->second.at(0).c << endl;
        }

       cout << "Saved first correct" << endl; 

        os << maps.at(1).size() << endl << endl; 
        for(map<string, vector<Node> >::const_iterator i2=maps.at(1).begin();i2 != maps.at(1).end(); ++i2){ 
            os << i2->second.size() << endl; 
            for(int q = 0; q < i2->second.size(); ++q){
                os << keymap.at(i2->first) << ' ' << keymap.at(i2->second.at(q).s) << ' ' << i2->second.at(q).c << endl;
                // os << i2->first << ' ' << i2->second.at(q).s << ' ' << i2->second.at(q).c << endl;
            }
        }

        cout << "Saved second correct" << endl; 

        os << maps.at(2).size() << endl << endl; 
        for(map<string, vector<Node> >::const_iterator i3= maps.at(2).begin();i3 != maps.at(2).end(); ++i3){ 
            os << i3->second.size() << endl; 
            for(int p = 0; p < i3->second.size(); ++p){ 
                // cout << i3->first << ' ' << i3->second.at(p).s << endl; 
                pair<int, int> pairy = this->ints(i3->first); 
                os << pairy.first <<' ' << pairy.second << ' ' << keymap.at(i3->second.at(p).s) << ' ' << i3->second.at(p).c << endl;
                // os << i3->first << ' ' << i3->second.at(p).s << ' ' << i3->second.at(p).c << endl;
            }
            // cout << "END OF OUTER LOOP" << endl; 
        }
        cout << "Saved third correct" << endl; 

    }
    void model::load(istream &is3, istream &is2, istream &is1){

        int size3 = 244045; // 1020006 entries
        int size2 = 47802; // 1020385 entries
        int size1 = 5000; 
        
        //load in unigram  
        string x;
        string y; 
        string z; 
        string t;
        double trash; 
        double num; 
        double total1 = 3.29795e+08; 
        map<string, vector<Node> > temp; 
        for(int i = 0; i < size1; ++i){
            is1 >> trash; // number ordering the list = trash
            is1 >> x;
            is1 >> t; // part of speech = trash 
            is1 >> num; // frequency 
            is1 >> trash;  
            num = num / total1;
            temp[x].push_back(Node(x, num)); 
            //cout << x << " " << num << endl; 
        }
   
        maps[0] = (temp); 

       

        //Bigram

        int vs; 
        double total2; 

        string b;
        string c; 

        double num1;
        map<string, vector<Node> > temp1; 

        for(int j = 0; j < size2 + 2; ++j){

            is2 >> vs;
            //cout << vs << endl; 
            is2 >> total2; 
            //cout << total2 << endl; 

            is2 >> num1;
            //cout << num1 << endl; 
            is2 >> b; 
            //cout << b << endl; 
            is2 >> c; 
            //cout << c << endl; 

            num1 = num1/total2; 
            
            temp1[b].push_back(Node(c, num1)); 
            //cout << b << " " << c << " " << num1 << endl; 
            vs = vs - 1; 
            if(vs > 0){
                for(int k = 0; k < vs; ++k){

                    is2 >> num1;
                    is2 >> b;
                    is2 >> c;
                    num1 = num1/total2; 
                    temp1[b].push_back(Node(c, num1)); 
                    //cout << b << " " << c << " " << num1 << endl; 
                }
            }
        }

        maps[1] = (temp1);

 


        // Trigram 
        int vss = 0;
        string d, f, g, h; 
        double num2 = 0; 
        double total3 = 0; 

        map<string, vector<Node> > temp2;  

 
        for(int l = 0; l < size3 + 2; ++l){ 
            is3 >> vss;
            is3 >> total3; 
            is3 >> num2;

            is3 >> d;
            is3 >> f;
            is3 >> h; 
            g = d + " " + f;
            //cout << vss << endl << total3 << endl; 
            //cout << d << " " << f << " " << h << " " << num2 << endl; 
 

            num2 = num2/total3; 
            temp2[g].push_back(Node(h, num2)); 


            if(vss > 1){

                for(int p = 0; p < vss - 1; ++p){
                    is3 >> num2;
                    is3 >> d;
                    is3 >> f;
                    is3 >> h; 
                    num2 = num2/total3; 
                    //cout << d << " " << f << " " << h << " " << num2 << endl; 
                    
                    temp2[g].push_back(Node(h, num2));

                }
                d = ""; 
                f = "";
                g = "";
                h = "";
            }
        }
        

        maps[2] = (temp2);
        //cout << maps.at(0).size() << "  " << maps.at(1).size() << "  " << maps.at(2).size() << endl; 
 
    }
    
    int model::find(vector<Node> x, string s) const {
        for(int i = 0; i < x.size(); ++i){
            if(x.at(i).s == s){
                return i; 
            }
        } 
        return -1; 
    }
    void model::addToMap(string x, string y){
        maps.at(1)[x].push_back(Node(y, 1));
        //m.at(x).push_back(Node(y));
        return; 
    }
    void model::calculate_p(){
        // cout << "BIGRAM:" << endl;  
        map<string, vector<Node> >::iterator it = maps.at(1).begin();
        while(it != maps.at(1).end()){
            int nwds = 0;
            // cout << it->first << endl; 
            for (vector<Node>::iterator vi= it->second.begin();
                vi!=it->second.end(); ++vi) {
                    nwds += vi->c;
                }
            for(vector<Node>::iterator vi = it->second.begin();
                vi!=it->second.end(); ++vi){
                    vi->c /= nwds;
                    // cout << "    " << vi->s << "    " << vi->c << endl; 
                }
    
            ++it;
        }
        // cout << "DONE WITH CALC P" << endl; 
        int size = maps.at(0).size(); 
        // cout << "UNIGRAM:" << endl; 
        map<string, vector<Node> >::iterator i = maps.at(0).begin(); 
        while(i != maps.at(0).end()){
            i->second.at(0).c /= size; 
            // cout << size << endl; 
            // cout << i->first << "    " << i->second << endl; 
            ++i;
        }
        // cout << "TRIGRAM:" << endl; 
        map<string, vector<Node> >::iterator t = maps.at(2).begin();
        while(t != maps.at(2).end()){
            int nwds = 0;
            // cout << t->first << endl; 
            for (vector<Node>::iterator vi= t->second.begin();
                vi!=t->second.end(); ++vi) {
                    nwds += vi->c;
                }
            for(vector<Node>::iterator vi = t->second.begin();
                vi!=t->second.end(); ++vi){
                    vi->c /= nwds;
                    // cout << "    " << vi->s << "    " << vi->c << endl; 
            }
        ++t;
        }
        // cout << "END OF CALC P" << endl; 
    }
    void model::n_gram(string x){
        // cout << "IN FUNCTION" << endl; 
        if(maps.at(0).find(x) != maps.at(0).end()){
            // cout << "already" << endl; 
            maps.at(0).find(x)->second.at(0).c += 1; 
            // cout << "already exsists" << endl; 
        }
        else { 
            // cout << "IN ELSE" << endl;
            maps.at(0)[x].push_back(Node(x, 1));
            // cout << "New Created" << endl; 
        }
        
        return; 
    } // called in process
    void model::m_gram(string x, string y){
        if(maps.at(1).find(x) != maps.at(1).end()){ // if x is in the map tree
            int r = find(maps.at(1).find(x)->second, y); 
            if (r != -1){
                maps.at(1).find(x)->second.at(r).c += 1; 
                return; 
            }
            else{ 
                maps.at(1)[x].push_back(Node(y,1)); 
                return; 
            }
        }
        else { 
            maps.at(1)[x];
            maps.at(1).at(x).push_back(Node(y,1));
            return; 
        }
    } // called in process
    void model::o_gram(string l, string y, string z){
        string x = l + " " + y; 
        // cout << x << endl; 
        if(maps.at(2).find(x) != maps.at(2).end()){ // if x is in the map tree
            int r = find(maps.at(2).find(x)->second, z); 
            if (r != -1){
                maps.at(2).find(x)->second.at(r).c += 1; 
                // cout << "      " <<  o.find(x)->second.at(r).s << " ++ 1" << endl; 
                return; 
            }
            else { 
                maps.at(2)[x].push_back(Node(z,1)); 
                // cout << "      " << z << endl; 
                // cout << "X IS: " << x << endl; 
                // cout << "Z IS: " << z << endl; 
                return; 
            }
        }
        else { 
            maps.at(2)[x];
            maps.at(2).at(x).push_back(Node(z,1));
            // cout << "      " << z << endl; 
            
            
            return; 
        }
    } // called in process
    double model::getm_p(string x, string y, double i) const {
        if(i != -1)    
            return maps.at(1).find(x)->second.at(i).c; 
        else 
            return 0; 
    }
    double model::getn_p(string x) const {
        if(maps.at(0).find(x) != maps.at(0).end())
            return maps.at(0).find(x)->second.at(0).c;
        else
            return 0; 
    }
    double model::geto_p(string xy, double i) const {
        if(i != -1){
            return (maps.at(2).find(xy)->second.at(i).c); 
        }
        else 
            return 0; 
    }
    void model::clean_string(string &s) const {
        int len = s.length();
        for (int i = 0; i < len; ++i) {
            if (ispunct(s[i]))
            {
                // if(s[i] == '.'){
                //     s[i] = "</s> <s>";
                // }
                // else
                s.erase(i--, 1);
                len = s.size();
            }
            s[i] = (tolower(s[i])); 
        }
    }
    int model::process(string filename){
        string s1;
        string s2; 
        string s3; 
        ifstream inFS; 
       
       inFS.open(filename); 
       if (!inFS.is_open())
       {
          cout << "Error opening file " <<endl;
          return 1;
       }
       // cout << "HELLO" << endl; 
       inFS >> s1;
       inFS >> s2; 
       clean_string(s1);
       clean_string(s2);
       while (inFS >> s3)
       { 
          clean_string(s3);

          n_gram(s1);
          m_gram(s1, s2);
          o_gram(s1, s2, s3); 
      

          s1 = s2;
          s2 = s3;
       }
       n_gram(s1);
       n_gram(s2);
       m_gram(s1, s2);
       inFS.close();
        return 0; 
    }
    double model::prob_of_sentence(std::vector<std::string> s) {
        //clean_string(s); 
        //cout << s.size() << endl; 
        double product = 0;
        double num0 = 0;
        double num1 = 0; 
        double num2 = 0; 
        double num3 = 0; 
        double num = 0; 
        //istringstream SS (s); 
        string x; 
        string y;
        string z; 
        int oi = -1;
        int mi = -1; 
        
        if(s.size() >= 3){
            x = s.at(s.size() - 3);
            // cout << x << " "; 
            y = s.at(s.size() - 2); 
            // cout << y << " ";
            z = s.at(s.size() - 1);
            // cout << z << " " << endl;
            string xy = x + " " + y;
            if(maps.at(1).find(x) != maps.at(1).end())
                mi = find(maps.at(1).find(x)->second, y); 
            if(maps.at(2).find(xy) != maps.at(2).end())
                oi = find(maps.at(2).find(xy)->second, z); 
            num0 = lambdas.at(0); 
            num1 = lambdas.at(1) * getn_p(x);
            num2 = lambdas.at(2) * getm_p(x, y, mi); 
            num3 = lambdas.at(3) * geto_p(xy, oi); 
            num = num0 + num1 + num2 + num3;
            num = log2(num) * -1; 
            return num;  
        }

        else if(s.size() == 2){
            x = s.at(s.size()-2);
            y = s.at(s.size()-1);
            //string xy = x + " " + y;
            if(maps.at(1).find(x) != maps.at(1).end())
                mi = find(maps.at(1).find(x)->second, y); 
            num0 = lambdas.at(0); 
            num1 = lambdas.at(1) * getn_p(x);
            num2 = lambdas.at(2) * getm_p(x, y, mi); 
            num = num0 + num1 + num2;
            num = log2(num) * -1; 
            return num; 
        }

        else if(s.size()==1){
            y = s.at(0);
            num0 = lambdas.at(0); 
            num1 = lambdas.at(1) * getn_p(y);
            num = num0 + num1;
            // cout << num0 << "   " << num1  << endl; 
            num = log2(num) * -1; 
            return num; 
        }
        else {
            product = lambdas.at(0);
            product = log2(product) * -1; 
            return product; 
        }

    // cout << product << "    " << num << endl;   
       // cout << product << endl;
        return product; 
    }
    void model::print_o() const {
        // map<string, vector<Node> >::iterator t = o.begin();
        // while(t != o.end()){
            // int nwds = 0;
            // cout << t->first << endl; 
            // for (vector<Node>::iterator vi= t->second.begin();
            //     vi!=t->second.end(); ++vi) {
            //         nwds += vi->c;
            //     }
        //     for(vector<Node>::iterator vi = t->second.begin();
        //         vi!=t->second.end(); ++vi){
        //             // vi->c /= nwds;
        //             // cout << "    " << vi->s << "    " << vi->c << endl; 
        //     }
        // ++t;
        // }
    }
    bool model::find(string s) const {
        if(maps.at(0).find(s) != maps.at(0).end()){
            return true;
              
        }
        return false;

    }

    model::model(string name){
        lambdas.resize(4);
        lambdas.at(0) = 0.001; // scale rate for blank
        lambdas.at(1) = 0.44; // scale rate for uni
        lambdas.at(2) = 0.39; // scale rate for bi
        lambdas.at(3) = 0.17; // tri
        maps.resize(3);
        this->process(name); 
        this->calculate_p();
        this->createkey(); 

    }
    model::model(){
        lambdas.resize(4);
        lambdas.at(0) = 0.001;
        lambdas.at(1) = 0.44; // scale rate for uni
        lambdas.at(2) = 0.39; // scale rate for bigram
        lambdas.at(3) = 0.17; // scale rate for trigram
        maps.resize(3);
    }


    double model::Test(string sentence) const { 
        
        //double p = prob_of_sentence(sentence); 

      //  return p;
        return 0; 
        
    }
    

//         void runningTest::setFileName(string filename){
//             name = filename;
//         }
//         int runningTest::process(){
//             ifstream inFS; 
//             string s1; 
//             // string s2; 

//             inFS.open(name); 
//             if (!inFS.is_open())
//             {
//                 cout << "Error opening file " <<endl;
//                 return 1;
//             }
    
//             for(int i = 0; i < 100; ++i){

//                 getline(inFS, s1);
//                 // cout << s1 << endl;
//                 test.push_back(s1);
//             }
//            inFS.close();
//            return 0; 
//         }
// // };

// void run(model x, model y, runningTest xt, runningTest yt){ // alice - pp
//      int numFalse = 0;
//         int xcount = 0; 
//         int ycount = 0;
//     for(int i = 0; i < 100; ++i){

//         // cout << "HELLO" << endl; 

//         // cout << num1 << "    " << num2 << endl;

        
//         double num1 = x.Test(yt.test.at(i)); // should be higher
//         double num2 = y.Test(yt.test.at(i)); // should be lower
//         // cout << num1 << "    " << num2 << endl;
//         if(num1 < num2){
//             numFalse++;
//             ycount++; 
//             // cout << yt.test.at(i) << endl;
//             // cout << "PP ided as alice" << endl;
//             // cout << num1 << "    " << num2 << endl;
            
//         }
//                 // cout << xt.test.at(i) << endl; 
//         num1 = x.Test(xt.test.at(i)); // should be lower
//         num2 = y.Test(xt.test.at(i)); // should be higher
//         if(num1 > num2){
//             numFalse++;
//             xcount++;
//             // cout << xt.test.at(i) << endl;
//             // cout << "Alice ided as pp" << endl;
//             // cout << num1 << "    " << num2 << endl;
//         }
//     }
//     cout << numFalse << endl; 
//     cout << xcount << " on the first test set." << endl; 
//     cout << ycount << " on the second test set." << endl; 
// }


// int main () { 
    
//     string alicetxt = "WarandPeace.txt"; 
//     string pptxt = "BleakHouse.txt"; 
    

//     model pp;
//     model alice; 
    
 
//     // cout << "REVERSE ALPHA" << endl; 
//     pp.make(pptxt); 
//     // cout << "ALPHA" << endl;
//     alice.make(alicetxt);

//     // double ls[] = {0.5};//{0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};

   
//     // // int i = 0;
//     // for(double l : ls){

    
//     // cout << l1 << ", " << l2 << ", " << l3 << ", " << l4 << endl; 
    
   
//     pp.SetLamdas(0.17, 0.29, 0.639, 0.001);
//     alice.SetLamdas(0.17, 0.29, 0.639, 0.001); // lambdas set
    

//     runningTest p; 
//     runningTest a; 
    
//     // a.setFileName("aliceTestSet.txt");
//     // p.setFileName("austenTestSet.txt");
 
//     // cout << "REVERSE ALPHA" << endl; 
//     p.setFileName("BleakHouseTest.txt");
//     // cout << "ALPHA" << endl;
//     a.setFileName("WarandPeaceTest.txt");
    

//     // cout << "REVERSE ALPHA Test Set" << endl; 
//     p.process();
//     // cout << "ALPHA Test Set" << endl; 
//     a.process();
    
//     run(alice, pp, a, p);
    
//  return 0;    
// }