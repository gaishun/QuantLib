/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2005 StatPro Italia srl

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

/*! \file greeks.hpp
    \brief 希腊字母(Greeks)计算
    
    Greeks是期权风险管理的核心指标
    
    主要Greeks:
    1. Delta (Δ): ∂V/∂S
       - 标的价格变化1单位时期权价值变化
       - 对冲比率
    
    2. Gamma (Γ): ∂²V/∂S²
       - Delta的变化率
       - 高Gamma意味着高风险
    
    3. Vega (ν): ∂V/∂σ
       - 波动率变化1%时期权价值变化
       - 衡量波动率风险
    
    4. Theta (Θ): ∂V/∂t
       - 时间衰减(每天损失的价值)
       - 买方为负，卖方为正
    
    5. Rho (ρ): ∂V/∂r
       - 利率变化1%时期权价值变化
    
    计算方法:
    1. 解析法: Black-Scholes公式直接求导
    2. 有限差分: 数值微分
    3. 蒙特卡洛: 路径微分
    
    使用示例:
    Real delta = option.delta();
    Real gamma = option.gamma();
    Real vega = option.vega();
    Real theta = option.theta();
    Real rho = option.rho();
*/

#ifndef quantlib_greeks_hpp
#define quantlib_greeks_hpp

#include <ql/processes/blackscholesprocess.hpp>

namespace QuantLib {

    //! default theta calculation for Black-Scholes options
    Real blackScholesTheta(
                     const ext::shared_ptr<GeneralizedBlackScholesProcess>&,
                     Real value, Real delta, Real gamma);

    //! default theta-per-day calculation
    Real defaultThetaPerDay(Real theta);

}


#endif
