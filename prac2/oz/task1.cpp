#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <stack>
#include <cstring>

using namespace std;


template<class T, int max_size>
class Stack {
    T s[max_size];
    int top;
public:
    Stack() { top = 0; }

    void reset() { top = 0; }

    void push(T i);

    T pop();

    bool is_empty() { return top == 0; }

    bool is_full() { return top == max_size; }
};

template<class T, int max_size>
void Stack<T, max_size>::push(T i) {
    if (!is_full()) {
        s[top] = i;
        ++top;
    } else
        throw "Stack_is_full";
}

template<class T, int max_size>
T Stack<T, max_size>::pop() {
    if (!is_empty()) {
        --top;
        return s[top];
    } else
        throw "Stack_is_empty";
}


enum type_of_lex {//наши лексемы
    LEX_NULL, //0
    LEX_PROGRAM,//1
    LEX_INT,//2
    LEX_BOOL,//3
    LEX_STRING,//4
    LEX_STRUCT,//5
    LEX_TRUE,//6
    LEX_FALSE,//7
    LEX_IF,//8
    LEX_ELSE,//9
    LEX_WHILE,//10
    LEX_FOR,//11
    LEX_BREAK,//12
    LEX_AND,//13
    LEX_OR,//14
    LEX_NOT,//15
    LEX_READ,//16
    LEX_WRITE,//17
    LEX_GOTO,//18
    LEX_FIN,//19
    LEX_COMMA,//20
    LEX_SEMICOLON,//21
    LEX_COLON,//22
    LEX_POINT,//23
    LEX_ASSIGN,//24
    LEX_PLUS,//25
    LEX_MINUS,//26
    LEX_TIMES,//27
    LEX_SLASH,//28
    LEX_DEC,//29
    LEX_EQ,//30
    LEX_NEQ,//31
    LEX_LSS,//32
    LEX_GTR,//33
    LEX_LEQ,//34
    LEX_GEQ,//35
    LEX_LPAREN,//36
    LEX_RPAREN,//37
    LEX_LBRACE,//38
    LEX_RBRACE,//39
    LEX_NUM, //40
    LEX_STR, //41
    LEX_ID, //42
    POLIZ_LABEL, // 43
    POLIZ_ADDRESS, // 44
    POLIZ_GO, // 45
    POLIZ_FGO // 46
};

class Lex {
    type_of_lex t_lex;
    int v_lex;
public:
    explicit Lex(type_of_lex t = LEX_NULL, int v = 0) {
        t_lex = t;
        v_lex = v;
    }

    type_of_lex get_type() const {
        return t_lex;
    }

    int get_value() const {
        return v_lex;
    }

    friend ostream &operator<<(ostream &s, Lex l);
};

class Poliz {
    Lex *p;
    int size;
    int free;
public:
    Poliz(int max_size) {
        p = new Lex[size = max_size];
        free = 0;
    };

    ~Poliz() { delete[]p; };

    void put_lex(Lex l) {
        p[free] = l;
        ++free;
    };

    void put_lex(Lex l, int place) { p[place] = l; };

    void blank() { ++free; };

    int get_free() { return free; };

    Lex &operator[](int index) {
        if (index > size)
            throw "POLIZ:out of array";
        else if (index > free)
            throw "POLIZ:indefinite element of array";
        else
            return p[index];
    };

    void print() {
        for (int i = 0; i < free; ++i)
            cout << p[i];
    };
};

class Ident {
    string name;
    bool declare;
    bool jump;
    type_of_lex type;
    bool assign;
    int value;
public:
    explicit Ident() {
        declare = false;
        assign = false;
        jump = false;
    }

    explicit Ident(const string &n) {
        name = n;
        declare = false;
        assign = false;
        jump = false;
    }

    bool operator==(const string &s) const {
        return name == s;
    }

    string get_name() { return name; }

    bool get_declare() { return declare; }

    void put_declare() { declare = true; }

    bool get_jump() { return jump; }

    bool put_jump() { jump = true; }

    type_of_lex get_type() { return type; }

    void put_type(type_of_lex t) { type = t; }

    bool get_assign() { return assign; }

    void put_assign() { assign = true; }

    int get_value() { return value; }

    void put_value(int v) { value = v; }
};

vector <Ident> TID;//таблица идентификаторов анализируемой программы;
vector<int> TJG;//таблица идентификаторов для goto
vector <string> TSTR;//таблица строк
vector<int> HFJ;

int put(const string &buf, char c) {
    vector<Ident>::iterator k;
    if (c == 'A') {
        if ((k = find(TID.begin(), TID.end(), buf)) != TID.end())
            return k - TID.begin();
        TID.push_back(Ident(buf));
        return TID.size() - 1;
    } else if (c == 'S') {
        vector<string>::iterator k;
        if ((k = find(TSTR.begin(), TSTR.end(), buf)) != TSTR.end()) {
            return k - TSTR.begin();
        }
        TSTR.push_back(buf);
        return TSTR.size() - 1;
    }
}

vector<int> checkJ;
stack <vector<int>> STBREAK;

void check_id_jump(int number) {
    vector<int>::iterator k;
    if ((k = find(checkJ.begin(), checkJ.end(), number)) != checkJ.end()) {
        cout << "MARKEDIDERROR";
        throw "a";
    } else {
        checkJ.push_back(number);
    }

}

int putgoto(const int buf, const int p) {
    vector<int>::iterator k;
    if ((k = find(TJG.begin(), TJG.end(), buf)) != TJG.end())
        return HFJ[k - TJG.begin()];
    if (p != 0) {
        TJG.push_back(buf);
        HFJ.push_back(p);
    }
    return 0;
}

class Scanner {
    FILE *fp;
    char c;

    int look(const string &buf, const char **list) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i]) {
                return i;
            }
            i++;
        }
        return 0;
    }

    void gc() { c = fgetc(fp); }

public:
    const static char *TW[], *TD[];
    vector<char *> TSTR;

    explicit Scanner(const char *program) {
        fp = fopen(program, "r");
    }

    Lex get_lex();

    ~Scanner() {
        for (int i = 0; i < TSTR.size(); i++) {
            delete TSTR[i];
        }
    }
};

const char *Scanner::TW[] = {//TW - таблица служебных слов М-языка;
        "",//0
        "program",//1
        "int",//2
        "bool",//3
        "string", // 4
        "struct", // 5
        "true", // 6
        "false", // 7
        "if", // 8
        "else", //9
        "while", //10
        "for", // 11
        "break", // 12
        "and", // 14
        "or", // 15
        "not", // 16
        "read", // 17
        "write", // 18
        "goto",// 19
        nullptr
};

const char *Scanner::TD[] = {//TD - таблица ограничителей М-языка;
        "", // 0
        "@", // 1
        ",", // 2
        ";", // 3
        ":", // 4
        ".", // 5
        "=", // 6
        "+", // 7
        "-", // 8
        "*", // 9
        "/", // 10
        "--", // 11
        "==", // 12
        "!=", // 13
        "<", // 14
        ">", // 15
        "<=", // 16
        ">=", // 17
        "(", // 18
        ")", // 19
        "{", // 20
        "}", // 21
        nullptr
};

ostream &operator<<(ostream &s, Lex l) {//перегрузка << для вывода информации
    s << '[' << l.t_lex << "---" << l.v_lex << "] === ";
    if ((l.t_lex >= LEX_PROGRAM) && (l.t_lex <= LEX_GOTO)) {
        cout << Scanner::TW[l.t_lex];
    } else if (l.t_lex == LEX_ASSIGN) {
        cout << "LEX_ASSIGN";
    } else if ((l.t_lex >= LEX_FIN) && (l.t_lex <= LEX_RBRACE) && (l.v_lex == 0)) {
        cout << Scanner::TD[l.t_lex - 18];
    } else if ((l.t_lex >= LEX_FIN) && (l.t_lex <= LEX_RBRACE)) {
        cout << Scanner::TD[l.v_lex];
    } else if (l.t_lex == LEX_NUM) {
        cout << l.v_lex;
    } else if (l.t_lex == LEX_STR) {
        cout << TSTR[l.v_lex];
    } else if (l.t_lex == LEX_ID) {
        cout << TID[l.v_lex].get_name();
    } else if (l.t_lex == POLIZ_LABEL) {
        cout << "POLIZ_LABEL";
    } else if (l.t_lex == POLIZ_ADDRESS) {
        cout << "POLIZ_ADDRESS";
    } else if (l.t_lex == POLIZ_GO) {
        cout << "POLIZ_GO";
    } else if (l.t_lex == POLIZ_FGO) {
        cout << "POLIZ_FGO";
    } else if (l.t_lex == POLIZ_FGO) {
        cout << "POLIZ_FGO";
    } else {
        cout << " Error";
    }
    cout << endl;
    return s;
}

Lex Scanner::get_lex() {
    enum state {
        H, IDENT, NUMB, COM, ALE, NEQ, CBEG, CEND, STR, DEC
    };
    state CS = H;
    string buf;
    int d, j;
    do {
        gc();
        switch (CS) {
            case H:
                if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                } else if (isalpha(c)) {
                    buf.push_back(c);
                    CS = IDENT;
                } else if (isdigit(c)) {
                    d = c - '0';
                    CS = NUMB;
                } else if (c == '"') {
                    CS = STR;
                } else if (c == '/') {
                    CS = CBEG;
                } else if (c == '=' || c == '<' || c == '>') {
                    buf.push_back(c);
                    CS = ALE;
                } else if (c == '-') {
                    buf.push_back(c);
                    CS = DEC;
                } else if (c == EOF) {
                    return Lex(LEX_FIN);
                } else if (c == '!') {
                    buf.push_back(c);
                    CS = NEQ;
                } else {
                    buf.push_back(c);
                    if ((j = look(buf, TD))) {
                        return Lex((type_of_lex) (j + (int) LEX_FIN - 1), j);
                    } else {
                        throw c;
                    }
                }
                break;

            case IDENT:
                if (isalpha(c) || isdigit(c)) {
                    buf.push_back(c);
                } else {
                    ungetc(c, fp);
                    if ((j = look(buf, TW))) return Lex((type_of_lex) j, j);
                    else {
                        j = put(buf, 'A');
                        return Lex(LEX_ID, j);
                    }
                }
                break;
            case NUMB:
                if (isdigit(c)) {
                    d = d * 10 + (c - '0');
                } else {
                    ungetc(c, fp);
                    return Lex(LEX_NUM, d);
                }
                break;

            case COM://тело комментария
                if (c == '*') { CS = CEND; }
                else if (c == EOF) { throw '@'; }
                break;
            case ALE://для сравнений
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex((type_of_lex) (j + (int) LEX_FIN - 1), j);
                } else {
                    ungetc(c, fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex) (j + (int) LEX_FIN - 1), j);
                }
            case NEQ://не равно
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex(LEX_NEQ, j);
                } else {
                    throw '!';
                }
            case CBEG://начало комментария
                if (c == '*') {
                    CS = COM;
                } else {
                    return Lex((type_of_lex) (LEX_SLASH), 10);
                }
                break;
            case CEND://конец комментария
                if (c == '/') {
                    CS = H;
                } else if (c == EOF) {
                    throw '/';
                } else {
                    CS = COM;
                }
                break;
            case STR://блок для строки
                if (c == '"') {
                    j = put(buf, 'S');
                    return Lex(LEX_STR, j);
                } else if (c == EOF) {
                    throw 'S';
                } else {
                    buf.push_back(c);
                }
                break;
            case DEC://блок для -
                if (c == '-') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex((type_of_lex) (j + (int) LEX_FIN - 1), j);
                } else {
                    ungetc(c, fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex) (j + (int) LEX_FIN - 1), j);
                }
        }
    } while (true);
}

class Parser {
    Lex curr_lex; // текущая лексема
    type_of_lex c_type;
    int c_val;
    static int i1, i2;
    Scanner scan;
    Stack<int, 100> st_int;
    Stack<type_of_lex, 100> st_lex;

    void start();

    void descriptions();

    void description();

    void type();

    void variable();

    void ident();

    void constant();

//    void celo();
//
//    void znak();
//
//    void strok();

    void operators();

    void operat();

    void composite_operator();

    void operat_expression();

    void E();

    void A();

    void Q();

    void M();

    void E1();

    void T();

    void F();

    void G();

    void dec(type_of_lex type, int id_idx); // семантичиеские действия

    void check_id();

    void check_op();

    void check_not();

    void check_minus();

    void eq_type();

    void eq_bool();

    void check_id_in_read();

    void check_id_in_goto();

    void gl() // получить очередную лексему
    {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }

public:

    Poliz prog; // внутреннее представление программы
    Parser(const char *program) : scan(program), prog(1000) {}

    void analyze(); // анализатор с действиями
};

int Parser::i1 = 0;
int Parser::i2 = 0;

void Parser::dec(type_of_lex type, int id_idx) {
    if (TID[id_idx].get_declare()) {
        throw "twice";
    } else {
        TID[id_idx].put_declare();
        if (type == LEX_STRING) {
            TID[id_idx].put_value(TSTR.size());
        }
        TID[id_idx].put_type(type);
    }
}

void Parser::analyze() {
    gl();

    start();

    prog.print();
    cout << endl << "Yes!!!" << endl;
}

void Parser::start() {
    if (c_type == LEX_PROGRAM)
        gl();
    else {
        cout << curr_lex;
        gl();
        cout << curr_lex;
        throw curr_lex;
    }
    if (c_type == LEX_LBRACE)
        gl();
    else {
        cout << curr_lex;
        gl();
        cout << curr_lex;
        throw curr_lex;
    }
    descriptions();
    operators();

    if (c_type == LEX_RBRACE)
        gl();
    else {
        cout << "}ERROR";
        cout << curr_lex;
        gl();
        cout << curr_lex;
        throw curr_lex;
    }
    if (c_type != LEX_FIN) {
        cout << curr_lex;
        gl();
        cout << curr_lex;
        throw curr_lex;
    }
}

void Parser::descriptions() {
    description();
    while (c_type == LEX_SEMICOLON) {
        gl();
        if (c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING) {
            description();
        }
    }
}

void Parser::description() {
    type();
    variable();
    while (c_type == LEX_COMMA) {
        gl();
        variable();
    }
}

void Parser::type() {
    if (c_type == LEX_INT) {
        gl();
        i1 = 1;
    } else if (c_type == LEX_BOOL) {
        gl();
        i1 = 2;
    } else if (c_type == LEX_STRING) {
        gl();
        i1 = 3;
    } else {
        cout << "typeERROR";
        cout << curr_lex;
        gl();
        cout << curr_lex;
        throw curr_lex;
    }
}

void Parser::variable() {
    if (c_type == LEX_ID) {
        if (i1 == 1)
            dec(LEX_INT, c_val);
        if (i1 == 2)
            dec(LEX_BOOL, c_val);
        if (i1 == 3)
            dec(LEX_STRING, c_val);
        check_id();
        int help3 = c_val;
        gl();
        if (c_type == LEX_ASSIGN) {
            prog.put_lex(Lex(POLIZ_ADDRESS, help3));
            st_lex.push(c_type);
            gl();
            constant();
            check_op();
        }
    } else {
        cout << "variableERROR";
        cout << curr_lex;
        gl();
        cout << curr_lex;
        throw curr_lex;
    }
}

void Parser::constant() {
    if (c_type == LEX_NUM && i1 == 1) {
        st_lex.push(LEX_INT);
        prog.put_lex(curr_lex);
        gl();
    } else if (c_type == LEX_TRUE && i1 == 2) {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_TRUE, 1));
        gl();
    } else if (c_type == LEX_FALSE && i1 == 2) {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_FALSE, 0));
        gl();
    } else if (c_type == LEX_STR && i1 == 3) {
        st_lex.push(LEX_STR);
        prog.put_lex(curr_lex);
        gl();
    } else if (c_type == LEX_MINUS && i1 == 1) {
        gl();
        if (c_type != LEX_NUM) {
            cout << "constantERROR";
            cout << curr_lex;
            gl();
            cout << curr_lex;
            throw curr_lex;
        } else {
            st_lex.push(LEX_INT);
            prog.put_lex(Lex(LEX_NUM, 0));
            prog.put_lex(curr_lex);
            check_minus();
            gl();
        }
    } else if (c_type == LEX_PLUS && i1 == 1) {
        gl();
        st_lex.push(LEX_INT);
        prog.put_lex(curr_lex);
        gl();
    } else {
        cout << "constantERROR";
        cout << curr_lex;
        gl();
        cout << curr_lex;
        throw curr_lex;
    }
}

//void Parser::celo() {
//    if (c_type == LEX_PLUS || c_type == LEX_MINUS) {
//        znak();
//        if (c_type == LEX_NUM) {
//            st_lex.push(LEX_INT);
//            prog.put_lex(curr_lex);
//            gl();
//        }
//    } else if (c_type == LEX_NUM) {
//        st_lex.push(LEX_INT);
//        prog.put_lex(curr_lex);
//        gl();
//    } else
//        cout << curr_lex; gl();cout << curr_lex;throw curr_lex;
//}

//void Parser::znak() {
//    if (c_type == LEX_PLUS || c_type == LEX_MINUS) {
//        st_lex.push(c_type);
//        gl();
//    } else
//        cout << curr_lex; gl();cout << curr_lex;throw curr_lex;
//}

//void Parser::strok() {
//    if (c_type == LEX_QUOTE) {
//        gl();
//    } else
//        cout << curr_lex; gl();cout << curr_lex;throw curr_lex;
//    if (c_type == LEX_STR) {
//        st_lex.push()(LEX_STR);
//        prog.put_lex(curr_lex);
//        gl();
//    } else
//        cout << curr_lex; gl();cout << curr_lex;throw curr_lex;
//    if (c_type == LEX_QUOTE) {
//        gl();
//    } else
//        cout << curr_lex; gl();cout << curr_lex;throw curr_lex;
//}


void Parser::operators() {
    operat();
    while (c_type == LEX_IF || c_type == LEX_WHILE || c_type == LEX_BREAK || c_type == LEX_READ ||
           c_type == LEX_WRITE || c_type == LEX_GOTO || c_type == LEX_LBRACE || c_type == LEX_ID ||
           c_type == LEX_FOR) {
        operat();
    }

}

void Parser::operat() {
    int pl0, pl1, pl2, pl3, pl4, pl5, pl6, pl7, pl8;
    if (c_type == LEX_IF) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            E();
            eq_bool();
            pl2 = prog.get_free();
            prog.blank();
            prog.put_lex(Lex(POLIZ_FGO));
            if (c_type == LEX_RPAREN) {
                gl();
                operat();
                pl3 = prog.get_free();
                prog.blank();
                prog.put_lex(Lex(POLIZ_GO));
                prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
                if (c_type == LEX_ELSE) {
                    gl();
                    operat();
                    prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
                } else {
                    cout << "ELSEERROR";
                    cout << curr_lex;
                    gl();
                    cout << curr_lex;
                    throw curr_lex;
                }
            } else {
                cout << "IFCLOSEERROR";
                cout << curr_lex;
                gl();
                cout << curr_lex;
                throw curr_lex;
            }
        } else {
            cout << "IFOPENERROR";
            cout << curr_lex;
            gl();
            cout << curr_lex;
            throw curr_lex;
        }
    } else if (c_type == LEX_FOR) {
        STBREAK.push(vector<int>());
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            if (c_type != LEX_SEMICOLON) {
                E();
            }
            if (c_type == LEX_SEMICOLON) {
                gl();
                pl4 = prog.get_free();
                if (c_type != LEX_SEMICOLON) {
                    E();
                    eq_bool();
                }
                pl5 = prog.get_free();
                prog.blank();
                prog.put_lex(Lex(POLIZ_FGO));
                if (c_type == LEX_SEMICOLON) {
                    gl();
                    pl6 = prog.get_free();
                    prog.blank();
                    prog.put_lex((Lex(POLIZ_GO)));
                    pl8 = prog.get_free();
                    if (c_type != LEX_RPAREN) {
                        E();
                    }
                    prog.put_lex(Lex(POLIZ_LABEL, pl4));
                    prog.put_lex(Lex(POLIZ_GO));
                    if (c_type == LEX_RPAREN) {
                        gl();
                        pl7 = prog.get_free();
                        prog.put_lex(Lex(POLIZ_LABEL, pl7), pl6);
                        operat();
                        prog.put_lex(Lex(POLIZ_LABEL, pl8));
                        prog.put_lex(Lex(POLIZ_GO));
                        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl5);
                        while (!STBREAK.top().empty()) {
                            prog[STBREAK.top().back()] = Lex(POLIZ_LABEL, prog.get_free());
                            STBREAK.top().pop_back();
                        }
                        STBREAK.pop();
//                        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
                    } else {
                        cout << "forsz";
                        cout << curr_lex;
                        gl();
                        cout << curr_lex;
                        throw curr_lex;
                    }
                } else {
                    cout << "for2tz";
                    cout << curr_lex;
                    gl();
                    cout << curr_lex;
                    throw curr_lex;
                }
            } else {
                cout << "for1tz";
                cout << curr_lex;
                gl();
                cout << curr_lex;
                throw curr_lex;
            }
        } else {
            cout << "for1tz";
            cout << curr_lex;
            gl();
            cout << curr_lex;
            throw curr_lex;
        }
    } else if (c_type == LEX_WHILE) {
        gl();
        STBREAK.push(vector<int>());
        if (c_type == LEX_LPAREN) {
            pl0 = prog.get_free();
            gl();
            E();
            eq_bool();
            pl1 = prog.get_free();
            prog.blank();
            prog.put_lex(Lex(POLIZ_FGO));
            if (c_type == LEX_RPAREN) {
                gl();
                operat();
                prog.put_lex(Lex(POLIZ_LABEL, pl0));
                prog.put_lex(Lex(POLIZ_GO));
                prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
                while (!STBREAK.top().empty()) {
                    prog[STBREAK.top().back()] = Lex(POLIZ_LABEL, prog.get_free());
                    STBREAK.top().pop_back();
                }
                STBREAK.pop();
            } else {
                cout << "WHILECLOSEERROR";
                cout << curr_lex;
                gl();
                cout << curr_lex;
                throw curr_lex;
            }
        } else {
            cout << "WHILEOPENERROR";
            cout << curr_lex;
            gl();
            cout << curr_lex;
            throw curr_lex;
        }
    } else if (c_type == LEX_BREAK) {
        if (STBREAK.size() == 0) {
            cout << "BREAKOUTERROR";
            cout << curr_lex;
            gl();
            cout << curr_lex;
            throw curr_lex;
        }
        STBREAK.top().push_back(prog.get_free());
        prog.put_lex(Lex());
        prog.put_lex(Lex(POLIZ_GO));
        gl();
        if (c_type == LEX_SEMICOLON) {
            gl();
        } else {
            cout << "BREAK;ERROR";
            cout << curr_lex;
            gl();
            cout << curr_lex;
            throw curr_lex;
        }
    } else if (c_type == LEX_GOTO) {
        gl();
        if (c_type == LEX_ID) {
            int help = putgoto(c_val, 0);
            if (help != 0) {
                prog.put_lex(Lex(POLIZ_LABEL, help));
                prog.put_lex(Lex(POLIZ_GO));
            } else {
                putgoto(c_val, prog.get_free());
                prog.blank();
                prog.put_lex((Lex(POLIZ_GO)));
            }
            gl();
            if (c_type == LEX_SEMICOLON) {
                gl();
            } else {
                cout << "GOTO;ERROR";
                cout << curr_lex;
                gl();
                cout << curr_lex;
                throw curr_lex;
            }
        } else {
            cout << "GOTOIDERROR";
            cout << curr_lex;
            gl();
            cout << curr_lex;
            throw curr_lex;
        }

    } else if (c_type == LEX_READ) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            if (c_type == LEX_ID) {
                check_id_in_read();
                prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
                gl();
                if (c_type == LEX_RPAREN) {
                    gl();
                    prog.put_lex(Lex(LEX_READ));
                    if (c_type == LEX_SEMICOLON) {
                        gl();
                    } else {
                        cout << "READ;ERROR";
                        cout << curr_lex;
                        gl();
                        cout << curr_lex;
                        throw curr_lex;
                    }
                } else {
                    cout << "READCLOSEERROR";
                    cout << curr_lex;
                    gl();
                    cout << curr_lex;
                    throw curr_lex;
                }
            } else {
                cout << "READIDERROR";
                cout << curr_lex;
                gl();
                cout << curr_lex;
                throw curr_lex;
            }
        } else {
            cout << "READOPENERROR";
            cout << curr_lex;
            gl();
            cout << curr_lex;
            throw curr_lex;
        }
    } else if (c_type == LEX_WRITE) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            E();
            prog.put_lex(Lex(LEX_WRITE));
            while (c_type == LEX_COMMA) {
                gl();
                E();
                prog.put_lex(Lex(LEX_WRITE));
            }
            if (c_type == LEX_RPAREN) {
                gl();
                if (c_type == LEX_SEMICOLON) {
                    gl();
                } else {
                    cout << "WRITE;ERROR";
                    cout << curr_lex;
                    gl();
                    cout << curr_lex;
                    throw curr_lex;
                }
            } else {
                cout << "WRITECLOSEERROR";
                cout << curr_lex;
                gl();
                cout << curr_lex;
                throw curr_lex;
            }
        } else {
            cout << "WRITEOPENERROR";
            cout << curr_lex;
            gl();
            cout << curr_lex;
            throw curr_lex;
        }
    } else if (c_type == LEX_LBRACE) {
        gl();
        operators();
        if (c_type == LEX_RBRACE) {
            gl();
        } else {
            cout << "COMPOPRERROR";
            cout << curr_lex;
            gl();
            cout << curr_lex;
            throw curr_lex;
        }
    } else if (c_type == LEX_ID) {
        if (TID[c_val].get_declare() == false) {
            int helpc_val1 = c_val;
            gl();
            if (c_type == LEX_COLON) {
                check_id_jump(helpc_val1);
                int help2;
                if ((help2 = putgoto(helpc_val1, prog.get_free())) != 0) {
                    int help3 = prog.get_free();
                    prog.put_lex(Lex(POLIZ_LABEL, help3), help2);
                }
//                pl7 = prog.get_free();
//                prog.put_lex(Lex(POLIZ_LABEL, pl7), pl6);

                gl();
                operat();
            } else {
                cout << curr_lex;
                gl();
                cout << curr_lex;
                throw "not declared ID";
            }
        } else {
            check_id();
//            while (c_type == LEX_ASSIGN){
//                gl();
//                E();
//                eq_type();
//                prog.put_lex(Lex(LEX_ASSIGN));
//            }
            E();
            operators();
        }
    } else {
        E();
        if (c_type == LEX_SEMICOLON) {
            gl();
        } else {
            cout << "E";
            cout << curr_lex;
            gl();
            cout << curr_lex;
            throw curr_lex;
        }
    }
}

//void Parser::E() {
//    A();
//    if(c_type == LEX_COLON){
//        st_lex.push(c_type);
//        if (putgoto(c_val) == 1){
//            cout << "pomid";
//            cout << curr_lex;
//            gl();
//            cout << curr_lex;
//            throw curr_lex;
//        }
//        gl();
//        operat();
//        check_op();
//    }
//}
void Parser::E() {
    G();
    if (c_type == LEX_ASSIGN) {
        int u = 0;
        while (c_type == LEX_ASSIGN) {
            gl();
            G();
            eq_type();
            u += 1;
        }
        for (int t = 0; t < u; t++){
            prog.put_lex(Lex(LEX_ASSIGN));
        }
    }
//    while (c_type == LEX_ASSIGN) {
//        gl();
//        G();
//        eq_type();
//        prog.put_lex(Lex(LEX_ASSIGN));
//    }
}

void Parser::G() {
    M();
    while (c_type == LEX_OR) {
        st_lex.push(c_type);
        gl();
        M();
        check_op();
    }
}

void Parser::M() {
    Q();
    while (c_type == LEX_AND) {
        st_lex.push(c_type);
        gl();
        Q();
        check_op();
    }
}

void Parser::Q() {
    E1();
    while (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
           c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ) {
        st_lex.push(c_type);
        gl();
        E1();
        check_op();
    }
}

void Parser::E1() {
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS) {
        st_lex.push(c_type);
        gl();
        T();
        check_op();
    }
}

void Parser::T() {
    F();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH) {
        st_lex.push(c_type);
        gl();
        F();
        check_op();
    }
}


void Parser::F() {
    if (c_type == LEX_ID) {
        check_id();
        int help4 = c_val;
        gl();
        if (c_type == LEX_ASSIGN) {
            prog.put_lex(Lex(POLIZ_ADDRESS, help4));
        } else {
            prog.put_lex(Lex(LEX_ID, help4));
        }
    } else if (c_type == LEX_NUM) {
        st_lex.push(LEX_INT);
        prog.put_lex(curr_lex);
        gl();
    } else if (c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_TRUE, 1));
        gl();
    } else if (c_type == LEX_STR) {
        st_lex.push(LEX_STR);
        prog.put_lex(curr_lex);
        gl();
    } else if (c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_FALSE, 0));
        gl();
    } else if (c_type == LEX_NOT) {
        gl();
        F();
        check_not();
    } else if (c_type == LEX_MINUS) {
        gl();
        prog.put_lex(Lex(LEX_NUM, 0));
        F();
        check_minus();
    } else if (c_type == LEX_LPAREN) {
        gl();
        E();
        if (c_type == LEX_RPAREN)
            gl();
        else {
            cout << "RPARENERROR";
            cout << curr_lex;
            gl();
            cout << curr_lex;
            throw curr_lex;
        }
    } else if (c_type == LEX_SEMICOLON) {

    } else {
        cout << "Expression error";
        cout << curr_lex;
        gl();
        cout << curr_lex;
        throw curr_lex;
    }
}


void Parser::check_id() {
    if (TID[c_val].get_declare())
        st_lex.push(TID[c_val].get_type());
    else {
        cout << curr_lex;
        gl();
        cout << curr_lex;
        throw "not declared ID";
    }
}

void Parser::check_op() {
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;
    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();
    if (op == LEX_COLON)
        return;
    if (t1 == LEX_STR)
        t1 = LEX_STRING;
    if (t2 == LEX_STR)
        t2 = LEX_STRING;
    if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH)
        r = LEX_INT;
    if (op == LEX_OR || op == LEX_AND)
        t = LEX_BOOL;
    if (t1 == LEX_STRING && t2 == LEX_STRING && (
            op == LEX_EQ || op == LEX_LSS || op == LEX_GTR ||
            op == LEX_LEQ || op == LEX_GEQ || op == LEX_NEQ)) {
        t = LEX_STRING;
    }
    if (t1 == LEX_STRING && t2 == LEX_STRING && op == LEX_PLUS) {
        t = LEX_STRING;
        r = LEX_STRING;
    }
    if (t1 == t2 && op == LEX_ASSIGN) {
        t = t1;
        r = t1;
    }
//    cout << t1 << ' ' << op << ' ' << t2 << ' ' << t << endl;
    if (t1 == t2 && t1 == t) {
        st_lex.push(r);
        prog.put_lex(Lex(op));
    } else {
        cout << "wrong types are in operation";
        cout << curr_lex;
        gl();
        cout << curr_lex;
        throw "wrong types are in operation";
    }
}

void Parser::check_not() {
    if (st_lex.pop() != LEX_BOOL)
        throw "wrong type is in not";
    else {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_NOT));
    }
}
void Parser::check_minus() {
    if (st_lex.pop() != LEX_INT)
        throw "wrong type is in not";
    else {
        st_lex.push(LEX_INT);
        prog.put_lex(Lex(LEX_MINUS));
    }
}

void Parser::eq_type() {
    if (st_lex.pop() != st_lex.pop()) throw "wrong types are in =";
}

void Parser::eq_bool() {
    if (st_lex.pop() != LEX_BOOL) {
        cout << "expression is not boolean";
        cout << curr_lex;
        gl();
        cout << curr_lex;
        throw "expression is not boolean";
    }
}

void Parser::check_id_in_read() {
    if (!TID[c_val].get_declare())
        throw "not declared 1111";
}

void Parser::check_id_in_goto() {

}

class Executer {
    Lex pc_el;
public:
    void execute(Poliz &prog);
};

void Executer::execute(Poliz &prog) {
    Stack<int, 100> args;
    Stack<type_of_lex, 100> argt;
    int i, j, index = 0, size = prog.get_free();
    while (index < size) {
        pc_el = prog[index];
        switch (pc_el.get_type()) {
            case LEX_TRUE:
            case LEX_FALSE:
            case LEX_NUM:
            case LEX_STR:
            case POLIZ_ADDRESS:
            case POLIZ_LABEL:
                args.push(pc_el.get_value());
                argt.push(pc_el.get_type());
                break;
            case LEX_ID:
                i = pc_el.get_value();
                if (TID[i].get_assign()) {
                    args.push(TID[i].get_value());
                    argt.push(TID[i].get_type());
                    break;
                } else {
                    cout << TID[i].get_name();
                    throw "POLIZ: indefinite identifier";
                }
            case LEX_NOT:
                args.push(!args.pop());
                argt.push(argt.pop());
                break;
            case LEX_OR:
                i = args.pop();
                argt.pop();
                args.push(args.pop() || i);
                argt.push(argt.pop());
                break;
            case LEX_AND:
                i = args.pop();
                argt.pop();
                args.push(args.pop() && i);
                argt.push(argt.pop());
                break;
            case POLIZ_GO:
                index = args.pop() - 1;
                argt.pop();
                break;
            case POLIZ_FGO:
                i = args.pop();
                if (!args.pop())index = i - 1;
                argt.pop();
                argt.pop();
                break;
            case LEX_WRITE:
                if (argt.pop() == LEX_STRING) {
                    cout << TSTR[args.pop()] << endl;
                } else {
                    cout << args.pop() << endl;
                }
                break;
            case LEX_READ: {
                int k;
                i = args.pop();
                argt.pop();
                if (TID[i].get_type() == LEX_INT) {
                    cout << "Input int value for ";
                    cout << TID[i].get_name() << endl;
                    cin >> k;
                } else if (TID[i].get_type() == LEX_BOOL) {
                    char j[20];
                    rep:
                    cout << "Input boolean value;";
                    cout << "(true or false)for ";
                    cout << TID[i].get_name() << endl;
                    cin >> j;
                    if (!strcmp(j, "true"))
                        k = 1;
                    else if (!strcmp(j, "false"))
                        k = 0;
                    else {
                        cout << "Error in input:true/false";
                        cout << endl;
                        goto rep;
                    }
                } else {
                    cout << "Input string value for ";
                    cout << TID[i].get_name() << endl;
                    string helpread;
                    cin >> helpread;
                    k = put(helpread, 'S');
                }
                TID[i].put_value(k);
                TID[i].put_assign();
                break;
            }
            case LEX_PLUS:
                type_of_lex helppl;
                helppl = argt.pop();
                if (helppl == LEX_STRING || helppl == LEX_STR) {
                    string helps;
                    helps = TSTR[args.pop()] + TSTR[args.pop()];
                    args.push(put(helps, 'S'));
                } else {
                    args.push(args.pop() + args.pop());
                }
                break;
            case LEX_TIMES:
                args.push(args.pop() * args.pop());
                argt.pop();
                break;
            case LEX_MINUS:
                i = args.pop();
                argt.pop();
                args.push(args.pop() - i);
                break;
            case LEX_SLASH:
                i = args.pop();
                argt.pop();
                if (!i) {
                    args.push(args.pop() / i);
                    break;
                } else
                    throw "POLIZ:divide by zero";
            case LEX_EQ:
                i = args.pop();
                argt.pop();
                if (argt.pop() == LEX_STRING) {
                    args.push(TSTR[args.pop()] == TSTR[i]);
                } else {
                    args.push(args.pop() == i);
                }
                argt.push(LEX_BOOL);
                break;
            case LEX_LSS:
                i = args.pop();
                argt.pop();
                if (argt.pop() == LEX_STRING) {
                    args.push(TSTR[args.pop()] < TSTR[i]);
                } else {
                    args.push(args.pop() < i);
                }
                argt.push(LEX_BOOL);
                break;
            case LEX_GTR:
                i = args.pop();
                argt.pop();
                if (argt.pop() == LEX_STRING) {
                    args.push(TSTR[args.pop()] > TSTR[i]);
                } else {
                    args.push(args.pop() > i);
                }
                argt.push(LEX_BOOL);
                break;
            case LEX_LEQ:
                i = args.pop();
                argt.pop();
                argt.pop();
                argt.push(LEX_BOOL);
                args.push(args.pop() <= i);
                break;
            case LEX_GEQ:
                i = args.pop();
                argt.pop();
                argt.pop();
                argt.push(LEX_BOOL);
                args.push(args.pop() >= i);
                break;
            case LEX_NEQ:
                i = args.pop();
                argt.pop();
                if (argt.pop() == LEX_STRING) {
                    args.push(TSTR[args.pop()] != TSTR[i]);
                } else {
                    args.push(args.pop() != i);
                }
                argt.push(LEX_BOOL);
                break;
            case LEX_ASSIGN:
                i = args.pop();
                j = args.pop();
                argt.pop();
                args.push(i);
                TID[j].put_value(i);
                TID[j].put_assign();
                break;
            default:
                throw "ERROR";
        } // end of switch
        ++index;
    }; //end of while
    cout << "Finish of executing!!!" << endl;
}

class Interpretator {
    Parser pars;
    Executer E;
public:
    Interpretator(char *program) : pars(program) {};

    void interpretation();
};

void Interpretator::interpretation() {
    pars.analyze();
    E.execute(pars.prog);
}

int main() {
    try {
        Interpretator I((char *) "test.txt");
        I.interpretation();
        return 0;
    }
    catch (char c) {
        cout << "unexpected symbol " << c << endl;
        return 1;
    }
    catch (Lex l) {
        cout << "unexpected lexeme";
        cout << l;
        return 1;
    }
    catch (const char *source) {
        cout << source << endl;
        return 1;
    }
}


//int main() {
//    try {
//        Scanner scan("test.txt");
//        Lex lex = scan.get_lex();
//        while (lex.get_type() != LEX_FIN) {
//            cout << lex ;
//            lex = scan.get_lex();
//        }
////        Parser analyz("test.txt");
////        analyz.analyze();
//    }
//    catch (char c) {
//        cout << "symbol" << c << "ERROR" << endl;
//        return 1;
//    }
//    catch (Lex lex1) {
//        cout << "lex" << lex1 << "----ERROR" << endl;
//        return 1;
//    }
//    catch (const char *s) {
//        cout << s << endl;
//        return 1;
//    }
//    for (int i = 0; i < TSTR.size(); i++){
//        cout << TSTR[i] << endl;
//    }
//}

