/***************************************************************************
 *            tests/test_dlmath.cpp
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

#include "test.hpp"
#include "dnn/type.hpp"
#include "dnn/dlmath.hpp"
#include "dnn/layer.hpp"
#include "dnn/model.hpp"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>

using namespace std;
using namespace Ariadne;

class TestDLMath {
public:
    void test() {
        ARIADNE_TEST_CALL(test_normal_pdf());
        ARIADNE_TEST_CALL(test_arr_sum());
        ARIADNE_TEST_CALL(test_arr_mul());
        ARIADNE_TEST_CALL(test_matarr_mul());
        ARIADNE_TEST_CALL(test_relu());
        ARIADNE_TEST_CALL(test_softmax());
        ARIADNE_TEST_CALL(test_relu_1());
        ARIADNE_TEST_CALL(test_softmax_1());
        ARIADNE_TEST_CALL(test_cross_entropy());
        ARIADNE_TEST_CALL(test_cross_entropy_1());
        ARIADNE_TEST_CALL(test_mean_squared_error());
        ARIADNE_TEST_CALL(test_mean_squared_error_1());
        ARIADNE_TEST_CALL(test_max_argmax());
    }

private:
    const RneType::result_type SEED = 1;
    const size_t PRINT_TIMES = 4;

    void test_normal_pdf() {
        RneType generator{SEED};
        auto dist = DLMath::normal_pdf<NumType>(0.0, 0.1);
        for (size_t i = 0; i < PRINT_TIMES; ++i)
        {
            ARIADNE_TEST_PRINT(std::to_string(i) + ": " 
                + std::to_string(dist(generator)));
        }

        std::random_device rd;
        generator = RneType{rd()};
        size_t gt1_count = 0, lt1_count = 0;
        for (size_t i = 0; i < 10000; ++i)
        {
            if (dist(generator) > 0.0)
            {
                ++gt1_count;
            }
            else 
            {
                ++lt1_count;
            }
        }
        ARIADNE_TEST_PRINT("Normal distribution >0 count similar to <=0 count:"
            + std::to_string(gt1_count) + ", " + std::to_string(lt1_count));
    }

    void test_arr_sum() {
        std::vector<int> test_vec1{5,4,3,2,1};
        std::vector<int> test_vec2{1,2,3,4,5};
        std::vector<int> truth_vec{6,6,6,6,6};
        DLMath::arr_sum<int>(test_vec1.data(), test_vec1.data(), 
            test_vec2.data(), test_vec1.size());
        for (size_t i = 0; i < truth_vec.size(); ++i)
        {
            ARIADNE_TEST_EQUAL(test_vec1[i], truth_vec[i]);
        }
    }

    void test_arr_mul() {
        std::vector<int> test_vec1{5,4,3,2,1};
        std::vector<int> test_vec2{1,2,3,4,5};
        std::vector<int> truth_vec{5,8,9,8,5};
        DLMath::arr_mul<int>(test_vec1.data(), test_vec1.data(), 
            test_vec2.data(), test_vec1.size());
        for (size_t i = 0; i < truth_vec.size(); ++i)
        {
            ARIADNE_TEST_EQUAL(test_vec1[i], truth_vec[i]);
        }
    }

    void test_matarr_mul() {
        std::vector<int> test_mat{1,2,3,4};
        std::vector<int> test_vec{1,2};
        std::vector<int> truth_vec{5,11};
        ARIADNE_TEST_FAIL(
            DLMath::matarr_mul<int>(test_vec.data(), test_mat.data(), 
                                    test_vec.data(), 2, 2)
        );
        std::vector<int> res_vec; res_vec.resize(test_vec.size());
        DLMath::matarr_mul<int>(res_vec.data(), test_mat.data(), 
                                test_vec.data(), 2, 2);
        for (size_t i = 0; i < truth_vec.size(); ++i)
        {
            ARIADNE_TEST_EQUAL(res_vec[i], truth_vec[i]);
        }
    }

    void test_relu() {
        std::vector<NumType> test_vec{-2,-1,0,1,2};
        std::vector<NumType> truth_vec{0,0,0,1,2};
        DLMath::relu<NumType>(test_vec.data(), test_vec.data(), 
            test_vec.size());
        for (size_t i = 0; i < truth_vec.size(); ++i)
        {
            ARIADNE_TEST_PRINT(std::to_string(i) + ": " 
                + std::to_string(test_vec[i]));
            ARIADNE_TEST_WITHIN(test_vec[i], truth_vec[i], 0.00000000001);
        }
    }

    void test_softmax() 
    {
        std::vector<NumType> test_vec{-2,-1,0,1,2};
        std::vector<NumType> truth_vec{
            0.01165623095604, 0.031684920796124, 0.086128544436269,
            0.23412165725274, 0.63640864655883};
        DLMath::softmax<NumType>(test_vec.data(), test_vec.data(), 
            test_vec.size());
        for (size_t i = 0; i < truth_vec.size(); ++i)
        {
            ARIADNE_TEST_PRINT(std::to_string(i) + ": " 
                + std::to_string(test_vec[i]));
            ARIADNE_TEST_WITHIN(test_vec[i], truth_vec[i], 0.00000000001);
        }
    }

    void test_relu_1() {
        std::vector<NumType> test_vec{-2,-1,0,1,2};
        std::vector<NumType> truth_vec{0,0,0,1,1};
        DLMath::relu_1<NumType>(test_vec.data(), test_vec.data(), 
            test_vec.size());
        for (size_t i = 0; i < truth_vec.size(); ++i)
        {
            ARIADNE_TEST_PRINT(std::to_string(i) + ": " 
                + std::to_string(test_vec[i]));
            ARIADNE_TEST_WITHIN(test_vec[i], truth_vec[i], 0.00000000001);
        }
    }

    void test_softmax_1() {
        std::vector<NumType> test_vec{-2.0,-1.0,0.0,1.0,2.0};
        ARIADNE_TEST_FAIL(DLMath::softmax_1_opt<NumType>(test_vec.data(), 
            test_vec.data(), test_vec.size()));
        ARIADNE_TEST_EXECUTE(DLMath::softmax_1<NumType>(test_vec.data(), 
            test_vec.data(), test_vec.size()));
        for (size_t i = 0; i < test_vec.size(); ++i)
        {
            std::cout << std::fixed << std::setprecision(40) 
                << "test_vec[i]: " << test_vec[i] << std::endl << std::endl;
        }
    }

    void test_cross_entropy() {
        std::vector<NumType> test_y    {0.0, 0.0, 0.00, 0.00, 1.0};
        std::vector<NumType> test_y_hat{0.1, 0.1, 0.25, 0.05, 0.5};
        NumType truth_ce = 0.6931471805599453;
        auto ret = DLMath::cross_entropy(test_y.data(), test_y_hat.data(), 
            test_y_hat.size());
        ARIADNE_TEST_WITHIN(ret, truth_ce, 0.00000000001);

        NumType test_val = 0.5;
        NumType truth_val = 0.34657359027997264;
        ret = DLMath::cross_entropy(test_val, test_val);
        ARIADNE_TEST_WITHIN(ret, truth_val, 0.00000000001);
    }

    void test_cross_entropy_1() {
        std::vector<NumType> test_y    {0.0, 0.0, 0.00, 0.00, 1.0};
        std::vector<NumType> test_y_hat{0.1, 0.1, 0.25, 0.05, 0.5};
        std::vector<NumType> truth_ce1 {0.0, 0.0, 0.00, 0.00, -2.0};
        std::vector<NumType> ret_vec; ret_vec.resize(truth_ce1.size());
        DLMath::cross_entropy_1(ret_vec.data(), test_y.data(), 
            test_y_hat.data(), 1.0, test_y_hat.size());
        for (size_t i = 0; i < truth_ce1.size(); ++i)
        {
            ARIADNE_TEST_WITHIN(ret_vec[i], truth_ce1[i], 0.00000000001);
        }
        

        NumType test_val = 0.5;
        NumType truth_val = -1.0;
        auto ret_val = DLMath::cross_entropy_1(test_val, test_val, 1.0);
        ARIADNE_TEST_WITHIN(ret_val, truth_val, 0.00000000001);
    }

    void test_mean_squared_error() {
        NumType test_val = 1.0;
        NumType truth_val = 0.0;
        auto ret = DLMath::squared_error(test_val, test_val);
        ARIADNE_TEST_WITHIN(ret, truth_val, 0.00000000001);

        std::vector<NumType> test_y    {1.0, 1.0, 1.0, 1.0, 1.0};
        std::vector<NumType> test_y_hat{1.1, 0.1, 1.2, 1.5, 0.5};
        NumType truth_mse = 0.272;
        ret = DLMath::mean_squared_error(test_y.data(), test_y_hat.data(), 
            test_y_hat.size());
        ARIADNE_TEST_WITHIN(ret, truth_mse, 0.00000000001);
    }

    void test_mean_squared_error_1() {
        NumType test_val1 = 1.0;
        NumType test_val2 = 1.5;
        NumType truth_val = 0.5;
        auto ret = DLMath::squared_error_1(test_val1, test_val2, 0.5);
        ARIADNE_TEST_WITHIN(ret, truth_val, 0.00000000001);

        std::vector<NumType> test_y    {1.0, 1.0, 1.0, 1.0, 1.0};
        std::vector<NumType> test_y_hat{1.1, 0.1, 1.2, 1.5, 0.5};
        std::vector<NumType> truth_mse1 {0.2, -1.8, 0.4, 1.0, -1.0};
        std::vector<NumType> ret_vec; ret_vec.resize(truth_mse1.size());
        DLMath::mean_squared_error_1(ret_vec.data(), test_y.data(), 
            test_y_hat.data(), 1.0, test_y_hat.size());
        for (size_t i = 0; i < truth_mse1.size(); ++i)
        {
            ARIADNE_TEST_WITHIN(ret_vec[i], truth_mse1[i], 0.00000000001);
        }
    }

    void test_max_argmax() {
        std::vector<NumType> test_vec{0,1,5,4,3};
        NumType truth_max = 5;
        NumType ret_max = DLMath::max<NumType>(test_vec.data(), test_vec.size());
        ARIADNE_TEST_EQUAL(ret_max, truth_max);

        NumType truth_argmax = 2;
        NumType ret_argmax = DLMath::argmax<NumType>(test_vec.data(), 
            test_vec.size());
        ARIADNE_TEST_EQUAL(ret_argmax, truth_argmax);

        auto ret_tuple = DLMath::max_and_argmax<NumType>(test_vec.data(), 
            test_vec.size());
        ARIADNE_TEST_EQUAL(std::get<0>(ret_tuple), truth_max);
        ARIADNE_TEST_EQUAL(std::get<1>(ret_tuple), truth_argmax);
    }
};

int main() {
    TestDLMath().test();
    return ARIADNE_TEST_FAILURES;
}
