#pragma once

#include <iomanip>

// #include "GLM/mat4x4.hpp"
#include "GLM/ext/scalar_constants.hpp"

#include "GLM/vec4.hpp"
#include "GLM/vec3.hpp"

//const float M_PI = 3.1415926535897932384626433832795028841971693993751058209f;
//const auto M_PI = glm::pi<float>();
const auto F_PI = glm::pi<float>();

// namespace cs4722 {
//     void printMat4(glm::mat4 mat) {
//         for (int r = 0; r < 4; r++) {
//             for (int c = 0; c < 4; c++) {
//                 std::cout << std::setw(10) << std::setprecision(3) << mat[c][r] << " ";
//             }
//             std::cout << std::endl;
//         }
//     }
// }

std::ostream& operator<<(std::ostream& output, const glm::vec4& v);
std::ostream& operator<<(std::ostream& output, const glm::vec3& v);

//     output << "[" << v.x << "," << v.y << "," << v.z << "," << v.w << "]" ;
//     return output;
// }

