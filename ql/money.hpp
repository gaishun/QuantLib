/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2004 Decillion Pty(Ltd)
 Copyright (C) 2004, 2005, 2006 StatPro Italia srl

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

/*! \file money.hpp
    \brief 货币金额(Money)类
    
    Money类处理不同货币的金额计算
    是QuantLib多货币系统的核心
    
    主要功能:
    1. 货币金额表示
       - 金额数值
       - 货币类型(USD, EUR, CNY等)
    
    2. 货币转换
       - 汇率转换
       - 自动舍入
    
    3. 算术运算
       - 加减乘除
       - 货币必须相同才能直接运算
    
    使用示例:
    // 创建货币金额
    Money m1(100, USD);  // 100美元
    Money m2(80, EUR);   // 80欧元
    
    // 金额操作
    Money sum = m1 + m1;  // 200美元
    Money diff = m1 - Money(50, USD);  // 50美元
    
    // 货币转换(需要汇率)
    Money m3 = m1.convert(EUR);  // 转换为欧元
    
    // 显示
    std::cout << m1 << std::endl;  // "100.00 USD"
*/

#ifndef quantlib_money_hpp
#define quantlib_money_hpp

#include <ql/currency.hpp>
#include <ql/patterns/singleton.hpp>
#include <utility>

namespace QuantLib {

    //! amount of cash
    /*! \test money arithmetic is tested with and without currency
              conversions.
    */
    class Money {
      public:
        //! \name Constructors
        //@{
        Money() = default;
        Money(Currency currency, Decimal value);
        Money(Decimal value, Currency currency);
        //@}
        //! \name Inspectors
        //@{
        const Currency& currency() const;
        Decimal value() const;
        Money rounded() const;
        //@}
        /*! \name Money arithmetics

            See below for non-member functions and for settings which
            determine the behavior of the operators.
        */
        //@{
        Money operator+() const;
        Money operator-() const;
        Money& operator+=(const Money&);
        Money& operator-=(const Money&);
        Money& operator*=(Decimal);
        Money& operator/=(Decimal);
        //@}
        /*! \name Conversion settings

            These parameters are used for combining money amounts
            in different currencies
        */
        //@{
        enum ConversionType {
            NoConversion,           /*!< do not perform conversions */
            BaseCurrencyConversion, /*!< convert both operands to
                                         the base currency before
                                         converting */
            AutomatedConversion     /*!< return the result in the
                                         currency of the first
                                         operand */
        };
        // Money::Settings forward declaration
        class Settings;
        //@}
      private:
        Decimal value_ = 0.0;
        Currency currency_;

        // temporary support for old syntax
        struct BaseCurrencyProxy {
          public:
            BaseCurrencyProxy& operator=(const Currency&);
            operator Currency() const;
        };

        struct ConversionTypeProxy {
          public:
            ConversionTypeProxy& operator=(Money::ConversionType);
            operator Money::ConversionType() const;
        };
    };

    //! Per-session settings for the Money class
    class Money::Settings : public Singleton<Money::Settings> {
        friend class Singleton<Money::Settings>;
      private:
        Settings() = default;

      public:
        const Money::ConversionType & conversionType() const;
        Money::ConversionType & conversionType();

        const Currency & baseCurrency() const;
        Currency & baseCurrency();

      private:
        Money::ConversionType conversionType_ = Money::NoConversion;
        Currency baseCurrency_;
    };

    // More arithmetics and comparisons

    /*! \relates Money */
    Money operator+(const Money&, const Money&);
    /*! \relates Money */
    Money operator-(const Money&, const Money&);
    /*! \relates Money */
    Money operator*(const Money&, Decimal);
    /*! \relates Money */
    Money operator*(Decimal, const Money&);
    /*! \relates Money */
    Money operator/(const Money&, Decimal);
    /*! \relates Money */
    Decimal operator/(const Money&, const Money&);

    /*! \relates Money */
    bool operator==(const Money&, const Money&);
    /*! \relates Money */
    bool operator!=(const Money&, const Money&);
    /*! \relates Money */
    bool operator<(const Money&, const Money&);
    /*! \relates Money */
    bool operator<=(const Money&, const Money&);
    /*! \relates Money */
    bool operator>(const Money&, const Money&);
    /*! \relates Money */
    bool operator>=(const Money&, const Money&);

    /*! \relates Money */
    bool close(const Money&, const Money&, Size n = 42);
    /*! \relates Money */
    bool close_enough(const Money&, const Money&, Size n = 42);

    // syntactic sugar

    /*! \relates Money */
    Money operator*(Decimal, const Currency&);
    /*! \relates Money */
    Money operator*(const Currency&, Decimal);

    // formatting

    /*! \relates Money */
    std::ostream& operator<<(std::ostream&, const Money&);


    // inline definitions

    inline Money::Money(Currency currency, Decimal value)
    : value_(value), currency_(std::move(currency)) {}

    inline Money::Money(Decimal value, Currency currency)
    : value_(value), currency_(std::move(currency)) {}

    inline const Currency& Money::currency() const {
        return currency_;
    }

    inline Decimal Money::value() const {
        return value_;
    }

    inline Money Money::rounded() const {
        return Money(currency_.rounding()(value_), currency_);
    }

    inline Money Money::operator+() const {
        return *this;
    }

    inline Money Money::operator-() const {
        return Money(-value_, currency_);
    }

    inline Money& Money::operator*=(Decimal x) {
        value_ *= x;
        return *this;
    }

    inline Money& Money::operator/=(Decimal x) {
        value_ /= x;
        return *this;
    }


    inline Money operator+(const Money& m1, const Money& m2) {
        Money tmp = m1;
        tmp += m2;
        return tmp;
    }

    inline Money operator-(const Money& m1, const Money& m2) {
        Money tmp = m1;
        tmp -= m2;
        return tmp;
    }

    inline Money operator*(const Money& m, Decimal x) {
        Money tmp = m;
        tmp *= x;
        return tmp;
    }

    inline Money operator*(Decimal x, const Money& m) {
        return m*x;
    }

    inline Money operator/(const Money& m, Decimal x) {
        Money tmp = m;
        tmp /= x;
        return tmp;
    }

    inline bool operator!=(const Money& m1, const Money& m2) {
        return !(m1 == m2);
    }

    inline bool operator>(const Money& m1, const Money& m2) {
        return m2 < m1;
    }

    inline bool operator>=(const Money& m1, const Money& m2) {
        return m2 <= m1;
    }

    inline Money operator*(Decimal value, const Currency& c) {
        return Money(value,c);
    }

    inline Money operator*(const Currency& c, Decimal value) {
        return Money(value,c);
    }

}


#endif
