#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        gfxc::TextRenderer* textRenderer = nullptr;

    void Init() override;

    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {
            }
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {
            }
            float x;
            float y;
            float width;
            float height;
        };

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void DrawScene(glm::mat3 visMatrix);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnWindowResize(int width, int height) override;
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
        glm::vec2 ConvertScreenToLogic(int mouseX, int mouseY);
        bool IsShipConnected();
        bool CheckMotorConstraint();
        bool CheckTunConstraint();
        void StartGame();
        void DrawSceneGame(glm::mat3 visMatrix);
        bool CheckAABB(glm::vec2 ballPos, float radius, glm::vec2 boxPos, glm::vec2 boxSize, glm::vec2& collisionNormal);
        void GameOver();
        void DrawSceneGameOver(glm::mat3 visMatrix);
        void UpdateLayout();

    protected:
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;
        float length;

        float widthWin;
        float heightWin;

        enum BlockType {
            EMPTY,
            SOLID,
            ENGINE,
            TUN,
            OCCUPIED,
            SOLID2,
        };

        BlockType grid[9][17];

		bool isMoving = false;
        BlockType currentBlock;
        BlockType underBlock;
        glm::vec2 mousePosition;
        
		float heightLeftGrid;
		float widthLeftGrid;

        float gridStartX;
        float gridStartY;
        int gridWidth;
        int gridHeight;
        float cellSize;

        int totalBlocks;
        int totalPieseOcupa;
        bool constraintsMet;

        glm::vec3 startColorRed;
        glm::vec3 startColorGreen;

        bool startColor;
        bool beforeStart;

        float shipGameX;
        float shipGameY;

        int score;
        int lives;

        glm::vec2 ballPos;   
        glm::vec2 ballSpeedVect; 
        float ballRadius; 
        bool ballOnPaddle; 
        float ballSpeed; 

        float heightPaddle;
        float widthPaddle;
        bool gameOver;

        struct Brick {
            glm::vec2 pos;
            glm::vec2 size;
            int broke;
            int hitsLeft;
            glm::vec3 color;
        };

        std::vector<Brick> bricks;
        int numCaramizi;

        std::vector<glm::vec3> culori = {
            glm::vec3(1, 0, 0),  // rosu
            glm::vec3(0, 0, 1),  // albastru
            glm::vec3(0, 1, 0),  // verde
            glm::vec3(0.5f, 0, 0.5f),   // mov
            glm::vec3(1, 0.5f, 0),  // portocaliu
            glm::vec3(1, 0, 1), // roz
            glm::vec3(1, 1, 1) // alb
        };

        struct Particle {
            glm::vec2 pos;       
            glm::vec2 speed;  
            glm::vec3 color;     
            float life;          
        };

        std::vector<Particle> particles;

        bool isShaking = false;
        float shakeDuration = 0.2f; 
        float shakeTime = 0.0f;
        float shakeIntensity = 0.3f;
        glm::vec2 shakeOffset = glm::vec2(0, 0);

        struct ShipPart {
            BlockType type;
            glm::vec2 pos;
        };
        std::vector<ShipPart> shipParts;

        bool reset;
    };
}   // namespace m1
