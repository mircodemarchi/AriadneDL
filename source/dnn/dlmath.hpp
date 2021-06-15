/***************************************************************************
 *            dlmath.hpp
 *
 *  Copyright  2021  Mirco De Marchi
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

/*! \file dlmath.hpp
 *  \brief Simply replace me.
 */

#include <cmath>
#include <functional>
#include <cassert>
#include <stdexcept>
#include <tuple>
#include <algorithm>
#include <iterator>


#ifndef ARIADNE_DL_DLMATH_HPP
#define ARIADNE_DL_DLMATH_HPP

namespace Ariadne {

namespace dlmath {

/**
 * \brief Gaussian Probability Density Function.
 * \tparam T      Input and output type.
 * \param x       Input value to compute.
 * \param mean    Mean of the probability distribution required.
 * \param std_dev Standard Deviation of the probability distribution required.
 * \return T
 */
template <typename T>
std::function<T(rne_t)> normal_pdf(float mean, float std_dev)
{
    static const double inv_sqrt_2pi = 0.3989422804014327;
    double inv_sqrt_2pi_std_dev = (inv_sqrt_2pi / std_dev);
    
    std::function<T(rne_t)> ret = [inv_sqrt_2pi_std_dev, mean, std_dev](rne_t x) 
    {
        double a = (double(x()) - mean) / std_dev;
        return T(inv_sqrt_2pi_std_dev * std::exp(-0.5f * a * a));
    };
    return ret;
}

/**
 * \brief Element wise multiplication between two arrays.
 * \tparam T     Type of each source and destination elements.
 * \param dst    Array to write the result.
 * \param src1   First operand array.
 * \param src2   Second operand array.
 * \param length Length of the arrays.
 */
template <typename T>
void arr_mul(T* dst, const T* src1, const T* src2, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        dst[i] = src1[i] * src2[i];
    }
}

/**
 * \brief Element wise summation between two arrays.
 * \tparam T     Type of each source and destination elements.
 * \param dst    Array to write the result.
 * \param src1   First operand array.
 * \param src2   Second operand array.
 * \param length Length of the arrays.
 */
template <typename T>
void arr_sum(T* dst, const T* src1, const T* src2, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        dst[i] = src1[i] + src2[i];
    }
}

/**
 * \brief Multiplication between a matrix and an array.
 * Used for y = Wx
 * @tparam T      Type of each source and destination elements.
 * \param arr_dst Array destination to write the result.
 * \param mat_src Matrix source, left operand.
 * \param arr_src Array source, right operand.
 * \param rows    Amount of rows.
 * \param cols    Amount of columns.
 */
template <typename T>
void matarr_mul(T* arr_dst, const T* mat_src, const T* arr_src, 
    size_t rows, size_t cols)
{
    if (arr_src == arr_dst) 
    {
        throw std::runtime_error("arr_src, arr_dst have to be different "
                                 "in order to perform matarr_mul");
    }

    for (size_t i = 0; i < rows; ++i)
    {
        arr_dst[i] = T{0};
        for (size_t j = 0; j < cols; ++j)
        {
            arr_dst[i] += mat_src[(i * cols) + j] * arr_src[j];
        }
    }
}

/**
 * \brief ReLU Function.
 * relu(x) = max(0, x)
 * \tparam T Type of the input and return type.
 * \param x  Input value.
 * \return T
 */
template <typename T>
T relu(T x)
{
    return std::max(x, T{0});
}

/**
 * \brief ReLU Function applied to a vector.
 * relu(z)_i = max(0, z_i)
 * \tparam T     Type of each source and destination elements.
 * \param dst    Array to write the result.
 * \param src    Array of read elements.
 * \param length Length of the arrays.
 */
template <typename T>
void relu(T* dst, const T* src, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        dst[i] = relu(src[i]);
    }
}

/**
 * \brief Softmax Function.
 * softmax(z)_i = exp(z_i) / \sum_j(exp(z_j))
 * \tparam T     Type of each source and destination elements.
 * \param dst    Array to write the result.
 * \param src    Array of read elements.
 * \param length Length of the arrays.
 */
template <typename T>
void softmax(T* dst, const T *src, size_t length)
{
    // Compute the exponential of each value and compute the sum. 
    T sum_exp_z{0};
    for (size_t i = 0; i < length; ++i)
    {
        dst[i] = std::exp(src[i]);
        sum_exp_z += dst[i];
    }

    // Compute the inverse of the sum.
    T inv_sum_exp_z = T{1} / sum_exp_z;

    // Multiply the inverse of the sum for each value.
    for (size_t i = 0; i < length; ++i)
    {
        dst[i] *= inv_sum_exp_z;
    }
}

/**
 * \brief Derivative of ReLU Function.
 * relu'[z]_i = 1 if z_i > 0 else 0
 * \tparam T     Type of each source and destination elements.
 * \param dst    Array to write the result.
 * \param src    Array of read elements.
 * \param length Length of the arrays.
 */
template <typename T>
void relu_1(T* dst, const T *src, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        dst[i] = (src[i] > T{0}) ? T{1} : T{0};
    }
}

/**
 * \brief Derivative Optimized of Softmax Function with the value of the 
 * argmax already saved in the src array. Source and Destination has to be 
 * differents.
 * softmax'(z)_i = \sum_j(
 *      softmax(z_i)(1 - softmax(z_i)) if i == j else -softmax(z_i)softmax(z_j))
 * \tparam T     Type of each source and destination elements.
 * \param dst    Array to write the result. It has to be different by src.
 * \param src    Array of read elements. It has to be different by dst.
 * \param length Length of the arrays.
 */
template <typename T>
void softmax_1_opt(T* dst, const T *src, size_t length)
{
    if (src == dst) 
    {
        throw std::runtime_error("src, dst have to be different "
                                 "in order to perform softmax_1_opt");
    }

    for (size_t i = 0; i < length; ++i)
    {
        dst[i] = T{0};
        for(size_t j = 0; j < length; ++j)
        {
            dst[i] += (i == j) ? src[i] * (T{1} - src[i]) : -src[i] * src[j];
        }
    }
}

/**
 * \brief Derivative of Softmax Function.
 * softmax'(z)_i = \sum_j(
 *  softmax(z_i)(1 - softmax(z_i)) if i == j else -softmax(z_i)softmax(z_j))
 * \tparam T     Type of each source and destination elements.
 * \param dst    Array to write the result.
 * \param src    Array of read elements.
 * \param length Length of the arrays.
 */
template <typename T>
void softmax_1(T* dst, const T *src, size_t length)
{
    T *tmp = new T[length];
    assert(tmp);
    softmax(tmp, src, length);
    softmax_1_opt(dst, tmp, length);
    delete[] tmp;
}

/**
 * \brief Cross-Entropy Function.
 * relu(x) = - y * log(max(y_hat, epsilon))
 * \tparam T Type of the input and return type.
 * \param y     Target value.
 * \param y_hat Estimated value.
 * \return T The resulting Cross-Entropy.
 */
template <typename T>
T cross_entropy(T y, T y_hat)
{
    return - y * std::log(std::max(y_hat, 
        std::numeric_limits<num_t>::epsilon()));
}

/**
 * \brief Cross-Entropy Function.
 * relu(x) = - \sum_j(y_j * log( max(y_hat_j, epsilon) ))
 * \tparam T Type of the input and return type.
 * \param y     Target array values.
 * \param y_hat Estimated array values.
 * \return T The resulting Cross-Entropy.
 */
template <typename T>
T cross_entropy(T *y, T *y_hat, size_t length)
{
    T ret{0};
    for (size_t i = 0; i < length; ++i)
    {
        ret += relu(y[i], y_hat[i]);
    }
    return ret;
}

template <typename T>
T max(T *src, size_t length) 
{
    return *std::max_element(src, src + length);
}

template <typename T>
T argmax(T *src, size_t length) 
{
    return std::distance(src, std::max_element(src, src + length));
}

template <typename T>
std::tuple<T, size_t> max(T *src, size_t length) 
{
    auto max_iter = std::max_element(src, src + length);
    auto dist = std::distance(src, max_iter);
    return {*max_iter, dist};
}


} // namespace dlmath

} // namespace Ariadne

#endif // ARIADNE_DL_DLMATH_HPP
