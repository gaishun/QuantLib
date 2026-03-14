/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2001, 2002, 2003 Sadruddin Rejeb
 Copyright (C) 2003 Ferdinando Ametrano
 Copyright (C) 2004, 2005, 2006, 2007, 2009 StatPro Italia srl
 Copyright (C) 2015 Peter Caspers

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

/*! \file blackscholesprocess.hpp
    \brief Black-Scholes 随机过程
    
    BlackScholesProcess是期权定价最核心的随机过程
    假设标的资产价格满足几何布朗运动:
    dS = (r - q)S*dt + sigma*S*dW
    
    主要组成部分:
    1. 标的资产价格(S): 股票、期货等
    2. 无风险利率(r): 用于折现
    3. 股息收益率(q): 股票分红
    4. 波动率(sigma): 资产价格的波动程度
    
    两种实现:
    1. BlackScholesMertonProcess: 考虑股息
    2. BlackScholesProcess: 不考虑股息
    
    使用示例:
    // 创建Black-Scholes过程
    Handle<Quote> spot = ...;  // 标的价格
    Handle<YieldTermStructure> r = ...;  // 无风险利率
    Handle<YieldTermStructure> q = ...;  // 股息收益率
    Handle<BlackVolTermStructure> vol = ...;  // 波动率
    
    ext::shared_ptr<BlackScholesMertonProcess> process = 
        ext::make_shared<BlackScholesMertonProcess>(spot, q, r, vol);
    
    // 用于蒙特卡洛模拟
    // 用于二叉树
    // 用于有限差分
*/

#ifndef quantlib_black_scholes_process_hpp
#define quantlib_black_scholes_process_hpp

#include <ql/stochasticprocess.hpp>
#include <ql/processes/eulerdiscretization.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>
#include <ql/termstructures/volatility/equityfx/blackvoltermstructure.hpp>
#include <ql/termstructures/volatility/equityfx/localvoltermstructure.hpp>
#include <ql/quote.hpp>

namespace QuantLib {

    class LocalConstantVol;
    class LocalVolCurve;

    //! Generalized Black-Scholes stochastic process
    /*! This class describes the stochastic process \f$ S \f$ governed by
        \f[
            d\ln S(t) = (r(t) - q(t) - \frac{\sigma(t, S)^2}{2}) dt
                     + \sigma dW_t.
        \f]

        \warning while the interface is expressed in terms of \f$ S \f$,
                 the internal calculations work on \f$ ln S \f$.

        \ingroup processes
    */
    class GeneralizedBlackScholesProcess : public StochasticProcess1D {
      public:
        GeneralizedBlackScholesProcess(Handle<Quote> x0,
                                       Handle<YieldTermStructure> dividendTS,
                                       Handle<YieldTermStructure> riskFreeTS,
                                       Handle<BlackVolTermStructure> blackVolTS,
                                       const ext::shared_ptr<discretization>& d =
                                           ext::shared_ptr<discretization>(new EulerDiscretization),
                                       bool forceDiscretization = false);

        GeneralizedBlackScholesProcess(Handle<Quote> x0,
                                       Handle<YieldTermStructure> dividendTS,
                                       Handle<YieldTermStructure> riskFreeTS,
                                       Handle<BlackVolTermStructure> blackVolTS,
                                       Handle<LocalVolTermStructure> localVolTS);

        //! \name StochasticProcess1D interface
        //@{
        Real x0() const override;
        /*! \todo revise extrapolation */
        Real drift(Time t, Real x) const override;
        /*! \todo revise extrapolation */
        Real diffusion(Time t, Real x) const override;
        Real apply(Real x0, Real dx) const override;
        /*! \warning in general raises a "not implemented" exception.
                     It should be rewritten to return the expectation E(S)
                     of the process, not exp(E(log S)).
        */
        Real expectation(Time t0, Real x0, Time dt) const override;
        Real stdDeviation(Time t0, Real x0, Time dt) const override;
        Real variance(Time t0, Real x0, Time dt) const override;
        Real evolve(Time t0, Real x0, Time dt, Real dw) const override;
        //@}
        Time time(const Date&) const override;
        //! \name Observer interface
        //@{
        void update() override;
        //@}
        //! \name Inspectors
        //@{
        const Handle<Quote>& stateVariable() const;
        const Handle<YieldTermStructure>& dividendYield() const;
        const Handle<YieldTermStructure>& riskFreeRate() const;
        const Handle<BlackVolTermStructure>& blackVolatility() const;
        const Handle<LocalVolTermStructure>& localVolatility() const;
        //@}
      private:
        Handle<Quote> x0_;
        Handle<YieldTermStructure> riskFreeRate_, dividendYield_;
        Handle<BlackVolTermStructure> blackVolatility_;
        Handle<LocalVolTermStructure> externalLocalVolTS_;
        bool forceDiscretization_;
        bool hasExternalLocalVol_;
        mutable RelinkableHandle<LocalVolTermStructure> localVolatility_;
        mutable bool updated_, isStrikeIndependent_;
    };

    //! Black-Scholes (1973) stochastic process
    /*! This class describes the stochastic process \f$ S \f$ for a stock
        given by
        \f[
            d\ln S(t) = (r(t) - \frac{\sigma(t, S)^2}{2}) dt + \sigma dW_t.
        \f]

        \warning while the interface is expressed in terms of \f$ S \f$,
                 the internal calculations work on \f$ ln S \f$.

        \ingroup processes
    */
    class BlackScholesProcess : public GeneralizedBlackScholesProcess {
      public:
        BlackScholesProcess(
            const Handle<Quote>& x0,
            const Handle<YieldTermStructure>& riskFreeTS,
            const Handle<BlackVolTermStructure>& blackVolTS,
            const ext::shared_ptr<discretization>& d =
                  ext::shared_ptr<discretization>(new EulerDiscretization),
            bool forceDiscretization = false);
    };

    //! Merton (1973) extension to the Black-Scholes stochastic process
    /*! This class describes the stochastic process ln(S) for a stock or
        stock index paying a continuous dividend yield given by
        \f[
            d\ln S(t, S) = (r(t) - q(t) - \frac{\sigma(t, S)^2}{2}) dt
                     + \sigma dW_t.
        \f]

        \ingroup processes
    */
    class BlackScholesMertonProcess : public GeneralizedBlackScholesProcess {
      public:
        BlackScholesMertonProcess(
            const Handle<Quote>& x0,
            const Handle<YieldTermStructure>& dividendTS,
            const Handle<YieldTermStructure>& riskFreeTS,
            const Handle<BlackVolTermStructure>& blackVolTS,
            const ext::shared_ptr<discretization>& d =
                  ext::shared_ptr<discretization>(new EulerDiscretization),
            bool forceDiscretization = false);
    };

    //! Black (1976) stochastic process
    /*! This class describes the stochastic process \f$ S \f$ for a
        forward or futures contract given by
        \f[
            d\ln S(t) = -\frac{\sigma(t, S)^2}{2} dt + \sigma dW_t.
        \f]

        \warning while the interface is expressed in terms of \f$ S \f$,
                 the internal calculations work on \f$ ln S \f$.

        \ingroup processes
    */
    class BlackProcess : public GeneralizedBlackScholesProcess {
      public:
        BlackProcess(
            const Handle<Quote>& x0,
            const Handle<YieldTermStructure>& riskFreeTS,
            const Handle<BlackVolTermStructure>& blackVolTS,
            const ext::shared_ptr<discretization>& d =
                  ext::shared_ptr<discretization>(new EulerDiscretization),
            bool forceDiscretization = false);
    };

    //! Garman-Kohlhagen (1983) stochastic process
    /*! This class describes the stochastic process \f$ S \f$ for an exchange
        rate given by
        \f[
            d\ln S(t) = (r(t) - r_f(t) - \frac{\sigma(t, S)^2}{2}) dt
                     + \sigma dW_t.
        \f]

        \warning while the interface is expressed in terms of \f$ S \f$,
                 the internal calculations work on \f$ ln S \f$.

        \ingroup processes
    */
    class GarmanKohlagenProcess : public GeneralizedBlackScholesProcess {
      public:
        GarmanKohlagenProcess(
            const Handle<Quote>& x0,
            const Handle<YieldTermStructure>& foreignRiskFreeTS,
            const Handle<YieldTermStructure>& domesticRiskFreeTS,
            const Handle<BlackVolTermStructure>& blackVolTS,
            const ext::shared_ptr<discretization>& d =
                  ext::shared_ptr<discretization>(new EulerDiscretization),
            bool forceDiscretization = false);
    };

}


#endif
