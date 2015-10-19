/**
 *автор: Демидов Александр
 *учебное заведение: МФТИ(ГУ) ФИВТ 296
 *направление: Прикладная Математика и Информатика
 *
 *отборочное задание на стажировку
 *в поисковом направлении 
 *в группе зарубежного ранжирования
 *Яндекс, Москва
 *
 *используемая среда разработки: Microsoft VS C++ 2010
 *используемые средства: stl
 *
 *содержание файла: тестовая программа для очереди с приоритетами,
 *                  реализованной в priority_queue.h
 */

#include <iostream>
#include <vector>
#include <cstdlib>

#include "priority_queue.h"


const int TEST_ELEMENTS_NUMBER = 1000;
const int VALUES_NUMBER = 200;
const int RAND_SEED = 283;


void initRandom() {
	std::srand(RAND_SEED);
}

// заполняет вектор случайными числами
void randFillVector(std::vector<int>& vector) {
	for (size_t position = 0; position < vector.size(); ++position) {
		vector[position] = rand() % VALUES_NUMBER + 1;
	}
} 

// проверяет корректность работы PriorityQueue
bool firstPriorityQueueTest() {
	initRandom();
	std::vector<int> priority(TEST_ELEMENTS_NUMBER);
	// приореты выберем случайными
	randFillVector(priority);

	// используем элементы, совпадающие со своими приоритетами
	std::vector<int> data(priority);
	PriorityQueue<int, int, TEST_ELEMENTS_NUMBER, std::less<int> > priorityQueue(data, priority);

	int steps = 0;
	int previousElement = priorityQueue.front();
	while ((!priorityQueue.empty()) && (previousElement <= priorityQueue.front())) {
		previousElement = priorityQueue.front();
		priorityQueue.pop();
		++steps;
	}

	if ((priorityQueue.empty()) && (priorityQueue.size() == 0) && (steps == data.size())) {
		return true;
	} else {
		return false;
	}
}

// проверяет работосособность метода PriorityQueue::push
bool secondPriorityQueueTest() {
	initRandom();
	std::vector<int> priority(TEST_ELEMENTS_NUMBER - 1);
	// приореты выберем случайными
	randFillVector(priority);

	// используем элементы, совпадающие со своими приоритетами
	std::vector<int> data(priority);
	PriorityQueue<int, int, TEST_ELEMENTS_NUMBER, std::less<int> > priorityQueue(data, priority);

	priorityQueue.push(201, 0);

	if (priorityQueue.front() == 201) {
		return true;
	} else {
		return false;
	}
}

int main() {
	if (firstPriorityQueueTest() && secondPriorityQueueTest()) {
		std::cout << "Priority queue works correctly" << std::endl;
	} else {
		std::cout << "Priority queue doesn't work correctly" << std::endl;
	}

	return 0;
}

