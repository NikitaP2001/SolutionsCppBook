#include <iostream>
#include <string.h>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cmath>

enum token_value {
        NAME, NUMBER, END,
        PLUS='+', MINUS='-', MUL='*', DIV='/',
        PRINT=';', ASSIGN='=', LP='(', RP=')'
};
token_value curr_tok;

struct name {        
        char *string;
        name *next;
        union {
                double (*fptr)(double);
                double value;
        };
        bool isFunc;
};
const int TBLSZ = 23;
name *table[TBLSZ];

token_value get_token();
double term();
double StringToDouble(std::string &str);
inline name* insert(const char *s);
name *look(const char *, int = 0);

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
                switch (curr_tok) {
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

double number_value;
char name_string[256];
double prim()
{
        switch (curr_tok) {
                case NUMBER:
                        get_token();
                        return number_value;
                case NAME:
                {
                        if (get_token() == ASSIGN) {
                                name* n = insert(name_string);
                                get_token();
                                n->value = expr();
                                return n->value;
                        }
                        name *var = look(name_string);
                        if (var->isFunc) {
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
                        if (curr_tok != RP)
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
                switch (curr_tok) {
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
        static std::string read_buf;
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
                        return curr_tok=PRINT;
                case '*':
                case '/':
                case '+':
                case '-':
                case '(':
                case ')':
                case '=':
                        return curr_tok=token_value(ch);
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                case '.':
                {
                        read_buf.insert(0, 1, ch);
                        number_value = StringToDouble(read_buf);                                                                     
                        return curr_tok=NUMBER;
                }
                default:
                        if (isalpha(ch)) {
                                char *p = name_string;
                                *p++ = ch;
                                while ((ch = read_buf.at(0)) && isalnum(ch)) {                                        
                                        read_buf = read_buf.substr(1, std::string::npos);
                                        *p++ = ch;
                                }                                
                                *p = 0;
                                return curr_tok=NAME;
                        }
                        error("Invalid lexem");
                        return curr_tok=PRINT;
        }
}

name *look(const char *p, int ins)
{
        int ii = 0;
        const char *pp = p;
        while (*pp) ii = ii << 1 ^ *pp++;
        if (ii < 0)
                ii = -ii;
        ii %= TBLSZ;
        for (name *n=table[ii]; n; n=n->next) {
                if (strcmp(p,n->string) == 0)
                        return n;
        }
        if (ins == 0) error("No such name found");
        name *nn = new name;
        nn->string = new char[strlen(p)+1];
        strcpy(nn->string, p);
        nn->value = 1;
        nn->next = table[ii];
        table[ii] = nn;
        return nn;
}

inline name* insert(const char *s) { return look(s, 1); } 

int main()
{
        insert("sqrt")->fptr = &sqrt;
        look("sqrt")->isFunc = 1;
        insert("log")->fptr = &log;
        look("log")->isFunc = 1;
        insert("sin")->fptr = &sin;
        look("sin")->isFunc = 1;        
        insert("pi")->value = 3.141592653589;
        insert("e")->value = 2.7182818284;
        while (std::cin) {
                get_token();
                if (curr_tok == END) break;
                if (curr_tok == PRINT) continue;
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