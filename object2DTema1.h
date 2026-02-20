#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2DTema1
{
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float width, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateStart(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateMotor(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 colorSquare, glm::vec3 colorBottom, bool fill = false);
    Mesh* CreateTun(const std::string& name, glm::vec3 leftBottomCorner, float width, float length,
        float Smallheight, float radius, glm::vec3 color, glm::vec3 colorRectangle, glm::vec3 colorTriangle, glm::vec3 colorRectangle1, bool fill = false);
    Mesh* CreateHeart(const std::string& name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill);
    Mesh* CreateBall(const std::string& name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill);
}
