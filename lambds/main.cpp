#include<iostream>
#include<functional>

class A; // Чтобы ф. ниже понимала, о каких переменных идёт речь
void ExternalCaller(A& z, A& x, std::function<bool(A& q, A& w)>func);

class A {
public:
	int a;

	int GetA()const { return a; }
	void SetA(int a) { this->a = a; }
	
	void LambdaExample() {
		A tmp; // Временная переменная класс А
		tmp.a = 86;
		auto lFunc = [this](A& q, A& w)mutable->bool { 
			if (q.a <= w.a) {
				this->SetA(w.a);
			}
			return q.a > w.a;
			};
		ExternalCaller(*this, tmp, lFunc); 
	}
	// this захватывается для передачи объекта класса в лямбду
};

void ExternalCaller(A& z, A& x, std::function<bool(A& q, A& w)>func) {
	if (func(z, x)) 
		std::cout << z.a << " greater\n";	
	else	
		std::cout << x.a << " greater\n";	
};

// Лямбда выражения как правило используются для захвата данных типа класса
int main() {
	std::pair<A*, A*>lastCompare;
	//[lastCompare](A& q, A& w)->bool {}; // Параметрический захват контеста
	//[&](A& q, A& w)->bool {}; // Всё, что было до лямбы будет доступен в контексте
	//[lastCompare](A& q, A& w)mutable->bool {}; // Изменение параметров, которые захвачены в контексте
	auto lFunc = [&lastCompare](A& q, A& w)mutable->bool { // В захват контекста добавится только ссылка на lastCompare
		lastCompare.first = &q;
		lastCompare.second = &w;
		return q.a > w.a;
		};
	A f;
	A t;
	f.SetA(8);
	t.a = 10;
	ExternalCaller(f, t, lFunc);
	std::cout << "Last compare:\n" << lastCompare.first << "with value - " << lastCompare.first->a << '\n';
	std::cout << lastCompare.second << "with value - " << lastCompare.second->a << '\n';
	
	t.LambdaExample();
	std::cout << t.a;

	return 0;
}
// Захват контекста - захват [] переменных, они передаются в лямбда-выражение (ссылка/значение).
// Если указать [&], то для изменения будут доступны все переменные в программе до лямбды.
// 