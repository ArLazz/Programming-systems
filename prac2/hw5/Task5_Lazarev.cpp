#include <iostream>
#include <cstring>
using namespace std;

class Matrix{
public:
    int row;//кол-во строк
    int column;//кол-во столбцов
    double **M;//сама матрица

    Matrix (int i, int j, int data = 0) {//конструктор,где i - кол-во строк, j - кол-во столбцов, data - элементы массива
        row = i;
        column = j;
        M = new double *[row];

        for (int i = 0; i < row; i++) {
            M[i] = new double[column];
            for (int j = 0; j < column; j++) {
                M[i][j] = data;
            }
        }
    }

    double* operator [] (int i) {return M[i];}//перегрузка оператора индексации

    void print() const{//функция вывода
        for (int i = 0; i < row; i++){
            for (int j = 0; j < column; j++){
                cout << M[i][j] << "  ";
            }
            cout << endl;
        }
    }

    ~Matrix(){//деструктор
        for (int i = 0; i < row; i++){
            delete [] M[i];
        }
        delete [] M;
    }
};

int main(){
    Matrix m(4,5,4);//создаем матрицу
    m[1][2] = 5;//инициализируем элемент
    int x = m[2][3];
    m.print();//вывод матрицы
    cout << x << "   " << m[2][3] << endl;
    const Matrix mc(3,4,3);
    mc.print();//вывод матрицы
//    cout<< mc[1][1];

}


