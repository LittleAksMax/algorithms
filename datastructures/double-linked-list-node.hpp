#pragma once

template<typename T>
struct d_list_node
{
    T data;
    d_list_node<T> *prev;
    d_list_node<T> *next;

    d_list_node(T data) 
        : data(data), next(nullptr), prev(nullptr) { }

};
