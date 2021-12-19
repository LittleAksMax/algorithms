#pragma once

template<typename T>
struct list_node_t
{
    T data;
    list_node_t *next;

    list_node_t(T data) : data(data), next(nullptr) { }
};
