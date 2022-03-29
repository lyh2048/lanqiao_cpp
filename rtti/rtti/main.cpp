#include <iostream>
#include <memory>
#include <ctime>
#include <vector>

#include "rtti.h"

using namespace rtti;
using namespace std;

class A : public rtti_port<A> {};
class B : public rtti_port<B> {};
class C : public rtti_port<C> {};
class D : public rtti_port<D> {};
class E : public rtti_port<E> {};

class AB : public virtual A, public virtual B, public rtti_port<AB> {};
class CD : public virtual C, public virtual D, public rtti_port<CD> {};
class AB_CD : public virtual AB, public virtual CD, public rtti_port<AB_CD> {};
class AB_CD_E : public virtual AB, public virtual CD, public virtual E, public rtti_port<AB_CD_E> {};


void test_speed() {
    std::time_t rtti_t = 0;
    std::time_t dyncast_t = 0;
    const int times = 10;
    const int size = 80000;

    for (int a = 0; a < times; a++) {
        std::vector<std::shared_ptr<A>> vec;
        for (int i = 0; i < size; i++)
            vec.push_back(std::make_shared<AB_CD_E>());
        std::time_t t = clock();
        for (int i = 0; i < size; i++)
            AB_CD_E* obj = cast<AB_CD_E>(vec[i].get());
        rtti_t += clock() - t;

        t = clock();
        for (int i = 0; i < size; i++)
            AB_CD_E* obj = dynamic_cast<AB_CD_E*>(vec[i].get());
        dyncast_t += clock() - t;
        vec.clear();
    }
    std::cout << "avg time of rtti_lib: " << rtti_t / times << std::endl;
    std::cout << "avg time of dynamic_cast: " << dyncast_t / times << std::endl;
}

void test() {
    std::cout.setf(std::ios::boolalpha);

    // ������� AB_CD_E, �ö���̳��� AB,CD,E, ���� AB �� CD �ֱ�̳��� A,B �� C,D
    AB_CD_E* obj1 = new AB_CD_E();

    // ʹ�� `rttilib::is` �������ж� AB_CD_E �����Ƿ��� A ���͵Ķ���, true
    cout << "Is object of A type? " << is<A>(obj1) << endl;
    // �ж� AB_CD_E �����Ƿ��� E ���͵Ķ���, true
    cout << "Is object of E type? " << is<E>(obj1) << endl;
    delete obj1;
    cout << endl;

    A* obj2 = new AB_CD_E();
    // �ж� A ��ָ��ָ��� AB_CD_E �����Ƿ��� A ���͵Ķ���, true
    cout << "Is object of A type? " << obj2->isKindOf(rtti_port<A>::type()) << endl;
    // �ж� A ��ָ��ָ��� AB_CD_E �����Ƿ��� E ���͵Ķ���, true
    cout << "Is object of E type? " << obj2->isKindOf(rtti_port<E>::type()) << endl;
    delete obj2;
    cout << endl;


    AB* obj3 = new AB();
    // �ж� AB ��ָ��ָ��� AB �����Ƿ��� A ���͵Ķ���, true
    cout << "Is object of A type? " << obj3->isKindOf(rtti_port<A>::type()) << endl;
    // �ж� AB ��ָ��ָ��� AB �����Ƿ��� E ���͵Ķ���, false
    cout << "Is object of E type? " << obj3->isKindOf(rtti_port<E>::type()) << endl;
    delete obj3;
    cout << endl;

    CD* obj4 = new CD();
    // �ж� CD ��ָ��ָ��� CD �����Ƿ��� A ���͵Ķ���, false
    cout << "Is object of A type? " << obj4->isKindOf(rtti_port<A>::type()) << endl;
    // �ж� CD ��ָ��ָ��� CD �����Ƿ��� E ���͵Ķ���, false
    cout << "Is object of E type? " << obj4->isKindOf(rtti_port<E>::type()) << endl;
    delete obj4;
    cout << endl;
}


int main()
{
    test_speed();
    test();
	return 0;
}