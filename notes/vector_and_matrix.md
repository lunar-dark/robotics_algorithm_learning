# Vector And Matrix
## 一、Eigen 核心类型与模板类

| Eigen 类型 | 含义 | 特点 |
|---|---|---|
| `Eigen::Vector2d` / `Vector3i` / `RowVector4f` | 固定长度向量 | 编译期确定大小，栈分配，零开销 |
| `Eigen::VectorXd` | 动态长度向量 | 运行期确定大小，堆分配 |
| `Eigen::Matrix3d` | 固定大小矩阵 (3×3 double) | 编译期确定，性能最优 |
| `Eigen::MatrixXd` | 动态大小矩阵 | 运行期确定，灵活但有分配开销 |
| `Eigen::DiagonalMatrix<double, 3>` | 对角矩阵（紧凑存储） | 只存对角线元素，节省内存 |

---

## 二、向量（Vector）相关知识点

### 1. 向量创建与初始化
```cpp
Eigen::Vector2d v1(1, 2);      // 构造函数初始化
Eigen::Vector3i v2;
v2 << 3, 4, 5;                  // 逗号初始化（Comma Initialization）
Eigen::VectorXd v4;
v4.setRandom(3);                // 随机初始化，动态设置维度
```

### 2. 行向量 vs 列向量
- **Eigen 中向量默认为列向量**（Column Vector），这是为了内存访问的局部性（按列连续存储，无需跳跃访问）
- `transpose()`：将列向量转为行向量，便于输出查看

### 3. 转置的陷阱与正确用法
```cpp
v = v.transpose();              // ❌ Bug：读写别名冲突（Aliasing）
v.transposeInPlace();           // ✅ 原地转置，Eigen 内部优化
v = v.transpose().eval();       // ✅ 强制先求值再赋值，消除别名问题
```

### 4. 向量运算（线性代数概念）

| 运算 | Eigen API | 数学含义 |
|---|---|---|
| 加法 | `v1 + v2` | 向量加法 |
| 数乘 | `3 * v2` | 标量乘法 |
| 模长 | `v1.norm()` | L2 范数 $\|\mathbf{v}\|_2 = \sqrt{\sum v_i^2}$ |
| 模长平方 | `v1.squaredNorm()` | $\|\mathbf{v}\|_2^2$，避免开方运算更高效 |
| 单位化 | `v.normalized()` / `v.normalize()` | $\mathbf{u} = \mathbf{v} / \|\mathbf{v}\|$ |
| 点积 | `v4.dot(v5)` | $\mathbf{a} \cdot \mathbf{b} = \sum a_i b_i$ |
| 叉积 | `v2.cross(v6)` | 仅三维有效，$\mathbf{a} \times \mathbf{b}$ |

---

## 三、矩阵（Matrix）相关知识点

### 1. 矩阵创建与初始化
```cpp
Eigen::Matrix3d m1;
m1 << 1, 2, 3,
      4, 5, 6,
      7, 8, 9;              // 逗号初始化

Eigen::MatrixXd m2;
m2.setRandom(3, 3);          // 动态矩阵随机初始化

Eigen::Matrix3d m3 = Eigen::Matrix3d::Random(3, 3);  // 静态矩阵随机初始化
```

### 2. 矩阵基本运算

| 运算 | Eigen 表达式 | 数学说明 |
|---|---|---|
| 加法 | `m2 + m3` | 对应元素相加 |
| 减法 | `m2 - m3` | 对应元素相减 |
| 乘法 | `m2 * m3` | **矩阵乘法**（非交换律：$AB \neq BA$） |

### 3. 逐元素运算（Array 语义）
Eigen 中矩阵默认是 **线性代数语义**，要进行逐元素运算需转为 `Array`：

```cpp
m1.array() * m1.array()   // 逐元素相乘（Hadamard 积）
m1.array().sqrt()         // 逐元素开平方
m1.array().exp()          // 逐元素指数函数 $e^{x}$
```

> **关键概念**：`Matrix` 语义 vs `Array` 语义
> - `Matrix`：`*` 表示矩阵乘法
> - `Array`：`*` 表示逐元素乘法

---

## 四、特殊矩阵

### 1. 单位矩阵（Identity Matrix）
```cpp
Eigen::Matrix3d::Identity();   // $I_3$，主对角线为 1，其余为 0
```

### 2. 对角矩阵（Diagonal Matrix）
```cpp
Eigen::DiagonalMatrix<double, 3> dm;   // 紧凑存储，只存对角线
dm.diagonal() << 1.0, 2.0, 3.0;        // 设置对角线元素
dm.toDenseMatrix();                     // 转为普通密集矩阵

Eigen::Vector3d v1(1, 2, 3);
v1.asDiagonal();                        // 从向量构造对角矩阵
```

---

## 五、核心线性代数概念总结

| 概念 | 代码体现 |
|---|---|
| **列向量优先存储** | Eigen 向量默认为列向量，符合数学惯例 |
| **别名问题（Aliasing）** | 转置赋值需用 `eval()` 或 `InPlace()` 版本 |
| **矩阵乘法不可交换** | `m2 * m3 != m3 * m2` |
| **逐元素运算 vs 矩阵运算** | 通过 `.array()` 切换语义 |
| **范数与单位化** | `norm()`、`normalized()`、`normalize()` |
| **内积与外积** | `dot()`（点积）、`cross()`（叉积，限 3D） |
| **特殊矩阵构造** | `Identity()`、`DiagonalMatrix`、`asDiagonal()` |

---