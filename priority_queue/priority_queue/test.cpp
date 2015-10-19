/**
 *�����: ������� ���������
 *������� ���������: ����(��) ���� 296
 *�����������: ���������� ���������� � �����������
 *
 *���������� ������� �� ����������
 *� ��������� ����������� 
 *� ������ ����������� ������������
 *������, ������
 *
 *������������ ����� ����������: Microsoft VS C++ 2010
 *������������ ��������: stl
 *
 *���������� �����: �������� ��������� ��� ������� � ������������,
 *                  ������������� � priority_queue.h
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

// ��������� ������ ���������� �������
void randFillVector(std::vector<int>& vector) {
	for (size_t position = 0; position < vector.size(); ++position) {
		vector[position] = rand() % VALUES_NUMBER + 1;
	}
} 

// ��������� ������������ ������ PriorityQueue
bool firstPriorityQueueTest() {
	initRandom();
	std::vector<int> priority(TEST_ELEMENTS_NUMBER);
	// �������� ������� ����������
	randFillVector(priority);

	// ���������� ��������, ����������� �� ������ ������������
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

// ��������� ���������������� ������ PriorityQueue::push
bool secondPriorityQueueTest() {
	initRandom();
	std::vector<int> priority(TEST_ELEMENTS_NUMBER - 1);
	// �������� ������� ����������
	randFillVector(priority);

	// ���������� ��������, ����������� �� ������ ������������
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

