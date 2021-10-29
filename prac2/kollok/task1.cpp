#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <typeinfo>
using namespace std;
void f (float , double ) { cout << "1";}

void f (double , float ) { cout << "2";}

void f ( long , short ) { cout << "4";}
int main(){
    f (5L, 5.0)
}