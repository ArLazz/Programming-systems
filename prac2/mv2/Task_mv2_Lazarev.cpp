#include <iostream>
#include <cstring>
#include <typeinfo>
using namespace std;

class Circle{
public:
    double x, y ,r;//координаты центра и радиус
    static int count_c;//счетчик объектов тип круг
    Circle(double x1 = 1, double y1 = 1, double r1 = 1):x(x1),y(y1),r(r1){//конструктор
        count_c += 1;
    }
    int Circle_count(){//возврат кол-ва объектов типа круг
        return count_c;
    };
    virtual print(){//вывод информации об объекте
        cout << "Circle " << "with center in: (" << x << ':' << y << ") and R = " << r << '.' << endl;
    }
    virtual zoom(int index){//увелечение круга в index раз
        r *= index;
    }
    double virtual square(){//площадь круга
        return r * r * 3.14;
    }
    ~Circle(){//деструктор
        count_c -= 1;
    }
};

class Ring: public Circle{
public:
    double ri;//радиус внутреннего круга
    static int count_r;//счетчик кол-ва объектов типа кольцо
    Ring(double x1 = 1, double y1 = 1, double r1 = 1, double ri1 = 0):Circle(x1,y1,r1),ri(ri1){//конструктор
        try{
            if (r < ri)
                throw 1;
        }
        catch (int){ri = r1;}
        count_c -=1;
        count_r += 1;
    }
    virtual print(){//вывод информации об объекте
        cout << "Ring " << "with center in: (" << x << ':' << y << "), R1 = " << ri << " and R2 = " << r << '.' << endl;
    }
    int Ring_count(){//вывод кол-ва объектов типа кольцо
        return count_r;
    }
    virtual zoom(int index){//увелечение колца в index раз
        r *= index;
        ri *= index;
    }
    double virtual square(){//площадь кольца
        return (r * r - ri * ri)*3.14;
    }
    ~Ring(){//деструтор
        count_r -=1;
    }
};

class Help{//класс-функция для увелечения объектов типа Circle и Ring
public:
    int index;
    Help(int index1 = 2):index(index1){};
    Circle& operator[](Circle &a) {
        a.zoom(index);
        return a;
    }
};

ptr_to(Circle *a){//вывод на экран тип объекта, на которую указывает полученный указатель
    cout << typeid(*a).name() << endl;
}

int Circle :: count_c=0;
int Ring :: count_r=0;

int main(){
    Circle c1, c2(1,2,3);//создание объектов
    Ring r1, r2(7, 8, 10, 9);//создание объектов
    c1.print();//вывод информации
    c2.print();//вывод информации
    r1.print();//вывод информации
    r2.print();//вывод информации
    cout << c1.Circle_count() << "- кол-во кругов." << endl << r1.Ring_count() << " - кол-во колец." << endl;//вывод кол-ва объектов
    ptr_to(&c1);//проверка функции, которая выводит на экран тип объекта, на которую указывает полученный указатель
    ptr_to(&r1);//проверка функции, которая выводит на экран тип объекта, на которую указывает полученный указатель
    Help help, help1(3);//создание объекта класса-функции
    help [c1];//увелечение объекта типа круг
    help1 [r2];//увелеченпие объекта типа кольцо
    c1.print();//вывод информации
    r2.print();//вывод информации
    cout << "Площадь 1 круга:" << c1.square() << ". Площадь 2 круга:" << c2.square() << endl;//вывод площади
    cout << "Площадь 1 кольца:" << r1.square() << ". Площадь 2 кольца:" << r2.square() << endl;//вывод площади

}