#pragma once

#include "singly-linked-list-node.hpp"

template<typename T>
class stack
{
private:
public:
    inline stack() noexcept : m_Head(nullptr), m_Size(0) { };
    inline stack(T data) noexcept
        : m_Head(new list_node_t<T>(data)), m_Size(1) { }
    
    inline ~stack() noexcept
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
    
    inline const T *top() noexcept { return m_Head ? m_Head->data : nullptr; }
    inline const list_node_t<T> *begin() noexcept { return m_Head; }
    inline const unsigned int size() noexcept { return m_Size; }

    inline void push(T data) noexcept
    {
        list_node_t<T> *new_node = new list_node_t<T>(data);
        new_node->next = m_Head;
        m_Head = new_node;
    }

    inline bool pop() noexcept
    {
        if (!m_Head) return false;
        list_node_t<T> *tmp = m_Head;
        m_Head = m_Head->next;
        delete tmp;
        return true;
    }
private:
    list_node_t<T> *m_Head;
    unsigned int m_Size;
};
