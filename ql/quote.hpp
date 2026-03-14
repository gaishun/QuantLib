/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2007 Ferdinando Ametrano
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl

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

/*! \file quote.hpp
    \brief 市场观测值(Quote)基类
    
    Quote代表金融市场的各种观测值
    是QuantLib市场数据系统的核心
    
    主要类型:
    
    1. SimpleQuote: 简单观测值
       - 固定数值
       - 例如: 股票价格100.0
    
    2. ForwardValueQuote: 远期价值
       - 远期合约的价值
    
    3. CompositeQuote: 组合观测值
       - 由多个Quote组合
    
    4. DerivedQuote: 派生观测值
       - 基于其他Quote计算
    
    使用场景:
    - 标的价格
    - 利率
    - 波动率
    - 汇率
    
    使用示例:
    // 创建简单报价
    Real spotPrice = 100.0;
    ext::shared_ptr<Quote> quote = 
        ext::make_shared<SimpleQuote>(spotPrice);
    
    // 获取价格
    Real price = quote->value();
    
    // 价格变化通知(观察者模式)
    quote->registerObserver(myObserver);
*/

#ifndef quantlib_quote_hpp
#define quantlib_quote_hpp

#include <ql/handle.hpp>
#include <ql/errors.hpp>
#include <ql/utilities/null.hpp>

namespace QuantLib {

    //! purely virtual base class for market observables
    /*! \test the observability of class instances is tested.
     */
    class Quote : public virtual Observable {
      public:
        ~Quote() override = default;
        //! returns the current value
        virtual Real value() const = 0;
        //! returns true if the Quote holds a valid value
        virtual bool isValid() const = 0;
    };

}

#endif
