#include <iostream>
#include <sstream>
#define DEBUG


#ifdef DEBUG
int error(const char *fmt, ...)
{
	std::puts(fmt);
	return 0;
}
#else
#define error(fmt) {}
#endif


class expr {
std::stringstream ss_expr;		
enum token_value {
	NUMBER, END, PLUS='+', 
	MINUS='-', MUL='*', DIV='/'
};
struct sym {
 	token_value tok;
	int number_value;
} sym;

token_value get_token();
int parse_prim();
int parse_term();
int parse_expr();
public:
	expr(std::string);
	int eval();
	void print();
};

expr::expr(std::string sexpr)
{		
	ss_expr.str(sexpr);
}

int expr::eval()
{
	int result;
	// save stringstream content
	std::string content = ss_expr.str();

	get_token();
	result = parse_expr();

	// restore stringstream
	ss_expr.str(content);
	ss_expr.clear();
	return result;
}

void expr::print()
{
	std::cout << eval() << std::endl;
}

int expr::parse_prim()
{
	switch (sym.tok) {
                case NUMBER:
                        get_token();
                        return sym.number_value;
                case MINUS:
                        get_token();
                        return parse_prim();
		case END:
                        return 1;
                default:
                        return error("Primary expected");
        }
}

int expr::parse_term()
{
	int left = parse_prim();
        for(;;) {
                switch (sym.tok) {
                        case MUL:
                                get_token();
                                left *= parse_prim();
                                break;
                        case DIV:
                                get_token();
                                int d;
                                d = parse_prim();
                                if (d == 0)
                                        return error("Zero division");
                                left /= d;
                                break;
                        default:
                        return left;
                }
        }
}

int expr::parse_expr()
{
	int left = parse_term();
        for(;;) {
                switch (sym.tok) {
                        case PLUS:
                                get_token();
                                left += parse_term();
                                break;
                        case MINUS:
                                get_token();
                                left -= parse_term();
                                break;
                        default:
                                return left;
                }
        }
	return 0;
}

expr::token_value expr::get_token()
{
	char ch;

	do {
		ss_expr.get(ch);
	} while (isspace(ch) && !ss_expr.eof());

	if (ss_expr.eof())
		return END;
	switch (ch) {
		case '+': case '-': case '/': case '*':
			return sym.tok = token_value(ch);
		case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
			int numval;
			ss_expr.putback(ch);
			ss_expr >> numval;
			sym.number_value = numval;
			return sym.tok = NUMBER;
		default:
			error("Invalid lexem");
			return END;
	}

	return END;
}

int main()
{
	expr x = expr("123/4+123*4-3");
	std::cout << x.eval() << std::endl;
	x.print();
	x = expr("150+150/1");
	std::cout << x.eval() << std::endl;
}
