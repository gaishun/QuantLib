/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2004, 2005, 2006, 2007 Ferdinando Ametrano

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

/*! \file compounding.hpp
    \brief 复利计息方式(Compounding)
    
    复利方式是金融计算的核心
    
    枚举类型:
    1. Simple (单利):
       - 公式: FV = PV × (1 + r × t)
       - 利息不产生利息
       
    2. Compounded (复利):
       - 公式: FV = PV × (1 + r)^t
       - 利息产生利息
       - 每年n次复利: FV = PV × (1 + r/n)^(n×t)
       
    3. Continuous (连续复利):
       - 公式: FV = PV × e^(r×t)
       - 无限高频复利
       
    4. SimpleThenCompounded:
       - 前期单利，后期复利
       
    5. CompoundedThenSimple:
       - 前期复利，后期单利
    
    使用示例:
    // 100元，5%年利率，1年
    Real pv = 100;
    Rate r = 0.05;
    Time t = 1;
    
    // 单利
    Real simple = pv * (1 + r * t);  // 105
    
    // 年复利
    Real compounded = pv * pow(1 + r, t);  // 105
    
    // 连续复利
    Real continuous = pv * exp(r * t);  // 105.127
*/

#ifndef quantlib_compounding_hpp
#define quantlib_compounding_hpp

#include <ql/qldefines.hpp>

namespace QuantLib {

    //! Interest rate coumpounding rule
    enum Compounding { Simple = 0,          //!< \f$ 1+rt \f$
                       Compounded = 1,      //!< \f$ (1+r)^t \f$
                       Continuous = 2,      //!< \f$ e^{rt} \f$
                       SimpleThenCompounded, //!< Simple up to the first period then Compounded
                       CompoundedThenSimple //!< Compounded up to the first period then Simple
    };

}

#endif
