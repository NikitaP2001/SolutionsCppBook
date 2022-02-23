#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <cmath>

#include "table.hpp"

enum token_value {
        NAME, NUMBER, END,
        PLUS='+', MINUS='-', MUL='*', DIV='/',
        PRINT=';', ASSIGN='=', LP='(', RP=')'
};

struct symbol {
        token_value tok;
        union {
                double number_value;
                char *name_string;
        };
} sym;

std::string GetFuncParams(std::string fline);
double ProcessFunction(std::string finput, std::string arguments);

std::string read_buf;   // holds current line

token_value get_token();
double term();
double StringToDouble(std::string &str);

table NameTable(200);

int no_of_errors;
double error(const char *s)
{
        std::cerr << "error: " << s << "\n";
        no_of_errors++;
        return 1;
}

double expr()
{
        double left = term();
        for(;;) {
                switch (sym.tok) {
                        case PLUS:
                                get_token();
                                left += term();
                                break;
                        case MINUS:
                                get_token();
                                left -= term();
                                break;
                        default:
                                return left;
                }
        }
}

double prim()
{
        switch (sym.tok) {
                case NUMBER:
                        get_token();
                        return sym.number_value;
                case NAME:
                {
                        if (get_token() == ASSIGN) {
                                name* n = NameTable.insert(sym.name_string);
                                get_token();
                                n->value = expr();
                                return n->value;
                        }
                        
                        std::string curr_line = read_buf;
                        curr_line.insert(0, 1, char(sym.tok));
                        std::string params = GetFuncParams(curr_line);
                        if (params.find('$', 0) != std::string::npos) {
                                name *n = NameTable.insert(sym.name_string);
                                n->isFunc = true;
                                n->isUserDef = true;
                                n->UserFunc = new std::string(curr_line);                                
                                
                                read_buf = "";
                                return 0;
                        }
                        
                        name *var = NameTable.look(sym.name_string);
                        if (var->isUserDef) {                               
                                double result = ProcessFunction(*var->UserFunc, params);
                                
                                sym.tok = PRINT;
                                read_buf = "";
                                return result;
                        } else if (var->isFunc) {
                                get_token();
                                double e = expr();
                                get_token();
                                return (*var->fptr)(e);
                        } else
                                return var->value;
                }
                case MINUS:
                        get_token();
                        return prim();
                case LP:
                        get_token();
                        double e;
                        e = expr();
                        if (sym.tok != RP)
                                return error(") expected");
                        get_token();
                        return e;
                case END:
                        return 1;
                default:
                        return error("Primary expected");
        }
}

double term()
{
        double left = prim();
        for(;;) {
                switch (sym.tok) {
                        case MUL:
                                get_token();
                                left *= prim();
                                break;
                        case DIV:
                                get_token();
                                double d;
                                d = prim();
                                if (d == 0)
                                        return error("Zero division");
                                left /= d;
                                break;
                        default:
                        return left;
                }
        }
}

token_value get_token()
{
        char ch;
        if (read_buf.length() == 0) {
                getline(std::cin, read_buf);
                if (read_buf.length() == 0)
                        return END;
                read_buf.push_back('\n');
        }
        
        do {
                ch = read_buf.at(0);
                read_buf = read_buf.substr(1, std::string::npos);
        } while ((ch != '\n') && isspace(ch));                      
        
        switch (ch) {
                case ';':
                case '\n':
                        return sym.tok=PRINT;
                case '*':
                case '/':
                case '+':
                case '-':
                case '(':
                case ')':
                case '=':
                        return sym.tok=token_value(ch);
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                case '.':
                {
                        read_buf.insert(0, 1, ch);
                        if (sym.tok == NAME)
                                delete[] sym.name_string;
                        sym.number_value = StringToDouble(read_buf);                                                                     
                        return sym.tok=NUMBER;
                }
                default:
                        if (isalpha(ch)) {                                
                                char *p = sym.name_string = new char[100];
                                *p++ = ch;
                                while ((ch = read_buf.at(0)) && isalnum(ch)) {                                        
                                        read_buf = read_buf.substr(1, std::string::npos);
                                        *p++ = ch;
                                }                                
                                *p = 0;
                                return sym.tok=NAME;
                        }
                        error("Invalid lexem");
                        return sym.tok=PRINT;
        }
}

/* Returns text inside first bracers met in fline, including
 * bracers themselves 
 */
std::string GetFuncParams(std::string fline)
{        
        char ch;
        int LBcount = 0;
        size_t prevBpos = 0;
        do {
                ch = fline.at(0);
                fline = fline.substr(1, std::string::npos);
        } while ((ch != '\n') && isspace(ch));
        
        if (ch == '(') {
                LBcount += 1;
                fline.insert(0, 1, ch);
        } else
                return "";
        
        while (LBcount != 0) {
                
                size_t LBpos = fline.find('(', prevBpos+1);
                size_t RBpos = fline.find(')', prevBpos+1);
                if (LBpos == std::string::npos && RBpos == std::string::npos)                     
                        return "";

                if (RBpos > LBpos) {
                        LBcount += 1;                                
                        prevBpos = LBpos;
                } else {                                
                        LBcount -= 1;
                        prevBpos = RBpos;                        
                }                
        }        
        
        return fline.substr(0, prevBpos + 1);
}


/* Executes function from finput, using string in arguments to
 * calculate function arguments.
 * Finput expected to be:
 *      ($param1,$param2,...) expr
 * arguments expects to be:
 *      (arg1, arg2,...)
 */
double ProcessFunction(std::string finput, std::string arguments)
{
        std::vector<std::string> params;
        arguments = arguments.substr(arguments.find('(', 0)+1, std::string::npos);
        bool notLastParam = true;
        
        // get each func param and put it in table with 
        // corresponding value from arguments
        while (notLastParam) {
                size_t startParam = finput.find('$', 0);
                if (startParam == std::string::npos) {
                        notLastParam = false;
                        continue;
                }                        
                finput = finput.substr(startParam + 1, std::string::npos);
                
                size_t endParam = finput.find(',', 0);
                if (endParam == std::string::npos) {    
                        endParam = finput.find(')', 0);
                        notLastParam = false;
                }
                size_t endArg = arguments.find(',', 0);
                if (endArg == std::string::npos) {
                        endArg = arguments.find(')', 0);
                        if (notLastParam) {
                                std::cout << "Error: wrong func args count" << std::endl;
                                return 0;
                        }                        
                }
                
                // read parameter                 
                std::string param = finput.substr(0, endParam);
                params.push_back(param);
                finput = finput.substr(endParam+1, std::string::npos);
                // read argument
                std::string arg = arguments.substr(0, endArg);
                arguments = arguments.substr(endArg+1, std::string::npos);
                
                // add param to name table
                name *new_param = NameTable.insert(&param[0]);
                read_buf = arg;
                read_buf.push_back('\n');
                get_token();
                new_param->value = expr();
        }
        
        // Execute function body
        read_buf = finput;
        read_buf.push_back('\n');
        get_token();
        double result = expr();
        
        // Delete local parameters
        for (int i = 0; i < params.size(); i++)
                NameTable.remove_name(&params[i][0]);
        return result;
}

int main()
{
        NameTable.insert("sqrt")->fptr = &sqrt;
        NameTable.look("sqrt")->isFunc = true;
        NameTable.insert("log")->fptr = &log;
        NameTable.look("log")->isFunc = true;
        NameTable.insert("sin")->fptr = &sin;
        NameTable.look("sin")->isFunc = 1;        
        NameTable.insert("pi")->value = 3.141592653589;
        NameTable.insert("e")->value = 2.7182818284;
        while (std::cin) {
                get_token();
                if (sym.tok == END) break;
                if (sym.tok == PRINT) continue;
                std::cout << expr() << '\n';
        }
        return no_of_errors;
}

/* Reads as much characters from string, as possible 
 * to interpret as double, and returns its value.
 * Part, which was read is truncated from string; 
 */
double StringToDouble(std::string &str)
{               
        if (!str.compare(0, std::strlen("inF"), "inF")) {
                std::cout << "inf found!" << std::endl;
                str = str.substr(std::strlen("inF"), std::string::npos);
                return std::atof("inF");
        }
        if (!str.compare(0, std::strlen("Nan"), "Nan")) {
                std::cout << "Nan found!" << std::endl;
                str = str.substr(std::strlen("Nan"), std::string::npos);
                return std::atof("Nan");
        }

        size_t pos = 0;
        bool isFloat = true, hasPoint = false;
        if (str.at(pos) == '+' || str.at(pos) == '-')
                pos++;
        while (isFloat && pos < str.length()) {                
                switch (str.at(pos)) {
                        case '0': case '1': case '2': case '3': case '4':
                        case '5': case '6': case '7': case '8': case '9':
                                pos++;
                                break;
                        case '.':
                                if (!hasPoint) {
                                        hasPoint = true;
                                        pos++;
                                } else
                                        isFloat = false;
                                break;
                        default:
                                isFloat = false;
                }
        }            
        std::string floatVal = str.substr(0, pos);
        str = str.substr(pos, std::string::npos);
        
        return std::atof(&floatVal[0]);
}