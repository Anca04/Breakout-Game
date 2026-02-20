#include "object2DTema1.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* object2DTema1::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2DTema1::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2DTema1::CreateStart(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(0, length, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),

        VertexFormat(corner + glm::vec3(0, length, 0), color),
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color)
    };

    Mesh* start = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5 };

    if (!fill) {
        start->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    start->InitFromData(vertices, indices);
    return start;
}

Mesh* object2DTema1::CreateMotor(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 colorSquare,
    glm::vec3 colorBottom,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, colorSquare),
        VertexFormat(corner + glm::vec3(length, 0, 0), colorSquare),
        VertexFormat(corner + glm::vec3(length, length, 0), colorSquare),
        VertexFormat(corner + glm::vec3(0, length, 0), colorSquare),

        VertexFormat(corner + glm::vec3(0.05f, 0, 0), colorBottom),
        VertexFormat(corner + glm::vec3(length - 0.05f, 0, 0), colorBottom),
        VertexFormat(corner + glm::vec3(0.05f, -(length - 0.05f), 0), colorBottom),

        VertexFormat(corner + glm::vec3(0.05f, 0, 0), colorBottom),
        VertexFormat(corner + glm::vec3(length - 0.05f, 0, 0), colorBottom),
        VertexFormat(corner + glm::vec3(length - 0.05f, -(length - 0.15f), 0), colorBottom),

        VertexFormat(corner + glm::vec3(0.05f, 0, 0), colorBottom),
        VertexFormat(corner + glm::vec3(length - 0.05f, 0, 0), colorBottom),
        VertexFormat(corner + glm::vec3(0.4f * length, -length, 0), colorBottom),

        VertexFormat(corner + glm::vec3(0.05f, 0, 0), colorBottom),
        VertexFormat(corner + glm::vec3(length - 0.05f, 0, 0), colorBottom),
        VertexFormat(corner + glm::vec3(0.7f * length, -length, 0), colorBottom),
    };

    Mesh* motor = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        7, 8, 9,
        10, 11, 12,
        13, 14, 15 };

    if (!fill) {
        motor->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    motor->InitFromData(vertices, indices);
    return motor;
}

Mesh* object2DTema1::CreateTun(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float height,
    float Smallheight,
    float radius,
    glm::vec3 color,
    glm::vec3 colorRectangle,
    glm::vec3 colorTriangle,
    glm::vec3 colorRectangle1,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        // dreptunghi pe verticala
        VertexFormat(corner + glm::vec3(0.1f, 0, 0), colorRectangle),
        VertexFormat(corner + glm::vec3(width, 0, 0), colorRectangle),
        VertexFormat(corner + glm::vec3(width, height, 0), colorRectangle),
        VertexFormat(corner + glm::vec3(0.1f, height, 0), colorRectangle),

        // colt dreapta
        VertexFormat(corner + glm::vec3(width, height, 0), colorTriangle),
        VertexFormat(corner + glm::vec3(width + 0.1f, height, 0), colorTriangle),
        VertexFormat(corner + glm::vec3(width, height - 0.3f, 0), colorTriangle),

        // colt stanga
        VertexFormat(corner + glm::vec3(0, height, 0), colorTriangle),
        VertexFormat(corner + glm::vec3(0.1f, height, 0), colorTriangle),
        VertexFormat(corner + glm::vec3(0.1f, height - 0.3f, 0), colorTriangle),

        // dreptunghi baza
        VertexFormat(corner + glm::vec3(0, 0, 0) , colorRectangle1),
        VertexFormat(corner + glm::vec3(width + 0.1f, 0, 0), colorRectangle1),
        VertexFormat(corner + glm::vec3(width + 0.1f, Smallheight, 0), colorRectangle1),
        VertexFormat(corner + glm::vec3(0, Smallheight, 0), colorRectangle1),

        // cerc
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius, 0, 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(2 * 3.14 / 50), radius * sin(2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(2 * 2 * 3.14 / 50), radius * sin(2 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(3 * 2 * 3.14 / 50), radius * sin(3 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(4 * 2 * 3.14 / 50), radius * sin(4 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(5 * 2 * 3.14 / 50), radius * sin(5 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(6 * 2 * 3.14 / 50), radius * sin(6 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(7 * 2 * 3.14 / 50), radius * sin(7 * 2 * 3.14 / 50), 0), color),

        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(8 * 2 * 3.14 / 50), radius * sin(8 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(9 * 2 * 3.14 / 50), radius * sin(9 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(10 * 2 * 3.14 / 50), radius * sin(10 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(11 * 2 * 3.14 / 50), radius * sin(11 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(12 * 2 * 3.14 / 50), radius * sin(12 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(13 * 2 * 3.14 / 50), radius * sin(13 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(14 * 2 * 3.14 / 50), radius * sin(14 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(15 * 2 * 3.14 / 50), radius * sin(15 * 2 * 3.14 / 50), 0), color),

        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(16 * 2 * 3.14 / 50), radius * sin(16 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(17 * 2 * 3.14 / 50), radius * sin(17 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(18 * 2 * 3.14 / 50), radius * sin(18 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(19 * 2 * 3.14 / 50), radius * sin(19 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(20 * 2 * 3.14 / 50), radius * sin(20 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(21 * 2 * 3.14 / 50), radius * sin(21 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(22 * 2 * 3.14 / 50), radius * sin(22 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(23 * 2 * 3.14 / 50), radius * sin(23 * 2 * 3.14 / 50), 0), color),

        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(24 * 2 * 3.14 / 50), radius * sin(24 * 2 * 3.14 / 50), 0), color),
        VertexFormat(corner + glm::vec3(0.6f, 0.8f, 0) + glm::vec3(radius * cos(25 * 2 * 3.14 / 50), radius * sin(25 * 2 * 3.14 / 50), 0), color),
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = {
        10, 11, 12,
        10, 12, 13,

        14, 15, 16,
        14, 16, 17,
        14, 17, 18,
        14, 18, 19,
        14, 19, 20,
        14, 20, 21,
        14, 21, 22,
        14, 22, 23,
        14, 23, 24,
        14, 24, 25,
        14, 25, 26,
        14, 26, 27,
        14, 27, 28,
        14, 28, 29,
        14, 29, 30,
        14, 30, 31,
        14, 31, 32,
        14, 32, 33,
        14, 33, 34,
        14, 34, 35,
        14, 35, 36,
        14, 36, 37,
        14, 37, 38,
        14, 38, 39,
        14, 39, 40,
        14, 40, 41,

        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        7, 8, 9,
    };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2DTema1::CreateHeart(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float radius,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // centrul cercului stang
    vertices.push_back(VertexFormat(corner, color));

    for (int i = 0; i <= 50; i++) {
        float angle = i * M_PI / 50;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.push_back(VertexFormat(corner + glm::vec3(x, y, 0), color));
    }

    // al doilea cerc
    glm::vec3 corner2 = corner + glm::vec3(radius + radius / 2, 0, 0);
    int offset = vertices.size();

    vertices.push_back(VertexFormat(corner2, color)); // centrul noului cerc
    for (int i = 0; i <= 50; i++) {
        float angle = i * M_PI / 50;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.push_back(VertexFormat(corner2 + glm::vec3(x, y, 0), color));
    }

    glm::vec3 v1 = corner + glm::vec3(-radius, 0, 0);
    glm::vec3 v2 = corner2 + glm::vec3(radius, 0, 0);
    glm::vec3 v3 = glm::vec3((corner.x + corner2.x) / 2.0f, corner.y - 2.5f * radius, 0);

    vertices.push_back(VertexFormat(v1, color));
    vertices.push_back(VertexFormat(v2, color));
    vertices.push_back(VertexFormat(v3, color));

    int offsetTriunghiStanga = vertices.size() - 3;

    Mesh* heart = new Mesh(name);

    for (int i = 1; i <= 50; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    for (int i = 1; i <= 50; i++) {
        indices.push_back(offset); // centrul celui de-al doilea cerc
        indices.push_back(offset + i);
        indices.push_back(offset + i + 1);
    }

    indices.push_back(offsetTriunghiStanga);
    indices.push_back(offsetTriunghiStanga + 1);
    indices.push_back(offsetTriunghiStanga + 2);

    if (!fill) {
        heart->SetDrawMode(GL_LINE_LOOP);
    }

    heart->InitFromData(vertices, indices);
    return heart;
}

Mesh* object2DTema1::CreateBall(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float radius,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;
        
    vertices.push_back(VertexFormat(corner, color));

    for (int i = 0; i <= 50; i++) {
        float angle = 2 * i * M_PI / 50;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.push_back(VertexFormat(corner + glm::vec3(x, y, 0), color));
    }

    Mesh* ball = new Mesh(name);
    std::vector<unsigned int> indices;

    for (int i = 1; i <= 50; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    if (!fill) {
        ball->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    ball->InitFromData(vertices, indices);
    return ball;
}
