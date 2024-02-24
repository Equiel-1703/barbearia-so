#pragma once

#include <iostream>
#include <pthread.h>

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
    };

    Node* head;
    int size = 0;
    // linked list mutex
    pthread_mutex_t mutex;

public:
    LinkedList() : head(nullptr) {
        pthread_mutex_init(&mutex, NULL);
    }

    ~LinkedList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        pthread_mutex_destroy(&mutex);
    }

    void push_back(const T& value) {
        pthread_mutex_lock(&mutex);
        size++;
        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        pthread_mutex_unlock(&mutex);
    }

    // Pop
    T pop() {
        pthread_mutex_lock(&mutex);
        size--;
        if (head == nullptr) {
            std::cerr << "List is empty" << std::endl;
            return T();
        }

        Node* temp = head;
        head = head->next;
        T value = temp->data;
        delete temp;
        pthread_mutex_unlock(&mutex);
        return value;
    }

    int getSize() {
        pthread_mutex_lock(&mutex);
        int tmp_size = this->size;
        pthread_mutex_unlock(&mutex);
        return tmp_size;
    }

    bool isEmpty() {
        pthread_mutex_lock(&mutex);
        bool empty = this->size == 0;
        pthread_mutex_unlock(&mutex);
        return empty;
    }

    T remove(const T& value) {
        pthread_mutex_lock(&mutex);
        Node* current = head;
        Node* previous = nullptr;
        while (current != nullptr) {
            if (current->data == value) {
                if (previous == nullptr) {
                    head = current->next;
                } else {
                    previous->next = current->next;
                }
                T data = current->data;
                delete current;
                size--;
                pthread_mutex_unlock(&mutex);
                return data;
            }
            previous = current;
            current = current->next;
        }
        
        std::cout << "Value not found" << std::endl;
        pthread_mutex_unlock(&mutex);
        return T();
    }

    const T& front() {
        pthread_mutex_lock(&mutex);
        if (head == nullptr) {
            std::cerr << "List is empty" << std::endl;
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        pthread_mutex_unlock(&mutex);
        return head->data;
    }
};
