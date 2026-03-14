/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl
 Copyright (C) 2003, 2004, 2005, 2007 StatPro Italia srl

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

/*! \file flatforward.hpp
    \brief 平直收益率曲线(Flat Forward Curve)
    
    FlatForward是最简单的收益率曲线
    假设整个期限内的利率是常数
    
    特点:
    - 简单: 只有一个参数(利率值)
    - 快速: 计算最简单
    - 适用: 短期或近似计算
    
    与其他曲线比较:
    - FlatForward: 常数利率
    - ZeroCurve: 逐点插值
    - ForwardCurve: 远期利率曲线
    - InterpolatedCurve: 插值收益率曲线
    
    使用示例:
    // 创建平直曲线
    Date referenceDate = Date::todaysDate();
    DayCounter dc = Actual365Fixed();
    Rate r = 0.03;  // 3%
    
    // 方式1: 使用固定值
    ext::shared_ptr<YieldTermStructure> curve = 
        ext::make_shared<FlatForward>(referenceDate, r, dc, Continuous);
    
    // 方式2: 使用Quote
    Handle<Quote> forward = ...;
    ext::shared_ptr<YieldTermStructure> curve2 = 
        ext::make_shared<FlatForward>(referenceDate, forward, dc);
    
    // 获取零息利率
    Date date1Y = referenceDate + 1*Years;
    Rate zeroRate = curve->zeroRate(date1Y, Continuous);
    
    // 获取折现因子
    DiscountFactor df = curve->discount(date1Y);
*/

#ifndef quantlib_flat_forward_curve_hpp
#define quantlib_flat_forward_curve_hpp

#include <ql/patterns/lazyobject.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>
#include <ql/quote.hpp>

namespace QuantLib {

    //! Flat interest-rate curve
    /*! \ingroup yieldtermstructures */
    class FlatForward : public YieldTermStructure,
                        public LazyObject {
      public:
        //! \name Constructors
        //@{
        FlatForward(const Date& referenceDate,
                    Handle<Quote> forward,
                    const DayCounter& dayCounter,
                    Compounding compounding = Continuous,
                    Frequency frequency = Annual);
        FlatForward(const Date& referenceDate,
                    Rate forward,
                    const DayCounter& dayCounter,
                    Compounding compounding = Continuous,
                    Frequency frequency = Annual);
        FlatForward(Natural settlementDays,
                    const Calendar& calendar,
                    Handle<Quote> forward,
                    const DayCounter& dayCounter,
                    Compounding compounding = Continuous,
                    Frequency frequency = Annual);
        FlatForward(Natural settlementDays,
                    const Calendar& calendar,
                    Rate forward,
                    const DayCounter& dayCounter,
                    Compounding compounding = Continuous,
                    Frequency frequency = Annual);
        //@}

        // inspectors
        Compounding compounding() const { return compounding_; }
        Frequency compoundingFrequency() const { return frequency_; }

        //! \name TermStructure interface
        //@{
        Date maxDate() const override { return Date::maxDate(); }
        //@}

        //! \name Observer interface
        //@{
        void update() override;
        //@}
      private:
        //! \name LazyObject interface
        //@{
        void performCalculations() const override;
        //@}

        //! \name YieldTermStructure implementation
        //@{
        DiscountFactor discountImpl(Time) const override;
        //@}

        Handle<Quote> forward_;
        Compounding compounding_;
        Frequency frequency_;
        mutable InterestRate rate_;
    };

    // inline definitions

    inline void FlatForward::update() {
        LazyObject::update();
        YieldTermStructure::update();
    }

    inline DiscountFactor FlatForward::discountImpl(Time t) const {
        calculate();
        return rate_.discountFactor(t);
    }
  
    inline void FlatForward::performCalculations() const {
        rate_ = InterestRate(forward_->value(), dayCounter(),
                             compounding_, frequency_);
    }

}

#endif
