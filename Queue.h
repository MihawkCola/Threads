//
// Created by mihawkcola on 03.05.20.
//

#ifndef UNTITLED1_QUEUE_H
#define UNTITLED1_QUEUE_H

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

#define QUEUESIZE 10
#define LOOP 20



/* Klasse Queue (Warteschlange) */
class Queue {
private:
    string buf[QUEUESIZE];
    long head, tail;
    bool full, empty;
public:
    std::mutex mut;
    std::condition_variable notFull, notEmpty;
public:
    Queue();
    ~Queue();
    void addItem(string in);
    void delItem(string *out);
    bool isFull() { return full; }
    bool isEmpty() { return empty; }
};
void *producer(void *args);
void *consumer(void *args, int number);
#endif //UNTITLED1_QUEUE_H
