#define EIGEN_USE_BLAS
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <cmath>
#include <chrono>

Eigen::Vector2f trilaterate(Eigen::RowVector2f tower1, Eigen::RowVector2f tower2, Eigen::RowVector2f tower3, float r1, float r2, float r3);

int main() {
    
    // Speed of sound in meters per second
    const int speedOfSound = 343;

    //tower positions
    Eigen::RowVector2f tower1(0.0, 0.0);
    Eigen::RowVector2f tower2(1.0, 0.0);
    Eigen::RowVector2f tower3(0.0, 1.0);

    Eigen::RowVector2f soundSource(0.75, 1.0);

    float distanceToTower1 = (soundSource - tower1).norm();
    float distanceToTower2 = (soundSource - tower2).norm();
    float distanceToTower3 = (soundSource - tower3).norm();

    std::cout << "Distance to Tower 1: " << distanceToTower1 << " meters" << std::endl;
    std::cout << "Distance to Tower 1: " << distanceToTower2 << " meters" << std::endl;
    std::cout << "Distance to Tower 1: " << distanceToTower3 << " meters" << std::endl << std::endl;

    float arrivalTime_Tower1 = 0.003644;
    float arrivalTime_Tower2 = 0.003005;
    float arrivalTime_Tower3 = 0.002187;

    std::cout << "Arrival time at Tower 1: " << arrivalTime_Tower1 << " seconds" << std::endl;
    std::cout << "Arrival time at Tower 2: " << arrivalTime_Tower2 << " seconds" << std::endl;
    std::cout << "Arrival time at Tower 3: " << arrivalTime_Tower3 << " seconds" << std::endl << std::endl;

    float r1 = arrivalTime_Tower1 * speedOfSound;
    float r2 = arrivalTime_Tower2 * speedOfSound;
    float r3 = arrivalTime_Tower3 * speedOfSound;

    auto start = std::chrono::high_resolution_clock::now();

    Eigen::Vector2f estPos = trilaterate(tower1, tower2, tower3, r1, r2, r3);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);


    std::cout << "Estimated Position of sound source :" << estPos.transpose() << " meters" << std::endl;
    std::cout << "Time taken for trilaterate function: " << duration.count() << " microseconds" << std::endl;

}

Eigen::Vector2f trilaterate(Eigen::RowVector2f tower1, Eigen::RowVector2f tower2, Eigen::RowVector2f tower3, float r1, float r2, float r3)
{

    Eigen::RowVector2f A = 2 * (tower2 - tower1);
    Eigen::RowVector2f B = 2 * (tower3 - tower1);

    float C = (float)std::pow(r1, 2.0) - (float)std::pow(r2, 2.0)- tower1.dot(tower1) + tower2.dot(tower2);
    float D = std::pow(r1, 2.0) - std::pow(r3, 2.0) - tower1.dot(tower1) + tower3.dot(tower3);

    // Solving the sys of eqs
    Eigen::Matrix2f A_mat;
    A_mat << A(0), A(1),
             B(0), B(1);

    Eigen::Vector2f b_vec(C, D);

    Eigen::Vector2f x = A_mat.colPivHouseholderQr().solve(b_vec);

    return x;

}