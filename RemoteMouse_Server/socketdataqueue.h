#ifndef SOCKETDATAQUEUE_H
#define SOCKETDATAQUEUE_H

/* FIFO queue for organizing socket read data */

#include <cstring>

class SocketDataQueue
{
public:
    SocketDataQueue();
    ~SocketDataQueue();

    class SocketData {
    public:
        SocketData(const char* data, const unsigned int len);
        SocketData(const SocketData& other);
        ~SocketData();

        const char* getData() { return m_data; }
        unsigned int getDataLen() { return m_len; }

    private:
        const unsigned int m_len;
        char* m_data;
    };

    void addData(const char* data, const unsigned int len);
    bool isEmpty() { return m_head == NULL; }
    SocketData dequeueData();

private:
    class Node {
    public:
        Node(const char* data, const unsigned int len);

        SocketData getData() { return m_data; }
        Node* getNext() { return m_next; }
        void setNext(Node* next) { m_next = next; }
    private:
        SocketData m_data;
        Node* m_next;
    };

    Node* m_head;
    Node* m_tail;
    char* m_leftover;
    unsigned int m_leftoverSize;
    unsigned int m_leftoverRemaining;

    void enqueueData(const char *data, const unsigned int len);
    void saveLeftover(const char* data, const unsigned int size,
                      const unsigned int remain);
};

#endif // SOCKETDATAQUEUE_H
