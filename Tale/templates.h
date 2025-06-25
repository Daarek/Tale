#pragma once
#include<array>

template<typename T, int W, int H>
using arr2d = std::array<std::array<T, H>, W>; //2d массив

template<typename T, int W, int H, int D>
using arr3d = std::array<std::array<std::array<T, H>, W>, D>; //3d массив