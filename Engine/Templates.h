
#pragma once

template <std::size_t N>
struct type_of_size
{
    typedef char type[N];
};

template <typename T, std::size_t Size>
typename type_of_size<Size>::type& sizeof_array_helper(T(&)[Size]);

#define sizeof_array(pArray) (sizeof(sizeof_array_helper(pArray)))

