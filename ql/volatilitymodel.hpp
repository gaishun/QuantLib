/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2006 Joseph Wang

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file volatilitymodel.hpp
    \brief 波动率模型(Volatility Model)
    
    波动率是期权定价的核心参数
    
    主要类型:
    
    1. 波动率期限结构:
       - BlackVolCurve: Black波动率曲线
       - BlackVolSurface: Black波动率曲面
       - LocalVolSurface: 本地波动率曲面
       - StochasticVolModel: 随机波动率模型(SABR, Heston)
       
    2. 波动率模型:
       - BlackVarianceSurface: Black方差曲面
       - LocalVolatilityEstimator: 本地波动率估计
    
    3. 参数模型:
       - SABR: SABR波动率模型
       - Heston: Heston随机波动率模型
       - Bergomi: Bergomi模型
    
    使用示例:
    // 创建波动率曲面
    Date today = Date::todaysDate();
    DayCounter dc = Actual365Fixed();
    
    // 波动率矩阵(执行价 x 到期时间)
    std::vector<Real> strikes = {80, 90, 100, 110, 120};
    std::vector<Date> dates = {today + 1*Months, today + 3*Months, ...};
    Matrix volMatrix(strikes.size(), dates.size());
    // 填充波动率...
    
    BlackVarianceSurface surface(today, NullCalendar(), 
                              strikes, dates, volMatrix, dc);
    surface.setInterpolation<Bilinear>();
*/

#ifndef quantlib_volatility_model_hpp
#define quantlib_volatility_model_hpp

#include <ql/types.hpp>
#include <ql/timeseries.hpp>


namespace QuantLib {

    template <class T>
    class LocalVolatilityEstimator {
      public:
        virtual ~LocalVolatilityEstimator() = default;
        virtual TimeSeries<Volatility>
        calculate(const TimeSeries<T> &quoteSeries) = 0;
    };

    class VolatilityCompositor {
      public:
        typedef TimeSeries<Volatility> time_series;
        virtual ~VolatilityCompositor() = default;
        virtual time_series calculate(const time_series& volatilitySeries) = 0;
        virtual void calibrate(const time_series& volatilitySeries) = 0;
    };

}


#endif
