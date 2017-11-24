#include "kind.h"
#include "lexer.h"
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <map>
#include <sstream>
// Use only the neeeded aspects of each namespace
using namespace ASM;
using std::map;
using std::pair;
using std::cout;
using std::string;
using std::vector;
using std::endl;
using std::cerr;
using std::cin;
using std::getline;
using ASM::Token;
using ASM::Lexer;
using std::istringstream;

// helper to print out binary numbers
void shift(int word) {
    putchar((word>>24) & 0xff);
    putchar((word>>16) & 0xff);
    putchar((word>>8) & 0xff);
    putchar(word & 0xff);
}

int main(int argc, char* argv[]){
    // Nested vector representing lines of Tokens
    // Needs to be used here to cleanup in the case
    // of an exception
    vector< vector<Token*> > tokLines;
    // label/address table
    map<string, int> table;
    //address
    int address = 0;
    // indicator for error
    int error = 0;

    try{
        // Create a MIPS recognizer to tokenize
        // the input lines
        Lexer lexer;
        // Tokenize each line of the input
        string line;
        while(getline(cin,line)){
            tokLines.push_back(lexer.scan(line));
        }

        // Iterate over the lines of tokens and print them
        // to standard error
        vector<vector<Token*> >::iterator it;
        for(it = tokLines.begin(); it != tokLines.end(); ++it){
            vector<Token*>::iterator it2;
            int indicator = 0;
            for(it2 = it->begin(); it2 != it->end(); ++it2){
                Kind kind = (*it2)->getKind();
                if (kind == Kind::DOTWORD) {
                    if (indicator) {
                        cerr << "ERROR" << endl;
                        error = 1;
                        break;
                    }
                    indicator = 1;
                    ++it2;
                    if (it2 == it->end()) {
                        cerr << "ERROR: Lack of Number or ID" << endl;
                        error = 1;
                        break;
                    }
                    Kind kind2 = (*it2)->getKind();
                    if (kind2 == Kind::INT || kind2 == Kind::HEXINT) {
                        address += 4;
                        //                        int num = (*it2)->toInt();
                        //                        cout << char(num >> 24);
                        //                        cout << char(num >> 16);
                        //                        cout << char(num >> 8);
                        //                        cout << char(num);
                    } else if (kind2 == Kind::ID) {
                        address += 4;
                    } else {
                        cerr << "ERROR: Invalid Number" << endl;
                        error = 1;
                        break;
                    }
                } else if (kind == Kind::ID) {
                    if (indicator) {
                        cerr << "ERROR" << endl;
                        error = 1;
                        break;
                    }
                    indicator = 1;
                    string lex = (*it2)->getLexeme();
                    if (lex == "jr" || lex == "jalr") {
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: Lack of Register" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind_2 = (*it2)->getKind();
                        if (kind_2 != Kind::REGISTER) {
                            cerr << "ERROR: NOT A REGISTER" << endl;
                            error = 1;
                            break;
                        } else {
                            string lexer = (*it2)->getLexeme();
                            string snum = lexer.substr(1, 2);
                            istringstream iss {snum};
                            int num;
                            iss >> num;
                            if (num < 0 || num > 31) {
                                cerr << "ERROR: INVALID REGISTER" << endl;
                                error = 1;
                                break;
                            }
                            address += 4;
                        }
                    } else if (lex == "add" || lex == "sub" || lex == "slt" || lex == "sltu") {
                        // $d
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: Lack of Register" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind_2 = (*it2)->getKind();
                        if (kind_2 != Kind::REGISTER) {
                            cerr << "ERROR: NOT A REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        string lexer2 = (*it2)->getLexeme();
                        string strd = lexer2.substr(1, 2);
                        istringstream iss2 {strd};
                        int d;
                        iss2 >> d;
                        if (d < 0 || d > 31) {
                            cerr << "ERROR: INVALID REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        // COMMA
                        ++it2;
                        if (it2 == it->end() || (*it2)->getKind() != Kind::COMMA) {
                            cerr << "ERROR: LACK OF COMMA" << endl;
                            error = 1;
                            break;
                        }
                        // $s
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: Lack of Register" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind_3 = (*it2)->getKind();
                        if (kind_3 != Kind::REGISTER) {
                            cerr << "ERROR: NOT A REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        string lexer3 = (*it2)->getLexeme();
                        string strs = lexer3.substr(1, 2);
                        istringstream iss3 {strs};
                        int s;
                        iss3 >> s;
                        if (s < 0 || s > 31) {
                            cerr << "ERROR: INVALID REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        // COMMA
                        ++it2;
                        if (it2 == it->end() || (*it2)->getKind() != Kind::COMMA) {
                            cerr << "ERROR: << LACK OF COMMA" << endl;
                            error = 1;
                            break;
                        }
                        // $t
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: Lack of Register" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind_4 = (*it2)->getKind();
                        if (kind_4 != Kind::REGISTER) {
                            cerr << "ERROR: NOT A REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        string lexer4 = (*it2)->getLexeme();
                        string strt = lexer4.substr(1, 2);
                        istringstream iss4 {strt};
                        int t;
                        iss4 >> t;
                        if (t < 0 || t > 31) {
                            cerr << "ERROR: INVALID REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        address += 4;
                    } else if (lex == "beq" || lex == "bne") {
                        // $s
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: Lack of Register" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind_3 = (*it2)->getKind();
                        if (kind_3 != Kind::REGISTER) {
                            cerr << "ERROR: NOT A REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        string lexer3 = (*it2)->getLexeme();
                        string strs = lexer3.substr(1, 2);
                        istringstream iss3 {strs};
                        int s;
                        iss3 >> s;
                        if (s < 0 || s > 31) {
                            cerr << "ERROR: INVALID REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        // COMMA
                        ++it2;
                        if (it2 == it->end() || (*it2)->getKind() != Kind::COMMA) {
                            cerr << "ERROR: LACK OF COMMA" << endl;
                            error = 1;
                            break;
                        }
                        // $t
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: Lack of Register" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind_4 = (*it2)->getKind();
                        if (kind_4 != Kind::REGISTER) {
                            cerr << "ERROR: NOT A REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        string lexer4 = (*it2)->getLexeme();
                        string strt = lexer4.substr(1, 2);
                        istringstream iss4 {strt};
                        int t;
                        iss4 >> t;
                        if (t < 0 || t > 31) {
                            cerr << "ERROR: INVALID REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        // COMMA
                        ++it2;
                        if (it2 == it->end() || (*it2)->getKind() != Kind::COMMA) {
                            cerr << "ERROR: LACK OF COMMA" << endl;
                            error = 1;
                            break;
                        }
                        // i
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: LACK OF NUMBER" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind5 = (*it2)->getKind();
                        if (kind5 != Kind::INT && kind5 != Kind::HEXINT && kind5 != Kind::ID) {
                            cerr << "ERROR: NOT A NUMBER" << endl;
                            error = 1;
                            break;
                        }
                        if (kind5 == Kind::INT || kind5 == Kind::HEXINT) {
                            int i = (*it2)->toInt();
                            if (kind5 == Kind::INT && (i < -32768 || i > 32767)) {
                                cerr << "ERROR: Branch offset out of range" << endl;
                                error = 1;
                                break;
                            }
                            if (kind5 == Kind::HEXINT && (i > 0xffff)) {
                                cerr << "ERROR: Branch offset out of range" << endl;
                                error = 1;
                                break;
                            }
                        }
                        address += 4;
                    } else if (lex == "lis" || lex == "mflo" || lex == "mfhi") {
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: Lack of Register" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind_2 = (*it2)->getKind();
                        if (kind_2 != Kind::REGISTER) {
                            cerr << "ERROR: NOT A REGISTER" << endl;
                            error = 1;
                            break;
                        } else {
                            string lexer = (*it2)->getLexeme();
                            string snum = lexer.substr(1, 2);
                            istringstream iss {snum};
                            int num;
                            iss >> num;
                            if (num < 0 || num > 31) {
                                cerr << "ERROR: INVALID REGISTER" << endl;
                                error = 1;
                                break;
                            }
                            address += 4;
                        } 
                    } else if (lex == "mult" || lex == "multu" ||
                            lex == "div" || lex == "divu") {
                        // $s
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: Lack of Register" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind_3 = (*it2)->getKind();
                        if (kind_3 != Kind::REGISTER) {
                            cerr << "ERROR: NOT A REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        string lexer3 = (*it2)->getLexeme();
                        string strs = lexer3.substr(1, 2);
                        istringstream iss3 {strs};
                        int s;
                        iss3 >> s;
                        if (s < 0 || s > 31) {
                            cerr << "ERROR: INVALID REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        // COMMA
                        ++it2;
                        if (it2 == it->end() || (*it2)->getKind() != Kind::COMMA) {
                            cerr << "ERROR: << LACK OF COMMA" << endl;
                            error = 1;
                            break;
                        }
                        // $t
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: Lack of Register" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind_4 = (*it2)->getKind();
                        if (kind_4 != Kind::REGISTER) {
                            cerr << "ERROR: NOT A REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        string lexer4 = (*it2)->getLexeme();
                        string strt = lexer4.substr(1, 2);
                        istringstream iss4 {strt};
                        int t;
                        iss4 >> t;
                        if (t < 0 || t > 31) {
                            cerr << "ERROR: INVALID REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        address += 4;
                    } else if (lex == "lw" || lex == "sw") {
                        // $t
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: LACK OF REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind_2 = (*it2)->getKind();
                        if (kind_2 != Kind::REGISTER) {
                            cerr << "ERROR: NOT A REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        string lexer2 = (*it2)->getLexeme();
                        string strt = lexer2.substr(1, 2);
                        istringstream iss2 {strt};
                        int t;
                        iss2 >> t;
                        if (t < 0 || t > 31) {
                            cerr << "ERROR: INVALID REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        // COMMA
                        ++it2;
                        if (it2 == it->end() || (*it2)->getKind() != Kind::COMMA) {
                            cerr << "ERROR: LACK OF COMMA" << endl;
                            error = 1;
                            break;
                        }
                        // i
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: LACK OF NUMBER FOR LW/SW" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind_3 = (*it2)->getKind();
                        if (kind_3 != Kind::INT && kind_3 != Kind::HEXINT) {
                            cerr << "ERROR: NOT A NUMBER FOR LW/SW" << endl;
                            error = 1;
                            break;
                        }
                        int i = (*it2)->toInt();
                        if (kind_3 == Kind::INT && (i < -32768 || i > 32767)) {
                            cerr << "ERROR: lw/sw offset out of range" << endl;
                            error = 1;
                            break;
                        }
                        if (kind_3 == Kind::HEXINT && i > 0xffff) {
                            cerr << "ERROR: lw/sw offset out of range" << endl;
                            error = 1;
                            break;
                        }
                        // lparen
                        ++it2;
                        if(it2 == it->end() || (*it2)->getKind() != Kind::LPAREN) {
                            cerr << "ERROR: LACK OF LPAREN" << endl;
                            error = 1;
                            break;
                        }
                        // $s
                        ++it2;
                        if (it2 == it->end()) {
                            cerr << "ERROR: LACK OF REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        Kind kind_4 = (*it2)->getKind();
                        if (kind_4 != Kind::REGISTER) {
                            cerr << "ERROR: NOT A REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        string lexer4 = (*it2)->getLexeme();
                        string strs = lexer4.substr(1, 2);
                        istringstream iss4 {strs};
                        int s;
                        iss4 >> s;
                        if (s < 0 || s > 31) {
                            cerr << "ERROR: INVALID REGISTER" << endl;
                            error = 1;
                            break;
                        }
                        // RPAREN
                        ++it2;
                        if (it2 == it->end() || (*it2)->getKind() != Kind::RPAREN) {
                            cerr << "ERROR: LACK OF RPAREN" << endl;
                            error = 1;
                            break;
                        }
                        address += 4;
                    }


                    else {
                        cerr << "ERROR: INVALID ID" << endl;
                        error = 1;
                        break;
                    }


                } else if (kind == Kind::LABEL) {
                    if (indicator) {
                        cerr << "ERROR: Label cannot be defined at the end of a line" << endl;
                        error = 1;
                        break;
                    }
                    string lexeme = (*it2)->getLexeme();
                    int sz = lexeme.size();
                    lexeme.resize(sz-1);
                    if (table.find(lexeme) == table.end()) {
                        pair <string, int> p = make_pair(lexeme, address);
                        table.insert(p);
                    } else {
                        cerr << "ERROR: Repeated Definition" << endl;
                        error = 1;
                        break;
                    }
                } else {
                    cerr << "ERROR: Invalid Instruction" << endl;
                    error = 1;
                    break;
                }
            }
        }
    } catch(const string& msg){
        // If an exception occurs print the message and end the program
        cerr << msg << endl;
    }
    int address1 = 0;
    if (!error) {
        vector<vector<Token*> >::iterator it;
        for(it = tokLines.begin(); it != tokLines.end(); ++it){
            vector<Token*>::iterator it2;
            for(it2 = it->begin(); it2 != it->end(); ++it2){
                Kind kind = (*it2)->getKind();
                if (kind == Kind::DOTWORD) {
                    ++it2;
                    Kind kind2 = (*it2)->getKind();
                    if (kind2 == Kind::INT || kind2 == Kind::HEXINT) {
                        address1 += 4;
                        int num = (*it2)->toInt();
                        shift(num);
                    } else if (kind2 == Kind::ID) {
                        string id = (*it2)->getLexeme();
                        if (table.find(id) == table.end()) {
                            cerr << "ERROR: No Definition for " << id << endl;
                            error = 1;
                            break;
                        } else {
                            address1 += 4;
                            int n = table.find(id)->second;
                            shift(n);
                        }
                    } else {
                        cerr << "ERROR: Invalid Operand" << endl;
                        error = 1;
                        break;
                    }
                } else if (kind == Kind::ID) {
                    string lexer = (*it2)->getLexeme();
                    if (lexer == "jr" || lexer == "jalr") {
                        address1 += 4;
                        ++it2;
                        string lexer2 = (*it2)->getLexeme();
                        string snum = lexer2.substr(1,2);
                        istringstream iss {snum};
                        unsigned int reg;
                        iss >> reg;
                        if (lexer == "jr") {
                            int word = (reg<<21) |8;
                            shift(word);
                        } else { 
                            int word = (reg<<21) |9;
                            shift(word);
                        }
                    } else if (lexer == "add" || lexer == "sub"
                            || lexer == "slt" || lexer == "sltu") {
                        address1 += 4;
                        // $d
                        ++it2;
                        string lexer2 = (*it2)->getLexeme();
                        string strd = lexer2.substr(1, 2);
                        istringstream iss2 {strd};
                        unsigned int d;
                        iss2 >> d;
                        // COMMA
                        ++it2;
                        // $s
                        ++it2;
                        string lexer3 = (*it2)->getLexeme();
                        string strs = lexer3.substr(1, 2);
                        istringstream iss3 {strs};
                        unsigned int s;
                        iss3 >> s;
                        // COMMA
                        ++it2;
                        // $t
                        ++it2;
                        string lexer4 = (*it2)->getLexeme();
                        string strt = lexer4.substr(1, 2);
                        istringstream iss4 {strt};
                        unsigned int t;
                        iss4 >> t;
                        if (lexer == "add") {
                            int word = (s<<21) | (t<<16) | (d<<11) | 32;
                            shift(word);
                        } else if (lexer == "sub") {
                            int word = (s<<21) | (t<<16) | (d<<11) | 34;
                            shift(word);
                        } else if (lexer == "slt") {
                            int word = (s<<21) | (t<<16) | (d<<11) |42;
                            shift(word);
                        } else {
                            int word = (s<<21) | (t<<16) | (d<<11) | 43;
                            shift(word);
                        }
                    } else if (lexer == "beq" || lexer == "bne") {
                        // $s
                        ++it2;
                        string lexer3 = (*it2)->getLexeme();
                        string strs = lexer3.substr(1, 2);
                        istringstream iss3 {strs};
                        unsigned int s;
                        iss3 >> s;
                        // COMMA
                        ++it2;
                        // $t
                        ++it2;
                        string lexer4 = (*it2)->getLexeme();
                        string strt = lexer4.substr(1, 2);
                        istringstream iss4 {strt};
                        unsigned int t;
                        iss4 >> t;
                        // COMMA
                        ++it2;
                        // i
                        ++it2;
                        Kind kind5 = (*it2)->getKind();
                        if (kind5 == Kind::INT || kind5 == Kind::HEXINT) {
                            int i = (*it2)->toInt();
                            if (lexer == "beq") {
                                int word = (1<<28) | (s<<21) | (t<<16) | (i & 0xffff);
                                shift(word);
                            } else if (lexer == "bne") {
                                int word = (5<<26) | (s<<21) | (t<<16) | (i & 0xffff);
                                shift(word);
                            }
                            address1 += 4;
                        } else if (kind5 == Kind::ID) {
                            string id = (*it2)->getLexeme();
                            if (table.find(id) == table.end()) {
                                cerr << "ERROR: No Definition for " << id << endl;
                                error = 1;
                                break;
                            } else {
                                int n = table.find(id)->second;
                                int offset = (n - address1)/4 -1;
                                if (offset < -32768 || offset > 32767) {
                                    cerr << "ERROR: Branch offset out of range" << endl;
                                    error = 1;
                                    break;
                                }
                                if (lexer == "beq") {
                                    int word = (1<<28) | (s<<21) | (t<<16) | (offset & 0xffff);
                                    shift(word);
                                } else {
                                    int word = (5<<26) | (s<<21) | (t<<16) | (offset & 0xffff);
                                    shift(word);
                                }
                                address1 += 4;
                            }
                        }
                    } else if (lexer == "lis" || lexer == "mflo" || lexer == "mfhi") {
                        address1 += 4;
                        ++it2;
                        string lexer2 = (*it2)->getLexeme();
                        string snum = lexer2.substr(1,2);
                        istringstream iss {snum};
                        unsigned int reg;
                        iss >> reg;
                        if (lexer == "lis") {
                            int word = (reg<<11) | 20;
                            shift(word);
                        } else if (lexer == "mflo") {
                            int word = (reg<<11) | 18;
                            shift(word);
                        } else if (lexer == "mfhi") {
                            int word = (reg<<11) | 16;
                            shift(word);
                        }
                    } else if (lexer == "mult" || lexer == "multu" ||
                            lexer == "div" || lexer == "divu") {
                        address1 += 4;
                        // $s
                        ++it2;
                        string lexer3 = (*it2)->getLexeme();
                        string strs = lexer3.substr(1, 2);
                        istringstream iss3 {strs};
                        unsigned int s;
                        iss3 >> s;
                        // COMMA
                        ++it2;
                        // $t
                        ++it2;
                        string lexer4 = (*it2)->getLexeme();
                        string strt = lexer4.substr(1, 2);
                        istringstream iss4 {strt};
                        unsigned int t;
                        iss4 >> t;
                        // print out bin
                        if (lexer == "mult") {
                            int word = (s<<21) | (t<<16) | 24;
                            shift(word);
                        } else if (lexer == "multu") {
                            int word = (s<<21) | (t<<16) |25;
                            shift(word);
                        } else if (lexer == "div") {
                            int word = (s<<21) | (t<<16) | 26;
                            shift(word);
                        } else if (lexer == "divu") {
                            int word = (s<<21) | (t<<16) | 27;
                            shift(word);
                        }
                    } else if (lexer == "lw" || lexer == "sw") {
                        // $t
                        ++it2;
                        string lexer3 = (*it2)->getLexeme();
                        string strt = lexer3.substr(1, 2);
                        istringstream iss3 {strt};
                        unsigned int t;
                        iss3 >> t;
                        // COMMA
                        ++it2;
                        // i
                        ++it2;
                        int i = (*it2)->toInt();
                        //lparen
                        ++it2;
                        // $s
                        ++it2;
                        string lexer4 = (*it2)->getLexeme();
                        string strs = lexer4.substr(1, 2);
                        istringstream iss4 {strs};
                        unsigned int s;
                        iss4 >> s;
                        // rparen
                        ++it2;
                        if (lexer == "lw") {
                            int word = (35<<26) | (s<<21) | (t<<16) |(i & 0xffff);
                            shift(word);
                        } else if (lexer == "sw") {
                            int word = (43<<26) | (s<<21) | (t<<16) |(i & 0xffff);
                            shift(word);
                        }
                        address1 += 4;
                    }


                } else if (kind == LABEL) {
                    continue;
                } else {
                    cerr << "ERROR: Invalid Instruction" << endl;
                    error = 1;
                    break;
                }
            }
        }
    }

    if (!error) {
        int size = table.size();
        for (map<string, int>::const_iterator it = table.begin(); it != table.end(); ++it) {
            cerr << it->first << " " << it->second << endl;
        }
    }

    // Delete the Tokens that have been made
    vector<vector<Token*> >::iterator it;
    for(it = tokLines.begin(); it != tokLines.end(); ++it){
        vector<Token*>::iterator it2;
        for(it2 = it->begin(); it2 != it->end(); ++it2){
            delete *it2;
        }
    }
}
