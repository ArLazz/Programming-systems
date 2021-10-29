#include <iostream>
#include <cstring>
using namespace std;


//Вариант №3 - Лазарев Артур - 211
//Элемент данных – объект «банковский счет». Необходимые члены-данные: номер
//        счета, владелец счета, дата создания счета (число или строка), сумма денег,
//        которая на нем хранится. Нумерация счетов единая для всех очередей (списков).



//2. АТД. Очередь для хранения данных.
//На основе класса Список_элементов_произвольного_типа (использовать класс из ДЗ-2)
//определить класс Очередь, который должен быть производным от Списка.
//Элемент данных, помещаемых в очередь — см. вариант задания.
//Максимальный допустимый размер очереди определен по умолчанию, а также может
//        явно указываться при создании объекта-Очереди.
//Определить необходимые конструкторы.
//Обеспечить корректное уничтожение объектов.
//В классе должны быть функции:
//добавления элемента в конец очереди (back),
//        чтение первого элемента из очереди без его удаления (front) ,
//        удаление первого элемента очереди (pop),
//        проверка очереди на пустоту (empty),
//        текущее число элементов (size),
//        проверка, что очередь целиком заполнена (full).
//вывод информации об элементах очереди без ее изменения (print).
//Замечания:
//Хотелось бы, чтобы класс список можно было использовать для хранения любых
//данных. Поэтому нужно выделить данные в отдельный класс.
//Функции класса Список должны остаться прежними, а не переделываться под
//        потребности очереди.
//Через объект Очередь должны быть доступны только функции, указанные в задании. При
//        этом базовый класс остается полноценным списком.
//Глобальных переменных быть НЕ должно.
//Использование STL и типа string запрещено.


class Elem{
private:
    int number, sum;
    char* name, *date;
    Elem *prev, *next;
    friend class List;
public:
    Elem(const int number1, const char* name1, const char* date1, const int sum1) : prev(0), next(0){
        number = number1;
        sum = sum1;
        name = new char[strlen(name1) + 1];
        strcpy(name, name1);
        date = new char[strlen(date1) + 1];
        strcpy(date, date1);
    }
    Elem(const Elem &e){
        if (this != &e){
            number = e.number;
            sum = e.sum;
            name = new char[strlen(e.name) + 1];
            strcpy(name, e.name);
            date = new char [strlen(e.date) + 1];
            strcpy(date, e.date);
        }
    }
    ~Elem(){
        delete[] date;
        delete[] name;
        number = 0;
        sum = 0;
    }
};


class List{
private:
    Elem *head, *first, *last, *hel = 0;
    List *next1;
    int count = 0;
    friend class Queue;
public:
    List();
    List(const List &l);
    List(const int number1, const char* name1, const char* date1, const int sum1);
    ~List();
    void push_front(const int number1, const char* name1, const char* date1, const int sum1);//добавления элемента в начало
    void push_back(const int number1, const char* name1, const char* date1, const int sum1);//добавления элемента в конец
    int front();//чтение первого элемента списка
    int back();//чтение последнего элемента списка
    void pop_front();//удаление первого элемента списка
    void pop_back();//удаление первого элемента списка
    void insert(const int p, const int number1, const char* name1, const char* date1, const int sum1);//добавление элемента x перед позицией p
    void erase(const int p);//удаление элемента из позиции p
    void empty();// проверка списка на пустоту
    int size();//текущее число элементов
    void print();//вывод информации об элементах списка в прямом порядке
    void printback();//вывод информации об элементах списка в обратном порядке
};

List::List(): next1(nullptr){
    head = new Elem(0,"","", 0);
    first = last = head;
    count = 1;
}

List::List(const List &l): next1(0) {
    if (this != &l) {
        count = l.count;
        hel = l.first;
        head = new Elem(*hel);
        first = last = head;
        hel = hel->next;
        while (hel) {
            head = new Elem(*hel);
            last->next = head;
            head->prev = last;
            last = head;
            hel = hel->next;
        }
        last -> next = nullptr;
    }
}

List::List(const int number1, const char* name1, const char* date1, const int sum1): next1(nullptr){
    head = new Elem(number1, name1, date1, sum1);
    first = last = head;
    count = 1;
}

List::~List(){
    while (first != last){
        head = first;
        first = first -> next;
        delete head;
    }
    count = 0;
}

void List::push_front(const int number1, const char* name1, const char* date1, const int sum1) {
    head = new Elem(number1, name1, date1, sum1);
    head -> next = first;
    first = head;
    count += 1;
}

void List::push_back(const int number1, const char* name1, const char* date1, const int sum1) {
    head = new Elem(number1, name1, date1, sum1);
    last -> next = head;
    head -> prev = last;
    last = head;
    count += 1;
}

int List::front() {
    return first -> number;
}

int List::back() {
    return last -> number;
}

void List::pop_front() {
    if (count == 0){
        return;
    } else if (count == 1){
        delete first;
        count -= 1;
    } else {
        head = first -> next;
        head -> prev = 0;
        delete first;
        first = head;
        count -= 1;
    }
}

void List::pop_back() {
    if (count == 0){
        return;
    } else if (count == 1){
        delete first;
        count -= 1;
    } else {
        head = last->prev;
        head->next = 0;
        delete last;
        last = head;
        count -= 1;
    }
}

void List::insert(const int p, const int number1, const char* name1, const char* date1, const int sum1) {
    head = first;
    for (int i = 0; i < p; i++) {
        head = head->next;
    }
    Elem *help = new Elem(number1, name1, date1, sum1);
    help->prev = head->prev;
    help->next = head;
    head -> prev = help;
    if (help -> prev) {
        head = help->prev;
        head->next = help;
    } else {
        first = help;
    }
    count += 1;
}

void List::erase(const int p) {
    if (p > count){
        cout << "Введено число больше,чем элементов." << endl;
        return;
    } else if (count == 0){
        return;
    } else if (count == 1){
        delete first;
        count -= 1;
    } else {
        head = first;
        for (int i = 0; i < p; i++) {
            head = head->next;
        }
        head->prev->next = head->next;
        delete head;
        count -= 1;
    }
}

void List::empty() {
    if (count == 0){
        cout << "Список пуст." << endl;
    } else {
        cout << "Список не пуст." << endl;
    }
}

int List::size() {
    return count;
}

void List::print() {
    cout << "Кол-во элементов списка: " << count << endl;
    head = first;
    while (head){
        cout << "Номер счета: " << head -> number << "; Владелец счета: " << head -> name << "; Дата создания счета: ";
        cout << head -> date << "; Сумма денег: " << head -> sum << ";  " << endl;
        head = head -> next;
    }
    cout << endl;
}

void List::printback() {
    cout << "Кол-во элементов: " << count << endl;
    head = last;
    while (head){
        cout << head -> number << "  " << head -> name << "  " << head -> date << "  " << head -> sum << "  ";
        head = head -> prev;
    }
    cout << endl;
}

class Queue : private List{
private:
    List* head1,*first1; // head1 - вспомогательный объект, first1 - указатель на начало очереди
    int count1 = 0;//текущее количество элементов в очереди
    int size_of_queue = 0;// максимальное количество элементов в очереди
public:
    Queue();//конструктор
    Queue(const int size_of_queue1);//конструктор
    ~Queue();//деструктор
    void back(List l);//добавления элемента в конец очереди
    List front();//чтение первого элемента из очереди без его удаления
    void pop();//удаление первого элемента очереди
    void empty();//проверка очереди на пустоту
    int size();//текущее число элементов
    void full();//проверка, что очередь целиком заполнена
    void print();//вывод информации об элементах очереди без ее изменения
};


Queue::Queue() {
    head1 = new List();
    first1 = head1;
    count1 = 1;
    size_of_queue = 5;//размер очереди по умолчанию - 5;
}

Queue::Queue(const int size_of_queue1) {
    head1 = new List();
    first1 = head1;
    count1 = 1;
    size_of_queue = size_of_queue1;
}

Queue::~Queue() {
    cout << "Работает деструктор очереди." << endl;
    while (first1){
        head1 = first1;
        first1 = first1 -> next1;
        delete head1;
    }
    count1 = 0;
    size_of_queue = 0;
}

void Queue::back(List l) {
    if (count < size_of_queue) {
        cout << "Работает back. " << endl;
        head1 = first1;
        while (head1->next1) {
            head1 = head1->next1;
        }
        List *li = new List(l);
        head1->next1 = li;
        count1 += 1;
    } else {
        cout << "Достигнуто максимально количество элементов" << endl;
    }
}

void Queue::print() {
    cout << "Вывод элементов очереди, количество -  " << count1 << endl;
    head1 = first1;
    while(head1){
        List l = *head1;
        l.print();
        head1 = head1 -> next1;
    }
    cout << endl;
}

List Queue::front() {
    cout << "Работает front. " << endl;
    return *first1;
}

void Queue:: pop(){
    cout << "Работает pop. " << endl;
    head1 = first1;
    first1 = first1 -> next1;
    delete head1;
    count1 -= 1;
}

void Queue::empty() {
    cout << "Работает empty. " << endl;
    if (count1 == 0){
        cout << "Очередь пуста" << endl;
    } else {
        cout << "Очередь не пуста" << endl;
    }
}

int Queue::size() {
    cout << "Работает size. " << endl;
    return count1;
}

void Queue::full() {
    cout << "Работает full. " << endl;
    if (count1 == size_of_queue){
        cout << "Очередь заполнена" << endl;
    } else {
        cout << "Очередь не заполнена" << endl;
    }
}

int main(){
    List l1, l2;
    l1.push_front(1234, "number1", "04.04.04", 1000);// заполнение списка 1
    l1.push_back(1111, "number2", "01.01.01", 10000);
    l1.print();
    cout << endl;

    l2.push_front(1111111, "number3", "02.12.11", 2121);// заполнение списка 2
    l2.push_back(10001, "number4", "02.12.1211", 21211111);
    l2.push_back(1111,"12", "213", 2121);
    l2.print();
    cout << endl;

    Queue q1(5);
    q1.back(l1);//добавления элемента в конец очереди
    q1.back(l2);//добавления элемента в конец очереди
    q1.print();//вывод информации об элементах очереди без ее изменения
    cout << endl;
    cout << endl;

    List l4 = q1.front();//чтение первого элемента из очереди без его удаления
    l4.print();
    cout << endl;
    cout << endl;

    q1.pop();//удаление первого элемента очереди
    cout << endl;
    q1.print();//вывод информации об элементах очереди без ее изменения

    q1.empty();//проверка очереди на пустоту
    cout << "Текущее число элементов: " << q1.size() << endl;
    q1.full();//проверка, что очередь целиком заполнена

}