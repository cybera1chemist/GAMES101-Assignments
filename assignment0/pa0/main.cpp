#include<cmath>
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Dense>
#include<iostream>

using namespace std;
using namespace Eigen;

void problem3(){
    // Define a point P
    Vector3f P(2.0, 1.0, 1.0);
    cout << "Point P: " << P.transpose() << endl;

    // Rotate it 45 degree around the origin
    float angle = 45.0 / 180.0 * acos(-1); // acos(-1) means pi!!!
    Matrix3f R;
    R << cos(angle), -sin(angle), 0,
         sin(angle),  cos(angle), 0,
         0,           0,          1;
    Vector3f P_rotated = R * P;
    cout << "After rotation, P: " << P_rotated.transpose() << endl;

    // Translate it by (1,2)
    Matrix3f T;
    T << 1, 0, 1,
         0, 1, 2,
         0, 0, 1;
    Vector3f P_translated = T * P_rotated;
    cout << "After translation, P: " << P_translated.transpose() << endl;
}

void examples(){

    // Basic Example of cpp
    std::cout << "Example of cpp \n";
    float a = 1.0, b = 2.0;
    std::cout << a << std::endl;
    std::cout << a/b << std::endl;
    std::cout << std::sqrt(b) << std::endl;
    std::cout << std::acos(-1) << std::endl;
    std::cout << std::sin(30.0/180.0*acos(-1)) << std::endl;

    // Example of vector
    std::cout << "Example of vector \n";
    // vector definition
    Eigen::Vector3f v(1.0f,2.0f,3.0f);
    Eigen::Vector3f w(1.0f,0.0f,0.0f);
    // vector output
    std::cout << "Example of output \n";
    std::cout << v << std::endl;
    // vector add
    std::cout << "Example of add \n";
    std::cout << v + w << std::endl;
    // vector scalar multiply
    std::cout << "Example of scalar multiply \n";
    std::cout << v * 3.0f << std::endl;
    std::cout << 2.0f * v << std::endl;

    // Example of matrix
    std::cout << "Example of matrix \n";
    // matrix definition
    Eigen::Matrix3f i,j;
    i << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
    j << 2.0, 3.0, 1.0, 4.0, 6.0, 5.0, 9.0, 7.0, 8.0;
    // matrix output
    std::cout << "Example of output \n";
    std::cout << i << std::endl;
    // matrix add i + j
    std::cout<< "i + j: " << i+j << std::endl;
    // matrix scalar multiply i * 2.0
    cout << "i * 2.0: " << i * 2.0 << endl;
    // matrix multiply i * j
    cout << "i * j: " << i * j << endl;
    // matrix multiply vector i * v
    cout << "i * v: " << i * v << endl;
}

int main(){
    // examples();
    problem3();
    return 0;
}