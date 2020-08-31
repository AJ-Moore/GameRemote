// Copyright © Allan Moore April 2020

#pragma once

#include <memory>

typedef unsigned char UNSIGNED_BYTE;
typedef unsigned char uint8; 
typedef char int8; 
typedef unsigned short uint16; 
typedef short int16; 
typedef unsigned int uint32; 
typedef int int32; 
typedef uint32 uint;

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
using SharedPtr = std::shared_ptr<T>;
