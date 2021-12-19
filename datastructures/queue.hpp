#pragma once

#include "singly-linked-list-node.hpp"

template<typename T>
class queue
{
private:
public:
    inline queue() noexcept : m_Head(nullptr), m_Tail(nullptr), m_Size(0) { };
    inline queue(T data) noexcept
        : m_Head(new list_node_t<T>(data)), m_Tail(m_Head), m_Size(1) { }
    
    inline ~queue() noexcept
    {
        if (m_Size == 0)
            return;
        list_node_t<T> *tmp = m_Head;
        while (tmp)
        {
            list_node_t<T> *next = tmp->next;
            delete tmp;
            tmp = next;
        }
        m_Head = nullptr;
    }

    inline const T *front() noexcept { return m_Head ? m_Head->data : nullptr; }
    inline const T *back() noexcept { return m_Tail ? m_Tail->data : nullptr; }
    inline const list_node_t<T> *begin() noexcept { return m_Head; }
    inline const list_node_t<T> *end() noexcept { return m_Tail; }
    inline const unsigned int size() noexcept { return m_Size; }

    inline void enqueue(T data) noexcept
    {
        list_node_t<T> *new_node = new list_node_t<T>(data);
        if (!m_Head && !m_Tail) m_Head = m_Tail = new_node;
        m_Tail->next = new_node;
        m_Tail = new_node;
    }
    
    // TODO: enqueue

private:
    list_node_t<T> *m_Head;
    list_node_t<T> *m_Tail;
    list_node_t<T> *m_Size;
};

