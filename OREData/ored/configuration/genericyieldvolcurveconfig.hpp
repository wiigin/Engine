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

/*! \file ored/configuration/genericyieldvolcurveconfig.hpp
    \brief Swaption volatility curve configuration classes
    \ingroup configuration
*/

#pragma once

#include <ored/configuration/curveconfig.hpp>
#include <ored/configuration/reportconfig.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/daycounter.hpp>
#include <ql/time/period.hpp>
#include <ql/types.hpp>

namespace ore {
namespace data {
using ore::data::XMLNode;
using QuantLib::BusinessDayConvention;
using QuantLib::Calendar;
using QuantLib::DayCounter;
using QuantLib::Period;
using QuantLib::Spread;
using std::string;
using std::vector;

//! Generic yield volatility curve configuration class
/*!
  \ingroup configuration
*/
class GenericYieldVolatilityCurveConfig : public CurveConfig {
public:
    //! supported volatility dimensions
    enum class Dimension { ATM, Smile };
    // supported volatility types
    enum class VolatilityType { Lognormal, Normal, ShiftedLognormal };

    //! \name Constructors/Destructors
    //@{
    //! Default constructor
    GenericYieldVolatilityCurveConfig(const std::string& underlyingLabel, const std::string& rootNodeLabel,
				      const std::string& marketDatumInstrumentLabel, const std::string& qualifierLabel,
				      const bool allowSmile, const bool requireSwapIndexBases,
				      const std::string& smileDynamics = "")
	: underlyingLabel_(underlyingLabel), rootNodeLabel_(rootNodeLabel),
	  marketDatumInstrumentLabel_(marketDatumInstrumentLabel), qualifierLabel_(qualifierLabel),
	  allowSmile_(allowSmile), requireSwapIndexBases_(requireSwapIndexBases), smileDynamics_(smileDynamics) {}
    //! Detailed constructor
    GenericYieldVolatilityCurveConfig(const std::string& underlyingLabel, const std::string& rootNodeLabel,
				      const std::string& marketDatumInstrumentLabel, const std::string& qualifierLabel,
				      const string& curveID, const string& curveDescription, const string& qualifier,
				      const Dimension& dimension, const VolatilityType& volatilityType,
				      const bool extrapolate, const bool flatExtrapolation,
				      const vector<string>& optionTenors, const vector<string>& underlyingTenors,
				      const DayCounter& dayCounter, const Calendar& calendar,
				      const BusinessDayConvention& businessDayConvention,
				      const string& shortSwapIndexBase = "", const string& swapIndexBase = "",
				      // Only required for smile
				      const vector<string>& smileOptionTenors = vector<string>(),
				      const vector<string>& smileUnderlyingTenors = vector<string>(),
				      const vector<string>& smileSpreads = vector<string>(),
				      const std::string& smileDynamics = "");
    //! Detailed contructor for proxy config
    GenericYieldVolatilityCurveConfig(const std::string& underlyingLabel, const std::string& rootNodeLabel,
				      const std::string& qualifierLabel, const string& curveID,
				      const string& curveDescription, const string& qualifier,
				      const std::string& proxySourceCurveId,
				      const std::string& proxySourceShortSwapIndexBase,
				      const std::string& proxySourceSwapIndexBase,
				      const std::string& proxyTargetShortSwapIndexBase,
				      const std::string& proxyTargetSwapIndexBase,
				      const std::string& smileDynamics = "");

    //@}

    //! \name Serialisation
    //@{
    void fromXML(XMLNode* node) override;
    XMLNode* toXML(XMLDocument& doc) override;
    //@}

    //! \name Inspectors
    //@{
    const string& qualifier() const { return qualifier_; }
    const Dimension& dimension() const { return dimension_; }
    const VolatilityType& volatilityType() const { return volatilityType_; }
    const bool& extrapolate() const { return extrapolate_; }
    const bool& flatExtrapolation() const { return flatExtrapolation_; }
    const vector<string>& optionTenors() const { return optionTenors_; }
    const vector<string>& underlyingTenors() const { return underlyingTenors_; }
    const DayCounter& dayCounter() const { return dayCounter_; }
    const Calendar& calendar() const { return calendar_; }
    const BusinessDayConvention& businessDayConvention() const { return businessDayConvention_; }
    const string& shortSwapIndexBase() const { return shortSwapIndexBase_; }
    const string& swapIndexBase() const { return swapIndexBase_; }
    const vector<string>& smileOptionTenors() const { return smileOptionTenors_; }
    const vector<string>& smileUnderlyingTenors() const { return smileUnderlyingTenors_; }
    const vector<string>& smileSpreads() const { return smileSpreads_; }
    const string& quoteTag() const { return quoteTag_; }
    const vector<string>& quotes() override;
    const std::string& smileDynamics() const { return smileDynamics_; }
     //
    const std::string& proxySourceCurveId() const { return proxySourceCurveId_; }
    const std::string& proxySourceShortSwapIndexBase() const { return proxySourceShortSwapIndexBase_; }
    const std::string& proxySourceSwapIndexBase() const { return proxySourceSwapIndexBase_; }
    const std::string& proxyTargetShortSwapIndexBase() const { return proxyTargetShortSwapIndexBase_; }
    const std::string& proxyTargetSwapIndexBase() const { return proxyTargetSwapIndexBase_; }
    //
    const ReportConfig& reportConfig() const { return reportConfig_; }
    //@}

    //! \name Setters
    //@{
    string& qualifier() { return qualifier_; }
    Dimension& dimension() { return dimension_; }
    VolatilityType& volatilityType() { return volatilityType_; }
    bool& extrapolate() { return extrapolate_; }
    bool& flatExtrapolation() { return flatExtrapolation_; }
    vector<string>& optionTenors() { return optionTenors_; }
    vector<string>& underlyingTenors() { return underlyingTenors_; }
    DayCounter& dayCounter() { return dayCounter_; }
    Calendar& calendar() { return calendar_; }
    BusinessDayConvention& businessDayConvention() { return businessDayConvention_; }
    string& shortSwapIndexBase() { return shortSwapIndexBase_; }
    string& swapIndexBase() { return swapIndexBase_; }
    vector<string>& smileOptionTenors() { return smileOptionTenors_; }
    vector<string>& smileUnderlyingTenors() { return smileUnderlyingTenors_; }
    vector<string>& smileSpreads() { return smileSpreads_; }
    string& quoteTag() { return quoteTag_; }
    string& smileDynamics() { return smileDynamics_; }
     //@}

private:
    void populateRequiredCurveIds();
    string ccyFromSwapIndexBase(const std::string& swapIndexBase);
    //
    const string underlyingLabel_, rootNodeLabel_, marketDatumInstrumentLabel_, qualifierLabel_;
    const bool allowSmile_, requireSwapIndexBases_;
    //
    string qualifier_;
    Dimension dimension_ = Dimension::Smile;
    VolatilityType volatilityType_ = VolatilityType::Normal;
    bool extrapolate_ = true, flatExtrapolation_ = true;
    vector<string> optionTenors_, underlyingTenors_;
    DayCounter dayCounter_;
    Calendar calendar_;
    BusinessDayConvention businessDayConvention_;
    string shortSwapIndexBase_, swapIndexBase_;
    vector<string> smileOptionTenors_;
    vector<string> smileUnderlyingTenors_;
    vector<string> smileSpreads_;
    string quoteTag_;
    string smileDynamics_;
  
    string proxySourceCurveId_;
    string proxySourceShortSwapIndexBase_;
    string proxySourceSwapIndexBase_;
    string proxyTargetShortSwapIndexBase_;
    string proxyTargetSwapIndexBase_;

    ReportConfig reportConfig_;
};

std::ostream& operator<<(std::ostream& out, GenericYieldVolatilityCurveConfig::VolatilityType t);

} // namespace data
} // namespace ore
