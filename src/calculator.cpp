/*
 * calculator.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: root
 */

#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
#include <exception>
#include <stdlib.h>

using namespace std;

//------------------------------------
//-------------VALUE MACROS-----------
//------------------------------------

#define DEG 57.295779513
#define RAD 0.0174532925

//------------------------------------
//-----------PARSING MACROS-----------
//------------------------------------

#define PI_CONST "pi"
#define E_CONST 'E'
#define SQRT_CONST "sqrt"
#define LOG_CONST "log"
#define LN_CONST "ln"
#define EXP10_CONST "e"
#define SIN_CONST "sin"
#define COS_CONST "cos"
#define TAN_CONST "tan"
#define SINH_CONST "sinh"
#define COSH_CONST "cosh"
#define TANH_CONST "tanh"
#define ASIN_CONST "asin"
#define ACOS_CONST "acos"
#define ATAN_CONST "atan"

//------------------------------------
//-------------STRUCTS----------------
//------------------------------------

struct exp_t{
	const char* exp;
	int explen;
	char c_ch;
	int pos;
};

//------------------------------------
//-------------FUNCTION DEF-----------
//------------------------------------

bool eat(exp_t* exp, int eat);
void next_char(exp_t* exp);
double parse_factor(exp_t* exp);
double parse_term(exp_t* exp);
double parse_exp(exp_t* exp);
double parse(const char* expstr);

//------------------------------------
//---------------MAIN-----------------
//------------------------------------

void calc(const char* exp){
	try{
		double result = parse(exp);
		cout << "= " << result << endl;
	}catch(std::exception &ex){
		cout << ex.what() << endl;
	}
}

int main(int argc, char** argv) {
	string str;

	if(argc > 1){
		calc(argv[1]);
		return 0;
	}

	cin >> str;
	while(str != "exit"){
		const char* exp = str.c_str();
		calc(exp);
		str.clear();
		cin >> str;
	}

	return 0;
}

//------------------------------------
//-----------FUNCTIONS IMPL-----------
//------------------------------------

bool eat(exp_t* exp, int eat){
	while (exp->c_ch == ' ')
		next_char(exp);
	if(exp->c_ch == eat){
		next_char(exp);
		return true;
	}
	return false;
}
void next_char(exp_t* exp){
	exp->c_ch = (++(exp->pos) < exp->explen) ? exp->exp[exp->pos] : -1;
}
double parse_factor(exp_t* exp){
    if(eat(exp, '+')) return parse_factor(exp); // unary plus
    if(eat(exp, '-')) return -parse_factor(exp); // unary minus

    double x = 0;
    int startPos = exp->pos;
    if(eat(exp, '(')) { // parentheses
        x = parse_exp(exp);
        if(!eat(exp, ')'))
        	throw std::runtime_error("Syntax Error: missing parentheses closer");
    }else if(eat(exp, '|')) {
        x = fabs(parse_exp(exp));
        if(!eat(exp, '|'))
     	   throw std::runtime_error("Syntax Error: missing absolute closer");
    }else if((exp->c_ch >= '0' && exp->c_ch <= '9') || exp->c_ch == '.') { // numbers
        while((exp->c_ch >= '0' && exp->c_ch <= '9') || exp->c_ch == '.')
        	next_char(exp);
        char sub[exp->pos - startPos + 1];
        strncpy(sub, exp->exp + startPos, exp->pos - startPos);
        sub[exp->pos - startPos] = 0;
        x = atof(sub);
    }else if((exp->c_ch >= 'a' && exp->c_ch <= 'z') || exp->c_ch == 'E') { // functions
        while((exp->c_ch >= 'a' && exp->c_ch <= 'z') || exp->c_ch == 'E')
        	next_char(exp);
        char func[exp->pos - startPos + 1];
        strncpy(func, exp->exp + startPos, exp->pos - startPos);
        func[exp->pos - startPos] = 0;

        if(func[0] == E_CONST && strlen(func) == 1)
        	return M_E;
        if(strcmp(func, PI_CONST) == 0)
        	return M_PI;

        x = parse_factor(exp);

        if (strcmp(func, SQRT_CONST) == 0) x = sqrt(x);
        //else if(strcmp(func, "exp")) x = exp(x);
        else if(strcmp(func, LN_CONST) == 0) x = log(x);
        else if(strcmp(func, LOG_CONST) == 0) x = log10(x);
        else if(strcmp(func, SIN_CONST) == 0) x = sin(x * RAD);
        else if(strcmp(func, COS_CONST) == 0) x = cos(x * RAD);
        else if(strcmp(func, TAN_CONST) == 0) x = tan(x * RAD);
        else if(strcmp(func, SINH_CONST) == 0) x = sinh(x * RAD);
        else if(strcmp(func, COSH_CONST) == 0) x = cosh(x * RAD);
        else if(strcmp(func, TANH_CONST) == 0) x = tanh(x * RAD);
        else if(strcmp(func, ASIN_CONST) == 0) x = asin(x) * DEG;
        else if(strcmp(func, ACOS_CONST) == 0) x = acos(x) * DEG;
        else if(strcmp(func, ATAN_CONST) == 0) x = atan(x) * DEG;

        else throw std::runtime_error("Syntax Error: unknown function");
    }

    if(eat(exp, '^'))
    	x = pow(x, parse_factor(exp)); // exponentiation
    else if(eat(exp, 'e'))
		x *= pow(10, parse_factor(exp));

    return x;
}
double parse_term(exp_t* exp){
	double result = parse_factor(exp);

	for(;;){
		if(eat(exp, '*'))
			result *= parse_factor(exp);
		else if(eat(exp, '/'))
			result /= parse_factor(exp);
		else
			return result;
	}
	return 0.0;
}
double parse_exp(exp_t* exp){
	double result = parse_term(exp);

	for(;;){
		if(eat(exp, '+'))
			result += parse_term(exp);
		else if(eat(exp, '-'))
			result -= parse_term(exp);
		else
			return result;
	}
	return 0.0;
}
double parse(const char* expstr){
	exp_t exp;
	exp.exp = expstr;
	exp.explen = strlen(expstr);
	exp.pos = -1;
	exp.c_ch = 0;

	next_char(&exp);
	double result = parse_exp(&exp);

	if(exp.pos < exp.explen)
		throw std::runtime_error("Syntax Error: unexpected character");
	return result;
}








