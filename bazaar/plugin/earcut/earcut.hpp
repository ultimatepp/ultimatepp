#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>
#include <vector>

namespace mapbox {

namespace util {

template <std::size_t I, typename T> struct nth {
    inline static typename std::tuple_element<I, T>::type
    get(const T& t) { return std::get<I>(t); };
};

}

namespace detail {

