#include <iostream>
#include <cstring>
using namespace std;
class Figure {
public:
    virtual double square() = 0;//площадь
    virtual double perimeter() = 0;//периметр
    virtual void name() = 0;//вывод информации
};
class Box: public Figure{
private:
    double side;//сторона квадрата
public:
    Box(double s){
        side = s;
    }
    virtual double square() override{//площадь
        return side * side;
    }
    virtual double perimeter() override{//периметр
        return side * 4;
    }
    virtual void name() override{//вывод информации
        cout << "Квадрат со стороной:" << side << "см., с площадью: " << square();
        cout << "см^2, с периметром: " << perimeter() << "см." << endl;
    }
};
class Circle: public Figure{
private:
    double radius;//радиус круга
public:
    Circle(double r){
        radius = r;
    }
    virtual double square() override{//площадь
        return radius * radius * 3.14;
    }
    virtual double perimeter() override{//периметр
        return 2 * 3.14 * radius;
    }
    virtual void name() override{//вывод информации
        cout << "Окружность с радиусом:" << radius << "см., с площадью: " << square();
        cout << "см^2, с периметром: " << perimeter() << "см." << endl;
    }
};

class Math {
public:
    Figure **f;
    Math(int lenB, int lenC){//конструктор класса,который работает  массивом объектов типа абстрактного класса
        f = new Figure *[lenB + lenC];//lenB - кол-во квадратов,lenC - кол-во окружностей
        for (int i = 0; i < lenB; i++){
            f[i] = new Box(i);
        }
        for (int i = lenB; i < lenC + lenB; i++){
            f[i] = new Circle(i);
        }
    }
};
int main(){
    Math m(2,2);
    for (int i = 0; i < 4; i++){
        (m.f[i])->name();//вывод информации об элементах массива
    }
}