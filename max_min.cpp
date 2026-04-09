#include <iostream>
using namespace std;

#define MAX 100

class MaxHeap {
    int heap[MAX];
    int size;

public:
    MaxHeap() {
        size = 0;
    }

    void insert(int value) {
        int i = size;
        heap[i] = value;
        size++;

        while (i > 0 && heap[i] > heap[(i - 1) / 2]) {
            swap(heap[i], heap[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    int getMax() {
        return heap[0];
    }

    int deleteRoot() {
        int val = heap[0];
        heap[0] = heap[size - 1];
        size--;
        heapify(0);
        return val;
    }

    void heapify(int i) {
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if (l < size && heap[l] > heap[largest])
            largest = l;
        if (r < size && heap[r] > heap[largest])
            largest = r;

        if (largest != i) {
            swap(heap[i], heap[largest]);
            heapify(largest);
        }
    }

    void display() {
        cout << "Max Heap: ";
        for (int i = 0; i < size; i++)
            cout << heap[i] << " ";
        cout << endl;
    }

    int getSize() {
        return size;
    }
};

class MinHeap {
    int heap[MAX];
    int size;

public:
    MinHeap() {
        size = 0;
    }

    void insert(int value) {
        int i = size;
        heap[i] = value;
        size++;

        while (i > 0 && heap[(i - 1) / 2] > heap[i]) {
            swap(heap[i], heap[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    int getMin() {
        return heap[0];
    }

    int deleteRoot() {
        int val = heap[0];
        heap[0] = heap[size - 1];
        size--;
        heapify(0);
        return val;
    }

    void heapify(int i) {
        int smallest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if (l < size && heap[l] < heap[smallest])
            smallest = l;
        if (r < size && heap[r] < heap[smallest])
            smallest = r;

        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapify(smallest);
        }
    }

    void display() {
        cout << "Min Heap: ";
        for (int i = 0; i < size; i++)
            cout << heap[i] << " ";
        cout << endl;
    }

    int getSize() {
        return size;
    }
};

class MedianFinder {
    MaxHeap maxH;
    MinHeap minH;

public:
    void addNum(int num) {
        if (maxH.getSize() == 0 || num <= maxH.getMax())
            maxH.insert(num);
        else
            minH.insert(num);

        if (maxH.getSize() > minH.getSize() + 1)
            minH.insert(maxH.deleteRoot());
        else if (minH.getSize() > maxH.getSize())
            maxH.insert(minH.deleteRoot());
    }

    double findMedian() {
        if (maxH.getSize() == minH.getSize())
            return (maxH.getMax() + minH.getMin()) / 2.0;
        else
            return maxH.getMax();
    }

    void display() {
        maxH.display();
        minH.display();
    }
};

int main() {
    MedianFinder m;
    int ch, x;

    while (1) {
        cout << "\n1.Insert\n2.Median\n3.Display\n4.Exit\nEnter: ";
        cin >> ch;

        if (ch == 1) {
            cout << "Enter number: ";
            cin >> x;
            m.addNum(x);
        }
        else if (ch == 2) {
            cout << "Median: " << m.findMedian() << endl;
        }
        else if (ch == 3) {
            m.display();
        }
        else if (ch == 4) {
            break;
        }
        else {
            cout << "Invalid\n";
        }
    }
}