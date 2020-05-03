//
// Created by mihawkcola on 03.05.20.
//

#include "Queue.h"
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

Queue::Queue () {
    empty = true;
    full = false;
    head = 0;
    tail = 0;
}

Queue::~Queue () { }

void Queue::addItem(string in) {
    buf[tail] = in;
    tail++;
    if (tail == QUEUESIZE)
        tail = 0;
    if (tail == head)
        full = 1;
    empty = 0;
}

void Queue::delItem (string *out) {
    *out = buf[head];
    head++;
    if (head == QUEUESIZE)
        head = 0;
    if (head == tail)
        empty = 1;
    full = 0;
}

/* Producer */
void *producer(void *q) {
    Queue * fifo = (Queue *) q;
    ifstream ein("url.txt");
    string a;
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < LOOP && ein >> a; i++) {
            std::unique_lock<std::mutex> lck(fifo->mut);
            // while -> spinlock
            if (fifo->isFull()) {
                printf("producer: queue FULL.\n");
                fifo->notFull.wait(lck);
            }
            fifo->addItem(a);
          //  lck.unlock();
            printf("produced: produced %s i: %d j: %d.\n", a.c_str(), i,j);

            fifo->notEmpty.notify_one();
            usleep(100000 + j * 100000);
        }
    }
    return (NULL);
}

/* Consumer */
void *consumer(void *q, int number) {
    Queue *fifo;
    int i;
    string d;
    int j;
    fifo = (Queue *) q;
    for (j = 0; j < 2; j++) {
        for (i = 0; i < LOOP; i++) {
            std::unique_lock<std::mutex> lck(fifo->mut);
            // while -> spinlock
            if (fifo->isEmpty()) {
                printf("consumer: queue EMPTY.\n");
                fifo->notEmpty.wait(lck);
            }
            fifo->delItem (&d);
            lck.unlock();

            printf("%d: consumer: consumed %s.\n",number, d.c_str());
            fifo->notFull.notify_one();
            usleep(200000 + j * 300000);
        }
    }
    return (NULL);
}