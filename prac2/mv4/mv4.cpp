#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
//void gc(FILE *fp) { c = fgetc(fp); }
bool scan_G(){
    enum state {H, A, B, C, S, BS, AB ,AS, ER, FIN};
    state CS;
    CS = H;
    char c;
    do {
//        gc();
        cin >> c;
        switch (CS) {
            case H:
                cout << "H ";
                if (c == '0') {
                    CS = C;
                } else
                    CS = ER;
                break;
            case A:
                cout << "A ";
                if (c == '0') {
                    CS = BS;
                } else
                    CS = ER;
                break;
            case B:
                cout << "B ";
                if (c == '1') {
                    CS = A;
                } else
                    CS = ER;
                break;
            case C:
                cout << "C ";
                if (c == '0') {
                    CS = C;
                } else if (c == '1') {
                    CS = AB;
                } else
                    CS = ER;
                break;
            case S:
                cout << "S ";
                if (c == '1') {
                    CS = S;
                } else if (c == '@') {
                    CS = FIN;
                } else
                    CS = ER;
                break;
            case BS:
                cout << "BS ";
                if (c == '1') {
                    CS = AS;
                } else if (c == '@') {
                    CS = FIN;
                } else
                    CS = ER;
                break;
            case AB:
                cout << "AB ";
                if (c == '1') {
                    CS = A;
                } else if (c == '0') {
                    CS = BS;
                } else if (c == '@') {
                    CS = FIN;
                } else
                    CS = ER;
                break;
            case AS:
                cout << "AS ";
                if (c == '0'){
                    CS = BS;
                } else if (c == '1'){
                    CS = S;
                } else if (c == '@'){
                    CS = FIN;
                } else
                    CS = ER;
                break;
        }
    } while ((CS != ER) && (CS != FIN));
    return CS == FIN;
};

int main(){
//    FILE *fp = fopen(program,"r");
    cout << scan_G() << endl;
}
