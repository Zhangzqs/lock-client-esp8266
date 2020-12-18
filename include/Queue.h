//
// Created by zhiqiang on 2020/12/12.
//

#ifndef ROBOT_LOCK_QUEUE_H
#define ROBOT_LOCK_QUEUE_H
template <class T>
class Queue{
private:
    T *data; //队列使用数组存储
    int front;
    int rear;
    int MaxSize;
public:
    explicit Queue(int max_size){
        max_size++;
        MaxSize = max_size;
        data = new T[max_size];
        //初始化为空队列
        front = 0;
        rear = 0;
    }

    ~Queue(){
        delete[] data;
    }

    bool isEmpty() {
        //当头尾指针相等时，即为空
        return front == rear;
    }

    bool isFull(){
        //当队尾+1等于队头时，即为满
        return (rear+1)%MaxSize == front;
    }

    void clear(){
        front = 0;
        rear = 0;
    }

    bool enQueue(T e) {
        if (isFull()) {
            return false;
        }
        rear = (rear + 1) % MaxSize;
        data[rear] = e;
        return true;
    }

    bool deQueue(T &e) {
        if (isEmpty()) {
            return false;
        }
        front = (front + 1) % MaxSize;
        e = data[front];
        return true;
    }
};

#endif //ROBOT_LOCK_QUEUE_H
