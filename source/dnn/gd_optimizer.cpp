/***************************************************************************
 *            gd_optimizer.cpp
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

#include "gd_optimizer.hpp"

namespace Ariadne {

GDOptimizer::GDOptimizer(NumType eta)
    : _eta{eta}
{ }

void GDOptimizer::train(Layer& layer) 
{
    size_t param_count = layer.param_count();
    for (size_t i = 0; i < param_count; ++i)
    {
        NumType& param    = *layer.param(i);
        NumType& gradient = *layer.gradient(i);

        param -= _eta * gradient;

        // Reset the gradient accumulated again in the next training epoch.
        gradient = NumType{0.0};
    }
}

} // namespace Ariadne
