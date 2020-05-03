#include <iostream>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <fstream>
#include "Queue.h"
using namespace std;
void dateiLesen(Queue &fifo){
    ifstream ein("url.txt");
    cout << "Wie viele Web Threads möchten Sie";
    if(!ein){
        cout << "Fehler datei konnte nicht geöfnet werden" << endl;
    } else {
        string a;
        while (!(ein >> a).eof()) {
            cout << "URL: " << a << endl;
            fifo.addItem(a);
        }
    }
    ein.close();
}

int main() {
    Queue fifo;
    ifstream ein("url.txt");
    if(!ein){
        cout << "Fehler datei konnte nicht geöfnet werden" << endl;
    } else {
        thread pro(producer, &fifo);
        thread con[10];
        for (int i = 0; i <10 ; ++i) {
            con[i] = thread(consumer, &fifo,i);
            cout << "start Thread("<< i <<") "<< con[i].get_id() << endl;
        }
        pro.join();
        for (int i = 0; i <10 ; ++i) {
           con[i].join();
        }
    }
    return 0;
}

