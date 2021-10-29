#include <iostream>
#include <cstring>
using namespace std;
class Elem{
private:
    Elem(const Elem &);
    int data;
    Elem *prev, *next;
    friend class List;
public:
    Elem(const int data1) : prev(0), next(0){
        data = data1;
    }
    ~Elem(){
        data = 0;
    }
};

class List{
private:
    Elem *head, *first, *last;
    int count = 0;
public:
    List();
    List(const int data);
    ~List();
    void push_front(const int data);//добавления элемента в начало
    void push_back(const int data);//добавления элемента в конец
    int front();//чтение первого элемента списка
    int back();//чтение последнего элемента списка
    void pop_front();//удаление первого элемента списка
    void pop_back();//удаление первого элемента списка
    void insert(const int p, const int x);//добавление элемента x перед позицией p
    void erase(const int p);//удаление элемента из позиции p
    void empty();// проверка списка на пустоту
    int size();//текущее число элементов
    void print();//вывод информации об элементах списка в прямом порядке
    void printback();//вывод информации об элементах списка в обратном порядке
};

List::List(){
    head = new Elem(0);
    first = last = head;
    count = 1;
    cout << "Работает конструктор с пустыми входными данными = 0." << endl;
}

List::List(const int data){
    cout << "Работает конструктор с начальными значениями." << endl;
    head = new Elem(data);
    first = last = head;
    count = 1;
}

List::~List(){
    cout << "Работает деструктор листа." << endl;
    while (first != last){
        head = first;
        first = first -> next;
        delete head;
    }
    count = 0;
}
void List::push_front(const int data) {
    cout << "Работает push_front." << endl;
    head = new Elem(data);
    head -> next = first;
    first = head;
    count += 1;
}

void List::push_back(const int data) {
    cout << "Работает push_back." << endl;
    head = new Elem(data);
    last -> next = head;
    head -> prev = last;
    last = head;
    count += 1;
}

int List::front() {
    cout << "Работает front." << endl;
    return first -> data;
}

int List::back() {
    cout << "Работает back." << endl;
    return last -> data;
}

void List::pop_front() {
    cout << "Работает pop_front." << endl;
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
    cout << "Работает pop_back." << endl;
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
void List::insert(const int p, const int x) {
    cout << "Работает insert c агрументами: " << p << ' ' << x << '.' << endl;
    head = first;
    for (int i = 0; i < p; i++) {
        head = head->next;
    }
    Elem *help = new Elem(x);
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
    cout << "Работает erase c агрументом: " << p << '.' << endl;
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
    cout << "Работает empty." << endl;
    if (count == 0){
        cout << "Список пуст." << endl;
    } else {
        cout << "Список не пуст." << endl;
    }
}

int List::size() {
    cout << "Работает size." << endl;
    return count;
}

void List::print() {
    cout << "Работает print." << endl;
    cout << "Кол-во элементов: " << count << endl;
    head = first;
    while (head){
        cout << head -> data << "  ";
        head = head -> next;
    }
    cout << endl;
}
void List::printback() {
    cout << "Работает printback." << endl;
    cout << "Кол-во элементов: " << count << endl;
    head = last;
    while (head){
        cout << head -> data << "  ";
        head = head -> prev;
    }
    cout << endl;
}
class Queue : private List {
private:
    List* head1,*first1;
    int count1 = 0;
    int size_of_queue1 = 0;
public:
    Queue();
    Queue(const int size_of_queue);
    ~Queue();
};

int main(){
    List l1;

}