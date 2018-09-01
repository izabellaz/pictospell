#ifndef N_GRAM_H
#define N_GRAM_H

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


struct Node {
  std::string s; 
  double c;
  Node(std::string x, double c) : c(c), s(x) {}
};

class model{ 
    private:

    std::vector<std::string> key; 
    std::map<std::string, int> keymap; 
    // std::vector<std::map<std::string, std::vector<Node> > > maps; // general model

    std::vector<double> lambdas; 
    // double l3; // scale rate for trigram
    // double l2; // scale rate for bigram
    // double l1; // scale rate for unigram
    // double lZ; // scale rate for non exsistant  
    
    std::pair<int, int> ints(std::string) const; 
    int find(std::vector<Node> x, std::string s) const;
    void addToMap(std::string x, std::string y);
    void calculate_p();
    void createkey();
    void n_gram(std::string x);
    void m_gram(std::string x, std::string y);
    void o_gram(std::string l, std::string y, std::string z);
    double getm_p(std::string x, std::string y, double i) const;
    double getn_p(std::string x) const;
    double geto_p(std::string xy, double i) const;
    void clean_string(std::string &s) const;
    int process(std::string filename); // returns 0 for good 1 for error with file 

    
public:
    double prob_of_sentence(std::string s) const;
    void print_o() const;
    bool find(std::string s) const;
    model(std::string name);
    model();
    double Test(std::string sentence) const;
    void save(std::ostream &os) const;
    void load(std::istream &is3, std::istream &is2, std::istream &is1);
    std::vector<Node> next(std::vector<std::string> ) const;
    void populatekey(); 
    std::vector<std::map<std::string, std::vector<Node> > > maps;
};

// class runningTest{
//     private: 

//         std::string name; 
//         int numFalse = 0; 
        
//     public:
//         std::vector<std::string> test; 
//         void setFileName(std::string filename);
//         int process();
// };

// void run(model x, model y, runningTest xt, runningTest yt);

#endif
