#include <iostream>
#include <cstring>
using namespace std;

class Arr {
    int *arr;//наш массив
    int size;//размер
public:
    Arr(int s, int q = 0) : size(s) {//конструктор с первым параметром - размер массива,
        // вторым - чем заполняем(по умолчанию - 0)
        arr = new int[s];
        for (int i = 0; i < s; arr[i++] = q);
    }

    Arr(Arr &&x) : size(x.size) {//конструктор копирования
        arr = x.arr;
        x.arr = nullptr;
    }

    ~Arr() {//деструтор
        delete[] arr;
    }

    int arr_size() const {//функция возврата размера
        return size;
    }

    int arr_number(int i) const {//функция возврата элемента под номером i
        if (i < size) {
            return arr[i];
        } else {
            return 0;
        }
    }

    void arr_new(int i, int q) {//функция инициализации элемента под номером i
        if (i < size) {
            arr[i] = q;
        }
        return;
    }

    friend ostream &operator<<(ostream &s, const Arr &x) {//перегрузка оператора из библиотеки ostream,
                                                          // для того,чтобы мы могли выводить элементы массива с помошью cout
        for (int i = 0; i < x.arr_size(); i++) {
            s << x.arr_number(i) << " - ";
        }
        s << endl;
        return s;
    }

    int &operator[](int i){ //перегрузка оператора []
        return this->arr[i];
    }

    int operator[](int i) const {//перегрузка оператора []
        if (i < size){
            return arr[i];
        }else{
            return 0;
        }
    }


    Arr &operator=(Arr &&x) {//перегрузка оператора =
        if (this != &x) {
            delete[]this->arr;
            size = x.size;
            arr = x.arr;
            x.arr = nullptr;
        }
        return *this;
    }

    Arr operator!() {//перегрузка оператора !
        Arr obj(this->arr_size());//инициализируем массив с таким же размером
        for (int i = 0; i < this->arr_size(); i++) {
            obj.arr[i] = - this->arr_number(i);//заменяем элменты на противоположные
        }
        return obj; //возвращаем переделанный массив
    }

    Arr operator&(const Arr &x) const {//перегрузка оператора &
        Arr z(this->arr_size() + x.arr_size());//инициализируем массив с суммой размеров
        for (int i = 0; i < this->arr_size(); i++) {
            z.arr_new(i, this->arr_number(i));//заполняем первым массивом
        }
        for (int i = 0; i < x.arr_size(); i++) {
            z.arr_new(i + this->arr_size(), x.arr_number(i));//заполняем вторым массивом
        }
        return z;//возвращаем переделанный массив
    }

};

int main() {
    Arr a1(5), a2(10, 1), a3(4, 2);
    const Arr a4(5);
    cout << a1 << a2 << a3 << a4 << endl;
    a2 = a2 & 2;
    a3 = !(a2) & a3;
    a1 = a4 & a3;
    a2[3] = 123;
    a1[1] = 100;
    cout << a1 << a2 << a3 << a4 << endl;
}