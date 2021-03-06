/***************************************************************************
 *            layer.hpp
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

/*! \file layer.hpp
 *  \brief High level layer of a deep neural network.
 */

#ifndef ARIADNE_DNN_LAYER_HPP
#define ARIADNE_DNN_LAYER_HPP

#include "type.hpp"

#include <cstdint>
#include <string>
#include <vector>


namespace Ariadne {

class Model;

/**
 * \brief Base class of computational layers in a model.
 */
class Layer 
{
public:
    Layer(Model& model, std::string name);
    virtual ~Layer() = default;

    /**
     * \brief Virtual method used to describe how a layer should be 
     * initialized.
     * \param rne RneType
     */
    virtual void init(RneType& rne) = 0;

    /**
     * \brief Virtual method used to perform forward propagations. During 
     * forward propagation nodes transform input data and feed results to all 
     * subsequent nodes.
     * \param inputs NumType ptr
     */
    virtual void forward(NumType* inputs) = 0;

    /**
     * \brief Virtual method used to perform reverse propagations. During 
     * reverse propagation nodes receive loss gradients to its previous outputs
     * and compute gradients with respect to each tunable parameter.
     * Compute dJ/dz = dJ/dg(z) * dg(z)/dz.
     * \param gradients NumType ptr dJ/dg(z)
     */
    virtual void reverse(NumType* gradients) = 0;

    /**
     * \brief Virtual method that return the number of tunable parameters. 
     * This methos should be overridden to reflect the quantity of tunable 
     * parameters.
     * \return size_t The amount of tunable parameters. 
     */
    virtual size_t param_count() const noexcept { return 0; }

    /**
     * \brief Virtual method accessor for parameter by index.
     * \param index size_t Parameter index.
     * \return NumType* Pointer to parameter.
     */
    virtual NumType* param(size_t index) { (void) index; return nullptr; }

    /**
     * \brief Virtual method accessor for loss-gradient with respect to a 
     * parameter specified by index.
     * \param index size_t Parameter index.
     * \return NumType* Pointer to gradient value of parameter.
     */
    virtual NumType* gradient(size_t index) { (void) index; return nullptr; }

    /**
     * \brief Print.
     */
    virtual void print() const = 0;

    /**
     * \brief Virtual method information dump for debugging purposes.
     * \return std::string const& The layer name.
     */
    [[nodiscard]] std::string const& name() const noexcept 
    { 
        return _name; 
    }

protected:
    friend class Model;

    Model& _model;                      ///< Model reference.
    std::string _name;                  ///< Layer naem (for debug).
    std::vector<Layer*> _antecedents;   ///< List of previous layers.
    std::vector<Layer*> _subsequents;   ///< List of followers layers.
};

} // namespace Ariadne

#endif // ARIADNE_DNN_LAYER_HPP
