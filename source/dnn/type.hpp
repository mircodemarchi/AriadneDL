/***************************************************************************
 *            type.hpp
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

/*! \file type.hpp
 *  \brief Simply replace me.
 */

#ifndef ARIADNE_DNN_TYPE_HPP
#define ARIADNE_DNN_TYPE_HPP

#include <random>

namespace Ariadne {

using NumType = double;

/**
 * Random number engine: 64-bit Mersenne Twister by Matsumoto and
 * Nishimura, 1998.
 */
using RneType = std::mt19937_64;

} // namespace Ariadne

#endif // ARIADNE_DNN_TYPE_HPP
