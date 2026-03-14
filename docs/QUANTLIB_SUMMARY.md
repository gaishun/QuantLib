# QuantLib 量化金融库核心功能总结

## 概述

QuantLib是一个免费/开源的C++量化金融库，用于期权定价、风险管理、利率建模等。

---

## 一、核心模块

### 1. 期权 (Options)

**期权类型:**
- `VanillaOption`: 普通股票/指数期权
- `BarrierOption`: 障碍期权(敲入/敲出)
- `AsianOption`: 亚式期权(平均价格)
- `LookbackOption`: 回望期权
- `DigitalOption`: 数字期权

**行权方式 (Exercise):**
- `EuropeanExercise`: 欧式(到期日行权)
- `AmericanExercise`: 美式(到期前任何时间)
- `BermudanExercise`: 百慕大(特定日期行权)

**收益函数 (Payoff):**
```cpp
// 看涨期权: max(S-K, 0)
PlainVanillaPayoff(Option::Call, strike)

// 看跌期权: max(K-S, 0)  
PlainVanillaPayoff(Option::Put, strike)

// 二元期权
CashOrNothingPayoff(strike, cashAmount)
AssetOrNothingPayoff(strike)
```

### 2. 期限结构 (Term Structures)

**收益率曲线:**
```cpp
// 平直曲线
FlatForward(referenceDate, rate, dc)

// 零息曲线
ZeroCurve(dates, rates, dc)

// 插值曲线
InterpolatedYieldCurve(dates, rates, dc, linearInterpolation)
```

**波动率结构:**
```cpp
// Black波动率曲面
BlackVarianceSurface(dates, strikes, volMatrix, dc)

// 局部波动率
LocalVolSurface(blackSurface, localVol)

// 随机波动率
HestonModel(v0, kappa, theta, sigma, rho)
```

### 3. 定价引擎 (Pricing Engines)

**解析引擎:**
```cpp
AnalyticEuropeanEngine // Black-Scholes解析解
BinomialTreeEngine    // 二叉树
FiniteDifferenceEngine // 有限差分
MonteCarloEngine      // 蒙特卡洛
```

### 4. 随机过程 (Stochastic Processes)

**Black-Scholes-Merton过程:**
```cpp
dS = (r - q)S*dt + σ*S*dW

// 参数:
- S: 标的资产价格
- r: 无风险利率
- q: 股息收益率
- σ: 波动率
```

---

## 二、核心类说明

### Handle (句柄/智能指针)
- 可重链接的引用
- 自动追踪依赖关系
- Handle重链接时自动通知相关对象更新

### Observable (观察者模式)
- 参数变化时自动通知
- 用于市场数据更新
- 支持延迟计算

### Settings (全局设置)
- 评估日期控制
- 月末惯例
- 全局参数配置

---

## 三、数值方法

### 1. 蒙特卡洛模拟
- 路径生成
- 布朗桥
- 伪随机数(Mersenne Twister)
- 拟随机数(Sobol)
- 方差缩减(Antithetic, Control Variates)

### 2. 树方法
- 二叉树
- 三叉树
- Monte-Carlo Longstaff-Schwartz

### 3. 有限差分
- 显示方法
- 隐式方法
- ADI方法(多因子)

---

## 四、Greeks计算

| 符号 | 名称 | 定义 |
|------|------|------|
| Δ (Delta) | Delta | ∂V/∂S 标的价格变化1单位 |
| Γ (Gamma) | Gamma | ∂²V/∂S² Delta的变化率 |
| ν (Vega) | Vega | ∂V/∂σ 波动率变化1% |
| Θ (Theta) | Theta | ∂V/∂t 时间衰减(每天) |
| ρ (Rho) | Rho | ∂V/∂r 利率变化1% |

---

## 五、使用示例

### 期权定价完整示例:
```cpp
// 1. 创建市场数据
Handle<Quote> spot = ...;
Handle<YieldTermStructure> r = ...;
Handle<BlackVolTermStructure> vol = ...;

// 2. 创建随机过程
ext::shared_ptr<BlackScholesMertonProcess> process = 
    ext::make_shared<BlackScholesMertonProcess>(spot, q, r, vol);

// 3. 创建期权
ext::shared_ptr<StrikedTypePayoff> payoff = 
    ext::make_shared<PlainVanillaPayoff>(Option::Call, 100);
ext::shared_ptr<Exercise> exercise = 
    ext::make_shared<EuropeanExercise>(Date(15, June, 2025));
VanillaOption option(payoff, exercise);

// 4. 设置定价引擎
option.setPricingEngine(ext::make_shared<AnalyticEuropeanEngine>(process));

// 5. 获取结果
Real npv = option.NPV();
Real delta = option.delta();
Real gamma = option.gamma();
Real vega = option.vega();
Real theta = option.theta();
Real rho = option.rho();
```

---

## 六、编译和安装

```bash
# 使用CMake
mkdir build
cd build
cmake ..
make
make install

# 或使用Boost.Build
b2 install
```

---

## 七、相关资料

- 官网: https://www.quantlib.org
- 文档: https://www.quantlib.org/reference/
- 书籍: "Quantitative Finance" by Ballabio
