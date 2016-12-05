#pragma once

#include <vector>

#include <cstdlib>

template <typename T>
std::vector<T> sample(const std::vector<T> &input, size_t size)
{
    if (input.size() < size)
    {
        return input;
    }
    else
    {
        std::vector<T> output;

        for (unsigned int n = 0; n < input.size(); n += (input.size() / size))
        {
            output.push_back(input[n]);
        }

        return output;
    }
}
