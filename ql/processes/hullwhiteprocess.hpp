/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2006 Banca Profilo S.p.A.

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <https://www.quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file hullwhiteprocess.hpp
    \brief Hull-White随机过程
    
    Hull-White模型是最流行的利率模型之一
    能够复现当前收益率曲线的形状
    
    数学公式:
    dr(t) = [θ(t) - a·r(t)]dt + σ·dW(t)
    
    参数说明:
    - a: 均值回归速度(mean reversion)
    - σ: 波动率(volatility)
    - θ(t): 时间相关参数，用于拟合收益率曲线
    
    特点:
    - 能够精确拟合初始收益率曲线
    - 产生合理的利率期限结构
    - 可用于利率衍生品定价
    
    使用示例:
    Handle<YieldTermStructure> curve = ...;  // 初始收益率曲线
    Real a = 0.1;  // 均值回归速度
    Real sigma = 0.01;  // 波动率
    
    ext::shared_ptr<HullWhiteProcess> hw = 
        ext::make_shared<HullWhiteProcess>(curve, a, sigma);
*/

#ifndef quantlib_hull_white_processes_hpp
#define quantlib_hull_white_processes_hpp

#include <ql/processes/forwardmeasureprocess.hpp>
#include <ql/processes/ornsteinuhlenbeckprocess.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>

namespace QuantLib {

    //! Hull-White stochastic process
    /*! \ingroup processes */
    class HullWhiteProcess: public StochasticProcess1D {
      public:
        HullWhiteProcess(const Handle<YieldTermStructure>& h,
                         Real a,
                         Real sigma);
        //! \name StochasticProcess1D interface
        //@{
        Real x0() const override;
        Real drift(Time t, Real x) const override;
        Real diffusion(Time t, Real x) const override;
        Real expectation(Time t0, Real x0, Time dt) const override;
        Real stdDeviation(Time t0, Real x0, Time dt) const override;
        Real variance(Time t0, Real x0, Time dt) const override;

        Real a() const;
        Real sigma() const;
        Real alpha(Time t) const;
        //@}
    protected:
        ext::shared_ptr<QuantLib::OrnsteinUhlenbeckProcess> process_;
        Handle<YieldTermStructure> h_;
        Real a_, sigma_;
    };

    //! %Forward Hull-White stochastic process
    /*! \ingroup processes */
    class HullWhiteForwardProcess: public ForwardMeasureProcess1D {
      public:
        HullWhiteForwardProcess(const Handle<YieldTermStructure>& h,
                                Real a,
                                Real sigma);
        //! \name StochasticProcess1D interface
        //@{
        Real x0() const override;
        Real drift(Time t, Real x) const override;
        Real diffusion(Time t, Real x) const override;
        Real expectation(Time t0, Real x0, Time dt) const override;
        Real stdDeviation(Time t0, Real x0, Time dt) const override;
        Real variance(Time t0, Real x0, Time dt) const override;
        //@}

        Real a() const;
        Real sigma() const;
        Real alpha(Time t) const;
        Real M_T(Real s, Real t, Real T) const;
        Real B(Time t, Time T) const;

    protected:
        ext::shared_ptr<QuantLib::OrnsteinUhlenbeckProcess> process_;
        Handle<YieldTermStructure> h_;
        Real a_, sigma_;
    };

}


#endif
