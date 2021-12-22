#pragma once

#include "double-linked-list-node.hpp"

template<typename T>
class list
{
private:
public:
    inline const T *front() noexcept { return m_Head ? m_Head->data : nullptr; }
    inline const T *back() noexcept { return m_Tail ? m_Tail->data : nullptr; }
    inline const d_list_node<T> *begin() noexcept { return m_Head; }
    inline const d_list_node<T> *end() noexcept { return m_Tail; }
    inline const unsigned int size() noexcept { return m_Size; }
private:
    d_list_node<T> m_Head, m_Tail;
    unsigned int m_Size;
};
