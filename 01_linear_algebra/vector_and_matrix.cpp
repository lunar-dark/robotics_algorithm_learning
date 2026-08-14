#include <eigen3/Eigen/Dense>
#include <iostream>

void test_vector()
{
    // 向量
    std::cout << "\t\tTest Vector\n";
    // 固定长度向量
    
    // Eigen 中创建的向量默认为列向量，这考虑到其在内存中存储状态为数组，按照列访问可以不用跳跃方式访问内存
    Eigen::Vector2d v1(1, 2);
    std::cout << "v1 = " << v1.transpose() << std::endl; // 转置，将向量行列互换，列向量 to 行向量，方便终端查看

    // v = v.transpose()        ❌ Bug      一边读取一边写入，导致后续要读取的数据已经被写入覆盖了
    // v.transposeInPlace()	    ✅ 正确     Eigen 内部优化处理
    // v = v.transpose().eval()	✅ 正确	   强制先求值再赋值

    Eigen::Vector3i v2;
    v2 << 3, 4, 5;
    std::cout << "v2 = " << v2.transpose() << std::endl;

    Eigen::RowVector4f v3(6, 7, 8, 9);
    std::cout << "v3 = " << v3 << std::endl;

    // 动态长度向量
    Eigen::VectorXd v4;
    v4.setRandom(3);
    std::cout << "v4 = " << v4.transpose() << std::endl;

    v4 = Eigen::Vector4d(6, 6, 6, 6); // 可以在运行期间更改维度
    std::cout << "v4 = " << v4.transpose() << std::endl;

    // 向量加法
    Eigen::Vector2d v_add = v1 + Eigen::Vector2d(3, 4);
    std::cout << "v_add = " << v_add.transpose() << std::endl;

    // 向量数乘
    Eigen::Vector3i v_mul = 3 * v2;
    std::cout << "v_mul = " << v_mul.transpose() << std::endl;

    // 模长
    double length = v1.norm();
    double length_sq = v1.squaredNorm();
    std::cout << "v1_length = " << length << "\tv1_length_sq = " << length_sq << std::endl;

    // 单位向量
    Eigen::Vector3i v2_unit = v2.normalized();
    Eigen::Vector4d v_tmp = v4;
    v_tmp.normalize();
    std::cout << "v2_unit = " << v2_unit.transpose() << "\tv4_unit = " << v_tmp.transpose() << std::endl;

    // 点积
    Eigen::Vector4d v5(1, 2, 3, 4);
    double v4v5_dot = v4.dot(v5);
    std::cout << "v4v5_dot = " << v4v5_dot << std::endl;

    // 叉积（在三维下有效）
    Eigen::Vector3i v6(1, 2, 3);
    Eigen::Vector3i v2v6_cross = v2.cross(v6);
    std::cout << "v2v6_cross = " << v2v6_cross.transpose() << std::endl;
}

void test_matrix()
{
    std::cout << "\t\tTest Matrix\n";

    // 固定大小矩阵矩阵（大小编译时期确定，栈上分配，无动态分配开销，可高度优化）
    Eigen::Matrix3d m1;
    m1 << 1, 2, 3,
          4, 5, 6,
          7, 8, 9;
    std::cout << "m1 = \n" << m1 << std::endl;

    // 动态大小矩阵（大小运行时期确定，堆上动态分配，有动态分配开销）
    Eigen::MatrixXd m2;
    m2.setRandom(3, 3);
    std::cout << "m2 = \n" << m2 << std::endl;

    Eigen::Matrix3d m3 = Eigen::Matrix3d::Random(3, 3);
    std::cout << "m3 = \n" << m3 << std::endl;

    // 矩阵加减法
    Eigen::Matrix3d m2m3_add = m2 + m3;
    std::cout << "m2m3_add = \n" << m2m3_add << std::endl;

    Eigen::Matrix3d m2m3_sub = m2 - m3;
    std::cout << "m2m3_sub = \n" << m2m3_sub << std::endl;

    // 矩阵乘法，不具备交换律
    Eigen::Matrix3d m2m3_mul = m2 * m3;
    std::cout << "m2m3_mul = \n" << m2m3_mul << std::endl;

    Eigen::Matrix3d m3m2_mul = m3 * m2;
    std::cout << "m3m2_mul = \n" << m3m2_mul << std::endl;

    // 矩阵元素相乘
    Eigen::Matrix3d m1_array_mul = m1.array() * m1.array();
    std::cout << "m1_array_mul = \n" << m1_array_mul << std::endl;

    // 逐元素开平方
    Eigen::Matrix3d m1_array_sqrt = m1.array().sqrt();
    std::cout << "m1_array_sqrt = \n" << m1_array_sqrt << std::endl;

    // 逐元素求 e^x
    Eigen::Matrix3d m1_array_exp = m1.array().exp();
    std::cout << "m1_array_exp = \n" << m1_array_exp << std::endl;

    // 单位矩阵
    Eigen::Matrix3d m4_unit = Eigen::Matrix3d::Identity();
    std::cout << "m4_unit = \n" << m4_unit << std::endl;

    // 对角矩阵
    Eigen::DiagonalMatrix<double,3> dm;
    dm.diagonal() << 1.0, 2.0, 3.0;
    std::cout << "diagonal = " << dm.diagonal().transpose() << std::endl; // 查看对角线
    std::cout << "dense = \n" << dm.toDenseMatrix() << std::endl; // 查看对角线

    Eigen::Vector3d v1(1, 2, 3);
    Eigen::Matrix3d m5_diagonal = v1.asDiagonal(); // 从向量构建单位矩阵
    std::cout << "m5_diagonal = \n" << m5_diagonal << std::endl;
}

int main()
{
    test_vector();
    test_matrix();
    return 0;
}