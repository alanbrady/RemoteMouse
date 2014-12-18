#include "socketdataqueue.h"
#include <QDebug>

SocketDataQueue::SocketDataQueue()
    : m_head(NULL), m_tail(NULL),
      m_leftover(NULL), m_leftoverSize(0), m_leftoverRemaining(0) {
}

SocketDataQueue::~SocketDataQueue()
{
    while (m_head != NULL) {
        Node* temp = m_head;
        m_head = m_head->getNext();
        delete temp;
    }
    if (m_leftover != NULL) {
        delete[] m_leftover;
    }
}

void SocketDataQueue::addData(const char *data, const unsigned int len)
{
    // if we're not expecting more data after this
    if (m_leftoverRemaining <= len) {
        unsigned int bytesLeft = len;
        // if we have data left over from last add
        if (m_leftover != NULL) {
            memcpy(m_leftover+(m_leftoverSize-m_leftoverRemaining), data,
                   m_leftoverRemaining);

            enqueueData(m_leftover, m_leftoverSize);
            delete[] m_leftover;
            m_leftover = NULL;
            m_leftoverSize = 0;
            m_leftoverRemaining = 0;

            data += m_leftoverRemaining;
            bytesLeft -= m_leftoverRemaining;
        }


//        const char* dataEnd = data+bytesLeft;
        // while there's still bytes to parse
        while (bytesLeft > 0) {
            unsigned int msgSize = *(data); // cast first byte into an int
            data++;
            bytesLeft--;
            // make sure we rcvd the whole message, increment pointers
            if (msgSize <= bytesLeft) {
                enqueueData(data, msgSize);
                data += msgSize;
                bytesLeft -= msgSize;
            } else {
                // save data for next add, remaining = total size - bytes left
                saveLeftover(data, msgSize, msgSize-bytesLeft);
            }
        }
    } else {
        // expecting more data, copy what we have and update remaining bytes
        memcpy(m_leftover+(m_leftoverSize-m_leftoverRemaining), data, len);
        m_leftoverRemaining -= len;
    }
//    while (data != dataEnd) {
//        unsigned int msgSize = *(data);
//        data++;
//        enqueueData(data, msgSize);
//        data += msgSize;
//    }
//    unsigned int msgSize = *(data);
//    data++;
//    while (data+msgSize < dataEnd) {

//    }


//    unsigned int i = 0;
//    while (i < len) {

//    }
//    char* newData = new char[len+m_leftoverSize];
//    memcpy(newData, m_leftover, m_leftoverSize);
//    memcpy(newData+m_leftoverSize, data, len);

//    delete[] m_leftover;
//    m_leftover = NULL;
//    m_leftoverSize = 0;


//    unsigned int i = 0;
//    unsigned int k = 0;
//    while (i < len) {
//        while (k < len && data[k] != '\n') {
//            k++;
//        }
//        if (k < len) {
//            enqueueData(data+i, k-i);
//            k++;
//            i = k;
//        }
//    }

//    if (data[len-1] != '\n') {
//        saveLeftover(data+i, len-i);
//    }
}

SocketDataQueue::SocketData SocketDataQueue::dequeueData()
{
    Node* temp = m_head;
    m_head = m_head->getNext();
    SocketData data = temp->getData();
    delete temp;
    return data;
}

void SocketDataQueue::enqueueData(const char *data, const unsigned int len)
{
//    qDebug() << "Enqueue data: " << data << " len: " << len;
    Node* newNode = new Node(data, len);

    if (m_head != NULL) {
        m_tail->setNext(newNode);
        m_tail = newNode;
    } else {
        m_head = newNode;
        m_tail = newNode;
    }
}

void SocketDataQueue::saveLeftover(const char *data, const unsigned int size,
                                   const unsigned int remain)
{
    if (m_leftover != NULL) {
        delete[] m_leftover;
    }
    m_leftover = new char[size];
    memcpy(m_leftover, data, size-remain);
    m_leftoverSize = size;
    m_leftoverRemaining = remain;
}


SocketDataQueue::SocketData::SocketData(const char *data, const unsigned int len)
    : m_len(len) {
    m_data = new char[len];
    memcpy(m_data, data, len);
}

SocketDataQueue::SocketData::SocketData(const SocketDataQueue::SocketData &other)
    : m_len(other.m_len){
    m_data = new char[other.m_len];
    memcpy(m_data, other.m_data, other.m_len);
}

SocketDataQueue::SocketData::~SocketData()
{
    delete[] m_data;
}


SocketDataQueue::Node::Node(const char *data, const unsigned int len)
    : m_data(data, len), m_next(NULL) {

}
