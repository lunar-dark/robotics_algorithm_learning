#include <eigen3/Eigen/Dense>
#include <iostream>

void test_det()
{
    Eigen::MatrixXd m;
    m.setRandom(3, 3);
    double det = m.determinant();
    std::cout << "determinant = " << det << "\n";
}

void test_rank()
{
    Eigen::MatrixXd m;
    m.setRandom(3, 3);

    Eigen::FullPivLU<Eigen::MatrixXd> lu(m);
    lu.setThreshold(1e-10);
    std::cout << "Rank of m: " << lu.rank() << "\n";
}

void test_inverse()
{
    Eigen::DiagonalMatrix<double, 3> m1;
    m1.diagonal() << 1, 2, 3;

    Eigen::Matrix3d m2 = m1.toDenseMatrix();
    std::cout << "m2 = \n" << m2 << "\n";

    Eigen::FullPivLU<Eigen::Matrix3d> lu(m2);
    lu.setThreshold(1e-10); // 在实际工程中，矩阵元素可能存在浮点误差，因此判断秩或可逆性时需要设定一个容差阈值。
    if (lu.isInvertible()) // 先验证下矩阵是否可逆
    {
        Eigen::Matrix3d m3 = m2.inverse();
        std::cout << "m3 = \n" << m3 << "\n";
        std::cout << "m2*m3 = \n" << m2 * m3 << "\n";
    }
}

int main()
{
    test_det();
    test_rank();
    test_inverse();
}