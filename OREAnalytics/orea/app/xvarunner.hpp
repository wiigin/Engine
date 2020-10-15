/*
 Copyright (C) 2020 Quaternion Risk Management Ltd
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

/*! \file orea/app/xvarunner.hpp
    \brief A class to run the xva analysis
    \ingroup app
*/
#pragma once

#include <orea/aggregation/postprocess.hpp>
#include <orea/engine/valuationcalculator.hpp>
#include <orea/scenario/scenariogeneratordata.hpp>
#include <orea/scenario/scenariosimmarketparameters.hpp>
#include <ored/configuration/curveconfigurations.hpp>
#include <ored/model/crossassetmodeldata.hpp>

#include <boost/optional.hpp>

namespace ore {
namespace analytics {

class XvaRunner {
public:
    virtual ~XvaRunner() {}

    XvaRunner(QuantLib::Date asof, const std::string& baseCurrency,
              const boost::shared_ptr<ore::data::Portfolio>& portfolio,
              const boost::shared_ptr<ore::data::NettingSetManager>& netting,
              const boost::shared_ptr<ore::data::EngineData>& engineData,
              const boost::shared_ptr<ore::data::CurveConfigurations>& curveConfigs,
              const boost::shared_ptr<ore::data::Conventions>& conventions,
              const boost::shared_ptr<ore::data::TodaysMarketParameters>& todaysMarketParams,
              const boost::shared_ptr<ScenarioSimMarketParameters>& simMarketData,
              const boost::shared_ptr<ScenarioGeneratorData>& scenarioGeneratorData,
              const boost::shared_ptr<ore::data::CrossAssetModelData>& crossAssetModelData,
              std::vector<boost::shared_ptr<ore::data::LegBuilder>> extraLegBuilders = {},
              std::vector<boost::shared_ptr<ore::data::EngineBuilder>> extraEngineBuilders = {},
              const boost::shared_ptr<ReferenceDataManager>& referenceData = nullptr, QuantLib::Real dimQuantile = 0.99,
              QuantLib::Size dimHorizonCalendarDays = 14, map<string, bool> analytics = {},
              string calculationType = "Symmetric", string dvaName = "", string fvaBorrowingCurve = "",
              string fvaLendingCurve = "", bool fullInitialCollateralisation = true, bool storeFlows = false);

    // run xva on full portfolio, generate post processor
    void runXva(const boost::shared_ptr<ore::data::Market>& market, bool continueOnErr = true);

    // get post processor from runXva() or generatePostProcessor() call
    const boost::shared_ptr<PostProcess>& postProcess() { return postProcess_; }

    // partial step 1: prepare simulation (build cam model and sim market / factory), with optional ccy filter
    void prepareSimulation(const boost::shared_ptr<ore::data::Market>& market, bool continueOnErr = true,
                           const boost::optional<std::set<std::string>>& currencyFilter = boost::none);

    // partial step 2: build trade and netting set cubes, optionally filtered on a subset of given trade ids
    //                 if the filter is given, the order of the trades will follow the one in the given filter set
    void buildCube(const boost::optional<std::set<std::string>>& tradeIds, bool continueOnErr = true);

    // get generated trade cube from last buildCube() or runXva() run
    boost::shared_ptr<NPVCube> npvCube() const { return cube_; }

    // get generated netting set cube from last buildCube() or runXva() run
    boost::shared_ptr<NPVCube> nettingCube() const { return nettingCube_; }

    // get aggregation scenario data from last buildCube() (if enabled there) or runXVA() run
    boost::shared_ptr<AggregationScenarioData> aggregationScenarioData() { return scenarioData_; }

    // partial step 3: build post processor on given cubes / agg scen data (requires runXva() or buildCube() run before)
    void generatePostProcessor(const boost::shared_ptr<Market>& market, const boost::shared_ptr<NPVCube>& npvCube,
                               const boost::shared_ptr<NPVCube>& nettingCube,
                               const boost::shared_ptr<AggregationScenarioData>& scenarioData,
                               const bool continueOnErr = true);

    // get a vector of netting set ids for the given portfolio sorted in alphabetical order, if no portfolio
    // is given here, the netting sets for the global portfolio set in the ctor are returned
    std::vector<std::string> getNettingSetIds(const boost::shared_ptr<Portfolio>& portfolio = nullptr) const;

protected:
    virtual boost::shared_ptr<NPVCube>
    getNettingSetCube(std::vector<boost::shared_ptr<ValuationCalculator>>& calculators,
                      const boost::shared_ptr<Portfolio>& portfolio) {
        return nullptr;
    };

    virtual boost::shared_ptr<DynamicInitialMarginCalculator>
    getDimCalculator(const boost::shared_ptr<NPVCube>& cube,
                     const boost::shared_ptr<CubeInterpretation>& cubeInterpreter,
                     const boost::shared_ptr<AggregationScenarioData>& scenarioData,
                     const boost::shared_ptr<QuantExt::CrossAssetModel>& model = nullptr,
                     const boost::shared_ptr<NPVCube>& nettingCube = nullptr);

    virtual boost::shared_ptr<ore::analytics::ScenarioSimMarketParameters>
    projectSsmData(const std::set<std::string>& currencyFilter) const;

    virtual boost::shared_ptr<ore::analytics::ScenarioGenerator> getProjectedScenarioGenerator(
        const boost::optional<std::set<std::string>>& currencyFilter, const boost::shared_ptr<Market>& market,
        const boost::shared_ptr<ScenarioSimMarketParameters>& projectedSsmData,
        const boost::shared_ptr<ScenarioFactory>& scenarioFactory, const bool continueOnErr) const;

    QuantLib::Date asof_;
    std::string baseCurrency_;
    boost::shared_ptr<ore::data::Portfolio> portfolio_;
    boost::shared_ptr<ore::data::NettingSetManager> netting_;
    boost::shared_ptr<ore::data::EngineData> engineData_;
    boost::shared_ptr<ore::data::CurveConfigurations> curveConfigs_;
    boost::shared_ptr<ore::data::Conventions> conventions_;
    boost::shared_ptr<ore::data::TodaysMarketParameters> todaysMarketParams_;
    boost::shared_ptr<ScenarioSimMarketParameters> simMarketData_;
    boost::shared_ptr<ScenarioGeneratorData> scenarioGeneratorData_;
    boost::shared_ptr<ore::data::CrossAssetModelData> crossAssetModelData_;
    std::vector<boost::shared_ptr<ore::data::LegBuilder>> extraLegBuilders_;
    std::vector<boost::shared_ptr<ore::data::EngineBuilder>> extraEngineBuilders_;
    boost::shared_ptr<ReferenceDataManager> referenceData_;
    QuantLib::Real dimQuantile_;
    QuantLib::Size dimHorizonCalendarDays_;
    map<string, bool> analytics_;
    string inputCalculationType_;
    string dvaName_;
    string fvaBorrowingCurve_;
    string fvaLendingCurve_;
    bool fullInitialCollateralisation_;
    bool storeFlows_;

    // generated data
    boost::shared_ptr<QuantExt::CrossAssetModel> model_;
    bool modelIsCalibrated_;
    boost::shared_ptr<ScenarioSimMarket> simMarket_;
    boost::shared_ptr<EngineFactory> simFactory_;
    boost::shared_ptr<AggregationScenarioData> scenarioData_;
    boost::shared_ptr<NPVCube> cube_, nettingCube_;
    boost::shared_ptr<CubeInterpretation> cubeInterpreter_;
    std::string calculationType_;
    boost::shared_ptr<PostProcess> postProcess_;
};

} // namespace analytics
} // namespace ore
