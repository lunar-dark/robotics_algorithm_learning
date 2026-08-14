# Eigen

[TOC]

## 1、环境配置

```
sudo apt-get install libeigen3-dev
```

### 1.1、头文件说明

| 头文件                   | 包含内容                   | 用途         | 大小估算 |
| ------------------------ | -------------------------- | ------------ | -------- |
| `<Eigen/Core>`           | 矩阵、数组、基本运算       | 最小依赖     | ~2MB     |
| `<Eigen/Dense>`          | Core + LU, QR, SVD, 特征值 | 稠密矩阵     | ~4MB     |
| `<Eigen/Geometry>`       | 旋转、四元数、变换         | 几何运算     | ~1MB     |
| `<Eigen/Sparse>`         | 稀疏矩阵                   | 大规模计算   | ~2MB     |
| `<Eigen/Eigenvalues>`    | 特征值分解                 | 单独使用     | ~1MB     |
| `<Eigen/QR>`             | QR分解                     | 单独使用     | ~0.5MB   |
| `<Eigen/SVD>`            | SVD分解                    | 单独使用     | ~0.5MB   |
| `<Eigen/LU>`             | LU分解                     | 单独使用     | ~0.5MB   |
| `<Eigen/Cholesky>`       | Cholesky分解               | 正定矩阵     | ~0.3MB   |
| `<Eigen/SparseCholesky>` | 稀疏Cholesky分解           | 稀疏正定矩阵 | ~0.5MB   |

**选择指南**：

- **最小依赖**：仅使用`<Eigen/Core>`
- **通用场景**：使用`<Eigen/Dense>`（包含大部分功能）
- **几何应用**：使用`<Eigen/Geometry>`
- **大规模计算**：使用`<Eigen/Sparse>`
- **单独功能**：按需包含特定模块（如`<Eigen/QR>`）

## 2、Eigen 简介

**Eigen** 是一个高性能的C++模板库，专注于线性代数、矩阵和向量运算。

## 3、矩阵与向量基础

Eigen 中，矩阵表示线性变换、系数表或二维数据；列向量表示坐标、状态、参数或观测值；行向量常用于一行数据、转置结果或统计场景。

### 3.1、矩阵

#### 3.1.1、矩阵类型命名规则

>Matrix\[尺寸\][数据类型] 
>
>尺寸：X = 动态大小, N = 固定大小 N (如 2, 3, 4) 
>
>数据类型：d = double, f = float, i = int, cd = complex\<double\>

#### 3.1.2、矩阵常用类型

| 类型          | 完整定义                           | 说明             |
| ------------- | ---------------------------------- | ---------------- |
| `Matrix3d`    | `Matrix<double, 3, 3>`             | 3×3双精度矩阵    |
| `MatrixXd`    | `Matrix<double, Dynamic, Dynamic>` | 动态双精度矩阵   |
| `Vector3f`    | `Matrix<float, 3, 1>`              | 3维单精度列向量  |
| `RowVector3d` | `Matrix<double, 1, 3>`             | 3维双精度行向量  |
| `VectorXd`    | `Matrix<double, Dynamic, 1>`       | 动态双精度列向量 |
| `RowVectorXd` | `Matrix<double, 1, Dynamic>`       | 动态双精度行向量 |

#### 3.1.3、固定大小矩阵 vs 动态大小矩阵

* 固定大小矩阵：
  * 一般用于小矩阵或尺寸固定；
  * 对象内部是静态数组，直接在栈上分配；
  * 当固定大小矩阵的尺寸是 16 字节的整数倍时，Eigen 申请 SIMD 要求的对齐，利用对齐的 SSE/AVX 指令高效读写。
* 动态大小矩阵：
  * 一般用于大矩阵或尺寸未知；
  * 运行时分配堆内存。
  * 一般不会被优化，但可以在运行期间更改矩阵大小。

#### 3.1.4、特殊矩阵

```c++
Eigen::Matrix3d I = Eigen::Matrix3d::Identity();  // 单位矩阵（方阵，主对角线全部为 1，其余元素为 0）
Eigen::MatrixXd Z = Eigen::MatrixXd::Zero(5, 5);  // 零矩阵（矩阵所有元素全部是 0）
Eigen::MatrixXd R = Eigen::MatrixXd::Random(3, 3);// 随机矩阵
```

对角矩阵：

* 使用 DiagonalMatrix 类
  ```c++
  // 定义一个 3x3 的对角矩阵，对角线元素为 1, 2, 3
  Eigen::DiagonalMatrix<double, 3> D;
  D.diagonal() << 1.0, 2.0, 3.0;

  // 转换为普通矩阵
  Eigen::Matrix3d M = D.toDenseMatrix();
  ```
  DiagonalMatrix 支持动态大小：Eigen::DiagonalMatrix<double, Eigen::Dynamic\>

* 使用 asDiagonal() 从一个向量创建对角矩阵
  asDiagonal() 方法返回一个表达式对象。
    ```c++
    Eigen::Vector3d v(1.0, 2.0, 3.0);

    // 用向量创建对角矩阵表达式
    auto D = v.asDiagonal();
    // 直接参与运算，例如乘以一个向量
    Eigen::Vector3d x(1.0, 1.0, 1.0);
    Eigen::Vector3d result = D * x;  // 等于 (1,2,3)

    std::cout << result.transpose() << std::endl;  // 输出 1 2 3  
    ```
* 提取矩阵的对角线
  如果你有一个普通矩阵，想获取它的对角线元素，可以用 .diagonal()：
  ```c++
  Eigen::Matrix3d M;
  M << 1, 0, 0,
      0, 2, 0,
      0, 0, 3;

  Eigen::Vector3d d = M.diagonal();  // d = (1,2,3)  
  ```
方案对比：
* 优先使用 DiagonalMatrix，内存占用小，运算高效。
* 需要快速从向量生成对角矩阵，使用 vec.asDiagonal()。

#### 3.1.5、矩阵初始化

* 逗号初始化

  ```c++
  Eigen::Matrix3d A;
  A << 1, 2, 3,
       4, 5, 6,
       7, 8, 9;
  ```

* 逐个元素赋值

  ```c++
  Eigen::MatrixXd B(3, 3);
  for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j)
          B(i, j) = i * 3 + j + 1;
  ```

* 借助预定义函数

  ```c++
  Eigen::Matrix3d E = Eigen::Matrix3d::Constant(5); // 全为5
  Eigen::Matrix3d F = Eigen::Matrix3d::Random();    // 随机值
  ```

#### 3.1.6、矩阵运算

`A * B` 是矩阵乘法，`A.array() * B.array()` 是逐元素乘法。涉及 `sqrt`（逐个元素开平方根）、`exp`（逐个元素求指数，即 e^x）、逐元素乘除法时，使用 `Array` 视图。

```c++
Eigen::Matrix3d A, B;
A << 2, -1, 0,
     -1, 2, -1,
     0, -1, 2;                    // 可逆矩阵
B << 9, 8, 7,
     6, 5, 4,
     3, 2, 1;

// 算术运算
Eigen::Matrix3d C = A + B;        // 矩阵加法
Eigen::Matrix3d AB = A * B;       // 矩阵乘法
Eigen::Matrix3d scaled = A * 2.0; // 数乘

// 转置，将行列互换
// v = v.transpose()        ❌ Bug      别名效应导致数据被覆盖
// v.transposeInPlace()	    ✅ 正确      Eigen 内部优化处理
// v = v.transpose().eval()	✅ 正确	  强制先求值再赋值
Eigen::Matrix3d At = A.transpose();

// 矩阵属性
double det = A.determinant();       // 行列式
double tr = A.trace();              // 迹
double norm = A.norm();             // Frobenius范数
Eigen::Matrix3d Ainv = A.inverse(); // 逆矩阵（仅对可逆方阵有意义）

// 逐元素运算（需转换为Array）
Eigen::Matrix3d coeff_product = A.array() * B.array(); // 逐元素乘法


Eigen::Matrix3d P;
P << 1, 4, 9,
     16, 25, 36,
     49, 64, 81;
// 逐元素平方根要求元素非负；这里单独构造一个满足前提的示例矩阵
Eigen::Matrix3d sqrtP = P.array().sqrt();
// 逐元素平求e^x
Eigen::Matrix3d sqrtE = P.array().exp();
```

### 3.2、向量

#### 3.2.1、向量运算

`dot()` 是点积，返回标量；`cross()` 限于三维向量；`normalized()` 返回新向量（不修改原对象），`normalize()` 原地修改。

```c++
Eigen::Vector3d v1(1, 2, 3);
Eigen::Vector3d v2(4, 5, 6);

// 基本运算
double dot = v1.dot(v2);          // 点积
Eigen::Vector3d cross = v1.cross(v2);  // 叉积（仅3D向量）
double norm = v1.norm();          // 模长
double length_sq = v1.squaredNorm();  // 模长平方 3² + 4² + 0² = 25

// 单位向量 
// normalized() 返回新的归一化向量，不修改 v1 本身
Eigen::Vector3d normalized = v1.normalized();

// normalize() 直接修改原向量，把它变成单位向量
v1.normalize();  // 此后 v1 变为单位向量

// 标准单位向量
Eigen::Vector3d i = Eigen::Vector3d::UnitX();  // (1, 0, 0)
Eigen::Vector3d j = Eigen::Vector3d::UnitY();  // (0, 1, 0)
Eigen::Vector3d k = Eigen::Vector3d::UnitZ();  // (0, 0, 1)

// 向量元素运算（此时 v1 仍是原始值 {1, 2, 3}）
v1.sum();                         // 所有元素之和
v1.prod();                        // 所有元素之积
v1.mean();                        // 平均值
v1.minCoeff();                    // 最小值
v1.maxCoeff();                    // 最大值

```

###  3.3、存储顺序

内存是一维连续数组，矩阵是二维，**行 / 列优先决定二维矩阵怎么摊平到一维内存**。

Eigen默认列优先存储。因为 CPU 喜欢**连续内存访问**，遍历列，同一列内从上往下访问，速度快，内存连续。遍历行，同一行左右访问，速度慢，内存跳跃，cache 失效。

```c++
Eigen::Matrix<double, 3, 4, Eigen::ColMajor> A;  // 列优先（默认）
Eigen::Matrix<double, 3, 4, Eigen::RowMajor> B;  // 行优先（与C一致）
```

