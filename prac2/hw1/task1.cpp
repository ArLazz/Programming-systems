#include <iostream>
#include <cstring>
using namespace std;
class First {
private:
    int* array;
    int size;
public:
    First(){
        cout << "Работает конструктор с пустыми входными данными." << endl;
        size = 0;
        array = 0;
    }
    First(int size1){
        cout << "Работает конструктор с начальными значениями = 0." << endl;
        size = size1;
        array = new int [size1];
        for (int i = 0; i < size1; i++){
            array[i] = 0;
        }
    }
    ~First(){
        cout << "Работает деструктор." << endl;
        delete [] array;
    }
    First(const First &ar){
        cout << "Работает конструктор копирования." << endl;
        if (this != &ar){
            size = ar.size;
            array = new int[size];
            for (int i = 0; i < size; i++){
                array[i] = ar.array[i];
            }
            size = ar.size;
        }
    }
    void setarr(){
        cout << "Работает setarr." << endl;
        for (int i = 0; i < size; i++){
            cin >> array[i];
        }
    }
    void show(){
        cout << "Работает show." << endl;
        if (size == 0){
            cout << "Пустой массив.";
        }
        for (int i = 0; i < size; i++){
            cout << array[i] << "; ";
        }
        cout << endl;
    }
    First& operator=(First &array2){
        cout << "Работает оператор присваивания." << endl;
        if (this != &array2){
            delete []this->array;
            this->size = array2.size;
            this->array = new int [this->size];
            for (int i = 0; i < size; i++){
                this->array[i] = array2.array[i];
            }
        }
        return *this;
    }

};
int main(){
    First arr(4), arr1(4), arr2;//объявление
    arr.setarr();//заполнение
    arr.show();//показ
    cout << endl;
    First arr3 = arr;//вызов конструктора копирования
    arr3.show();//показ
    cout << endl;
    arr1 = arr;//вызов оператора присваивания
    arr1.show();// показ
    cout << endl;
    arr2.show();//вызов показа для пустого объекта
    cout << endl;
    First *a = new First(arr);
    delete a;//вызов деструктора
    arr.show();
}

