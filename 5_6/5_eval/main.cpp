#include <iostream>
#include <sstream>

enum token_value {
	NUMBER, END, PLUS='+', 
	MINUS='-', MUL='*', DIV='/'
};

class expr {
std::stringstream ss_expr;		
enum token_value {
	NUMBER, END, PLUS='+', 
	MINUS='-', MUL='*', DIV='/'
} tok;

token_value get_token();

public:
	expr(char *);
	int eval();
	void print();
};

expr::expr(char *szexpr)
{		
	ss_expr.str(szexpr);
}

int expr::eval()
{
	char ch;
	ss_expr.get(&ch);
	if (ss_expr.eof)
		return END;
		
	
	
	return 0;
}

void expr::print()
{
	std::cout << 0 << std::endl;
}

expr::token_value expr::get_token()
{
	return END;
}

int main()
{
	expr x = expr((char *)"123/4+123*4-3");
	std::cout << x.eval() << std::endl;
	x.print();
}