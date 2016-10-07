/*
 Copyright (C) 2016 Quaternion Risk Management Ltd
 All rights reserved.

 This file is part of ORE, a free-software/open-source library
 for transparent pricing and risk analysis - http://opensourcerisk.org

 ORE is free software: you can redistribute it and/or modify it
 under the terms of the Modified BSD License.  You should have received a
 copy of the license along with this program.
 The license is also available online at <http://opensourcerisk.org>

 This program is distributed on the basis that it will form a useful
 contribution to risk analytics and model standardisation, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE. See the license for more details.
*/

/*! \file ored/marketdata/fxspot.hpp
    \brief
    \ingroup marketdata
*/

#pragma once

#include <ored/marketdata/curvespec.hpp>
#include <ored/marketdata/loader.hpp>
#include <ql/quote.hpp>
#include <ql/handle.hpp>

namespace ore {
namespace data {

//! Wrapper class for holding FX (spot) quotes
/*!
  \ingroup marketdata
*/
class FXSpot {
public:
    //! Constructor
    FXSpot(const Date& asof, FXSpotSpec spec, const Loader& loader);

    //! Inspector
    Handle<Quote> spot() const { return spot_; }

private:
    Handle<Quote> spot_;
};
}
}
