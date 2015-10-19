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
 *���������� �����: ���������� ��������� ������ ������� � ������������
 */

#pragma once
#include <vector>
#include <list>
#include <stdexcept>

// ������������ ��� �������������� ��������, 
// ��������� � ������� PriorityQueue
class PriorityQueueException: public std::runtime_error {
public:
	explicit PriorityQueueException(const std::string& errorMessage)
		: runtime_error(errorMessage) {

		//do nothing
	}
};

template<typename DataType, typename PriorityType, size_t MAX_SIZE = 1024, typename Compare = std::less<PriorityType>>
class PriorityQueue {
private:
	struct Item {
	public:
		typename std::list<DataType>::iterator elementIterator;
		typename std::list<PriorityType>::iterator priorityIterator;

		Item(typename std::list<DataType>::iterator newElementIterator,
			typename std::list<PriorityType>::iterator newPriorityIterator)

			: elementIterator(newElementIterator),
			priorityIterator(newPriorityIterator) {

			//do nothing
		}
	};

	// ������ ��������� �������
	std::list<DataType> data_;

	// ������ ����������� ��������� �������
	std::list<PriorityType> priorities_;

	// ��������� ��� �������� ��������� ������ heap
	std::vector<Item> heap_;

	// ����������, ������������ ��� ���������
	// �������� ���� PriorityType
	Compare comparator_;

	size_t firstItemPosition() const {
		return 0;
	}

	size_t lastItemPosition() const {
		return heap_.size() - 1;
	}

	// ���������, ��������� �� ��������
	// ���������� ��������� � �������
	bool checkMaxSizePositionReached() const {
		if (heap_.size() == MAX_SIZE) {
			return true;
		} else {
			return false;
		}
	}

	void swap(size_t firstItemPosition, size_t secondItemPosition) {
		std::swap(heap_[firstItemPosition].elementIterator, heap_[secondItemPosition].elementIterator);
		std::swap(heap_[firstItemPosition].priorityIterator, heap_[secondItemPosition].priorityIterator);
	}

	// ������� ������� �� itemPosition ���� �� ������,
	// ���� ���������� �������� ��������� ������ heap
	void siftDown(size_t itemPosition) {
		++itemPosition;

		while (itemPosition * 2 <= heap_.size()) {
			int newPosition = itemPosition * 2;

			if ((newPosition < heap_.size()) &&
				(!comparator_(*(heap_[newPosition - 1].priorityIterator), *(heap_[newPosition].priorityIterator)))) {

				++newPosition;
			}

			if (comparator_(*(heap_[itemPosition - 1].priorityIterator), *(heap_[newPosition - 1].priorityIterator))) {
				break;
			}

			swap(itemPosition - 1, newPosition - 1); 
			itemPosition = newPosition;
		}
	}

	// ������� ������� �� itemPosition ����� �� ������,
	// ���� ���������� �������� ��������� ������ heap
	void siftUp(size_t itemPosition) {
		++itemPosition;

		while ((itemPosition > firstItemPosition() + 1) && 
				(!comparator_(*(heap_[itemPosition / 2 - 1].priorityIterator), *(heap_[itemPosition - 1].priorityIterator)))) {

			swap(itemPosition - 1, itemPosition / 2 - 1);
			itemPosition = itemPosition / 2;
		}
	}

public:
	PriorityQueue(const Compare& comparator = Compare())
		: heap_(),
		data_(),
		priorities_(),
		comparator_(comparator) {

		//do nothing
	}

	// ������� ������� �� ����������� ��������� �����
	// DataContainer ��� ������
	// PriorityContainer ��� �����������
	template<typename DataContainer, typename PriorityContainer>
	PriorityQueue(const DataContainer& dataContainer, const PriorityContainer& priorityContainer,
					const Compare& comparator = Compare()) {

		makePriorityQueue(dataContainer, priorityContainer, comparator);
	}

	// ������� ������� �� ����������� ��������� �����
	// DataContainer ��� ������
	// PriorityContainer ��� �����������
	template<typename DataContainer, typename PriorityContainer>
	void makePriorityQueue(const DataContainer& dataContainer, const PriorityContainer& priorityContainer,
							const Compare& comparator = Compare()) {

		comparator_ = comparator;

		typename DataContainer::const_iterator dataIterator;
		typename PriorityContainer::const_iterator priorityIterator;

		for (dataIterator = dataContainer.cbegin(), priorityIterator = priorityContainer.cbegin();
			dataIterator != dataContainer.cend(), priorityIterator != priorityContainer.cend();
			++dataIterator, ++priorityIterator) {

			if (checkMaxSizePositionReached()) {
				throw PriorityQueueException("priority queue overflow");
			}

			data_.push_back(*dataIterator);
			priorities_.push_back(*priorityIterator);

			heap_.push_back(Item(--data_.end(), --priorities_.end()));
		}

		for (size_t itemPosition = lastItemPosition(); itemPosition + 1 > firstItemPosition(); --itemPosition) {
			siftDown(itemPosition);
		}
	}

	// ��������� � ������� ������� value
	// � ����������� priority
	void push(const DataType& value, const PriorityType& priority) {
		if (checkMaxSizePositionReached()) {
			throw PriorityQueueException("priority queue overflow");
		}

		data_.push_back(value);
		priorities_.push_back(priority);

		heap_.push_back(Item(--data_.end(), --priorities_.end()));

		siftUp(lastItemPosition());
	}

	// ������� �� ������� ��������� �� ���������� �������
	void pop() {
		if (empty()) {
			throw PriorityQueueException("pop from empty priority queue");
		}

		swap(firstItemPosition(), lastItemPosition());

		data_.erase(heap_[lastItemPosition()].elementIterator);
		priorities_.erase(heap_[lastItemPosition()].priorityIterator);

		heap_.pop_back();

		siftDown(firstItemPosition());
	}

	// ���������� ��������� �� ���������� �������
	const DataType& front() const {
		if (empty()) {
			throw PriorityQueueException("top of empty priority queue");
		}

		return *(heap_[firstItemPosition()].elementIterator);
	}

	size_t size() const {
		return heap_.size();
	}
	
	bool empty() const {
		return heap_.empty(); 
	}
};