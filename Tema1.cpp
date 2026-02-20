#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/Tema1/object2DTema1.h"
#include "lab_m1/Tema1/transform2D.h"

#include <vector>
#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>

using namespace std;
using namespace m1;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init() {
    // seteaza camera
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // pentru a genera random culorile la caramizi
    srand(static_cast<unsigned int>(time(NULL)));

    // obtin dimensiunea ferestrei
    glm::ivec2 resolution = window->GetResolution();
    // definesc zona de desenare
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    visMatrix = VisualizationTransf2D(logicSpace, viewSpace);

    // setez dimensiunile
    logicSpace.x = 0.0f;
    logicSpace.y = 0.0f;
    logicSpace.width = 35.0f;
    logicSpace.height = 19.7f;

    // initializare text
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, window->props.resolution.x, window->props.resolution.y);
    textRenderer->Load("assets/fonts/Hack-Bold.ttf", 48);

    // pentru pozitii
    mousePosition = glm::vec2(0, 0);
    glm::vec3 cornerGrila = glm::vec3(0, 0, 0);

    // initializare alte variabile
    isMoving = false;
    currentBlock = EMPTY;
    underBlock = EMPTY;
    totalBlocks = 0;
    totalPieseOcupa = 0;
    constraintsMet = false;
    beforeStart = true;
    score = 0;
    lives = 3;
    numCaramizi = 60;
    reset = false;
    gameOver = false;

    // dimensiuni si coordonate pentru gridul unde se construieste nava spatiala
    widthLeftGrid = 22.0f;
    heightLeftGrid = 12.4f;
    gridWidth = 17;
    gridHeight = 9;
    cellSize = 1.2f; // dimensiune piese din meniul din stanga

    // coordonate de unde sa inceapa patratelele albastre
    gridStartX = 13;
    gridStartY = 3;

    // initializare grid
    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) {
            grid[i][j] = EMPTY;
        }
    }

    // mesh pentru contur grid albastru
    Mesh* rectangle = object2DTema1::CreateRectangle("rectangle", cornerGrila, widthLeftGrid, heightLeftGrid, glm::vec3(0, 0, 1), false);
    AddMeshToList(rectangle);

    // mesh pentru contur tot ecranul
    Mesh* contur = object2DTema1::CreateRectangle("contur", cornerGrila, logicSpace.width - 0.1f, logicSpace.height - 0.1f, glm::vec3(1, 0, 0), false);
    AddMeshToList(contur);

    // pentru patratele albastre ale gridului din centru
    glm::vec3 cornerSquare = glm::vec3(0, 0, 0);
    length = 0.9f;

    Mesh* square1 = object2DTema1::CreateSquare("square1", cornerSquare, length, glm::vec3(0, 0, 1), true);
    AddMeshToList(square1);

    // mesh separat pentru patratul din mijloc pentru a fi de o culoare mai inchisa
    Mesh* squareCentru = object2DTema1::CreateSquare("squareCentru", cornerSquare, length, glm::vec3(0, 0, 0.5f), true);
    AddMeshToList(squareCentru);

    // patrat verde pentru numarul pieselor
    length = 1.2f;
    Mesh* square2 = object2DTema1::CreateSquare("square2", cornerSquare, length, glm::vec3(0, 1, 0), true);
    AddMeshToList(square2);

    // butonul start de culoare rosie cand nu sunt respectate conditiile
    length = 1.2f;
    startColorRed = glm::vec3(1, 0, 0);
    Mesh* start1 = object2DTema1::CreateStart("start1", cornerSquare, length, startColorRed, true);
    AddMeshToList(start1);

    // butonul start de culoare verde cand sunt respectate conditiile
    startColorGreen = glm::vec3(0, 1, 0);
    Mesh* start2 = object2DTema1::CreateStart("start2", cornerSquare, length, startColorGreen, true);
    AddMeshToList(start2);

    // definire mesh pentru un dreptunghi de contur rosu pentru meniul din stanga
    float width = 10;
    float height = logicSpace.height / 4;
    Mesh* rectangle2 = object2DTema1::CreateRectangle("rectangle2", cornerGrila, width, height, glm::vec3(1, 0, 0), false);
    AddMeshToList(rectangle2);

    // primul bloc solid din meniul din stanga
    length = 1.2f;
    Mesh* square3 = object2DTema1::CreateSquare("square3", cornerSquare, length, glm::vec3(0.8f, 0.8f, 0.8f), true);
    AddMeshToList(square3);

    // motor in meniul din stanga
    Mesh* motor = object2DTema1::CreateMotor("motor", cornerSquare, length, glm::vec3(1.0f, 0.6f, 0.0f), glm::vec3(1.0f, 0.4f, 0.0f), true);
    AddMeshToList(motor);

    // tun in meniul din stanga
    width = 1.1f;
    height = 3.5f;
    float smallHeight = 0.8f;
    float radius = 0.6f;
    Mesh* tun = object2DTema1::CreateTun("tun", cornerSquare, width, height, smallHeight, radius, glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.6f, 0.6f, 0.6f), true);
    AddMeshToList(tun);

    // al doilea bloc solid din meniul din stanga
    length = 1.2f;
    Mesh* square4 = object2DTema1::CreateSquare("square4", cornerSquare, length, glm::vec3(1, 0, 1), true);
    AddMeshToList(square4);

    // inimi pentru a reprezenta vietile din joc
    radius = 0.2f;
    Mesh* heart = object2DTema1::CreateHeart("heart", cornerSquare, radius, glm::vec3(1, 0, 0), true);
    AddMeshToList(heart);

    // initializare variabile pentru minge: pozitie, dimensiune, viteza, daca
    // se afla pe paleta
    shipGameX = 0.0f;
    shipGameY = 0.0f;
    ballRadius = 0.5f;
    heightPaddle = 1.2f;
    ballPos = glm::vec2(shipGameX, shipGameY + heightPaddle + ballRadius + 0.01f);
    ballSpeedVect = glm::vec2(0, 0);
    ballOnPaddle = true;
    ballSpeed = 5.0f;
    widthPaddle = 0.0f;

    // mesh pentru minge
    Mesh* ball = object2DTema1::CreateBall("ball", cornerSquare, ballRadius, glm::vec3(1, 0, 1), true);
    AddMeshToList(ball);

    // setare dim pentru caramizi si patrate pt animatie
    width = logicSpace.width / 12;
    height = logicSpace.height / 2 / 6;
    length = 0.3f;

    // contur caramizi
    Mesh* caramidaContur = object2DTema1::CreateRectangle("caramidaContur", cornerGrila, width, height, glm::vec3(0, 0, 0), false);
    AddMeshToList(caramidaContur);

    // generare random caramizi
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 12; i++) {
            Brick b;
            b.pos = glm::vec2(i * width, (logicSpace.height - 1) - (j + 1) * height);
            b.size = glm::vec2(width, height);
            b.broke = 0;
            b.hitsLeft = 1;

            // genereaza o culoare random
            int k = rand() % 6;

            switch (k) {
            case 0: {
                b.color = culori[0];
                b.hitsLeft = 1;
                Mesh* caramidaRosie = object2DTema1::CreateRectangle("caramidaRosie", cornerGrila, width, height, glm::vec3(1, 0, 0), true);
                AddMeshToList(caramidaRosie);
                Mesh* sqAnimatieRosu = object2DTema1::CreateSquare("sqAnimatieRosu", cornerGrila, length, culori[0], true);
                AddMeshToList(sqAnimatieRosu);
                break;
            }
            case 1: {
                b.color = culori[1];
                b.hitsLeft = 2;
                Mesh* caramidaAlbastru = object2DTema1::CreateRectangle("caramidaAlbastru", cornerGrila, width, height, b.color, true);
                AddMeshToList(caramidaAlbastru);
                Mesh* sqAnimatieAlbastru = object2DTema1::CreateSquare("sqAnimatieAlbastru", cornerGrila, length, culori[1], true);
                AddMeshToList(sqAnimatieAlbastru);
                break;
            }
            case 2: {
                b.color = culori[2];
                b.hitsLeft = 3;
                Mesh* caramidaVerde = object2DTema1::CreateRectangle("caramidaVerde", cornerGrila, width, height, b.color, true);
                AddMeshToList(caramidaVerde);
                Mesh* sqAnimatieVerde = object2DTema1::CreateSquare("sqAnimatieVerde", cornerGrila, length, culori[2], true);
                AddMeshToList(sqAnimatieVerde);
                break;
            }
            case 3: {
                b.color = culori[3];
                b.hitsLeft = 4;
                Mesh* caramidaMov = object2DTema1::CreateRectangle("caramidaMov", cornerGrila, width, height, b.color, true);
                AddMeshToList(caramidaMov);
                Mesh* sqAnimatieMov = object2DTema1::CreateSquare("sqAnimatieMov", cornerGrila, length, culori[3], true);
                AddMeshToList(sqAnimatieMov);
                break;
            }
            case 4: {
                b.color = culori[4];
                b.hitsLeft = 1;
                Mesh* caramidaPortocaliu = object2DTema1::CreateRectangle("caramidaPortocaliu", cornerGrila, width, height, b.color, true);
                AddMeshToList(caramidaPortocaliu);
                Mesh* sqAnimatiePortocaliu = object2DTema1::CreateSquare("sqAnimatiePortocaliu", cornerGrila, length, culori[4], true);
                AddMeshToList(sqAnimatiePortocaliu);
                break;
            }
            case 5: {
                b.color = culori[5];
                b.hitsLeft = 5;
                Mesh* caramidaRoz = object2DTema1::CreateRectangle("caramidaRoz", cornerGrila, width, height, b.color, true);
                AddMeshToList(caramidaRoz);
                Mesh* sqAnimatieRoz = object2DTema1::CreateSquare("sqAnimatieRoz", cornerGrila, length, culori[5], true);
                AddMeshToList(sqAnimatieRoz);
                break;
            }
            }
            bricks.push_back(b);
        }
    }

    // patrat pentru animatie
    length = 0.3f;
    Mesh* sqAnimatieAlb = object2DTema1::CreateSquare("sqAnimatieAlb", cornerGrila, length, glm::vec3(1, 1, 1), true);
    AddMeshToList(sqAnimatieAlb);
}

void Tema1::UpdateLayout() {
    glm::ivec2 resolution = window->GetResolution();
    widthWin = (float)resolution.x;
    heightWin = (float)resolution.y;

    viewSpace = ViewportSpace(0, 0, (int)widthWin, (int)heightWin);
    glViewport(0, 0, widthWin, heightWin);

    logicSpace.width = 35;
    logicSpace.height = logicSpace.width * (heightWin / widthWin);
    visMatrix = VisualizationTransf2D(logicSpace, viewSpace);

    // dimensiuni si coordonate pentru gridul unde se construieste nava spatiala
    widthLeftGrid = 22.0f;
    heightLeftGrid = 12.4f;

    gridStartX = 13.0f;
    gridStartY = 3.0f;

    cellSize = 1.2f;

    mousePosition = glm::vec2(0, 0);
    glm::vec3 cornerGrila = glm::vec3(0, 0, 0);

    gridWidth = 17;
    gridHeight = 9;

    // initializare grid
    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) {
            grid[i][j] = EMPTY;
        }
    }

    // mesh pentru gridul alabstru
    Mesh* rectangle = object2DTema1::CreateRectangle("rectangle", cornerGrila, widthLeftGrid, heightLeftGrid, glm::vec3(0, 0, 1), false);
    AddMeshToList(rectangle);

    // mesh pentru contur tot ecranul
    Mesh* contur = object2DTema1::CreateRectangle("contur", cornerGrila, logicSpace.width - 0.1f, logicSpace.height - 0.1f, glm::vec3(1, 0, 0), false);
    AddMeshToList(contur);

    // pentru patratele albastre ale gridului din centru
    glm::vec3 cornerSquare = glm::vec3(0, 0, 0);
    length = 0.9f;

    Mesh* square1 = object2DTema1::CreateSquare("square1", cornerSquare, length, glm::vec3(0, 0, 1), true);
    AddMeshToList(square1);

    // mesh separat pentru patratul din mijloc pentru a fi de o culoare mai inchisa
    Mesh* squareCentru = object2DTema1::CreateSquare("squareCentru", cornerSquare, length, glm::vec3(0, 0, 0.5f), true);
    AddMeshToList(squareCentru);

    // patrat verde pentru numarul pieselor
    length = 1.2f;
    Mesh* square2 = object2DTema1::CreateSquare("square2", cornerSquare, length, glm::vec3(0, 1, 0), true);
    AddMeshToList(square2);

    // butonul start de culoare rosie cand nu sunt respectate conditiile
    length = 1.2f;
    startColorRed = glm::vec3(1, 0, 0);
    Mesh* start1 = object2DTema1::CreateStart("start1", cornerSquare, length, startColorRed, true);
    AddMeshToList(start1);

    // butonul start de culoare verde cand sunt respectate conditiile
    startColorGreen = glm::vec3(0, 1, 0);
    Mesh* start2 = object2DTema1::CreateStart("start2", cornerSquare, length, startColorGreen, true);
    AddMeshToList(start2);

    // definire mesh pentru un dreptunghi de contur rosu pentru meniul din stanga
    float width = 10;
    float height = logicSpace.height / 4;
    Mesh* rectangle2 = object2DTema1::CreateRectangle("rectangle2", cornerGrila, width, height, glm::vec3(1, 0, 0), false);
    AddMeshToList(rectangle2);

    // primul bloc solid din meniul din stanga
    length = 1.2f;
    Mesh* square3 = object2DTema1::CreateSquare("square3", cornerSquare, length, glm::vec3(0.8f, 0.8f, 0.8f), true);
    AddMeshToList(square3);

    // motor in meniul din stanga
    Mesh* motor = object2DTema1::CreateMotor("motor", cornerSquare, length, glm::vec3(1.0f, 0.6f, 0.0f), glm::vec3(1.0f, 0.4f, 0.0f), true);
    AddMeshToList(motor);

    // tun in meniul din stanga
    width = 1.1f;
    height = 3.5f;
    float smallHeight = 0.9f;
    float radius = 0.55f;
    Mesh* tun = object2DTema1::CreateTun("tun", cornerSquare, width, height, smallHeight, radius, glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.6f, 0.6f, 0.6f), true);
    AddMeshToList(tun);

    // al doilea bloc solid din meniul din stanga
    length = 1.2f;
    Mesh* square4 = object2DTema1::CreateSquare("square4", cornerSquare, length, glm::vec3(1, 0, 1), true);
    AddMeshToList(square4);

    // inimi pentru a reprezenta vietile din joc
    radius = 0.2f;
    Mesh* heart = object2DTema1::CreateHeart("heart", cornerSquare, radius, glm::vec3(1, 0, 0), true);
    AddMeshToList(heart);

    // initializare variabile pentru minge: pozitie, dimensiune, viteza, daca
    // se afla pe paleta
    shipGameX = 0.0f;
    shipGameY = 0.0f;
    ballRadius = 0.5f;
    heightPaddle = 1.2f;
    ballPos = glm::vec2(shipGameX, shipGameY + heightPaddle + ballRadius + 0.01f);
    ballSpeedVect = glm::vec2(0, 0);
    ballOnPaddle = true;
    ballSpeed = 5.0f;
    widthPaddle = 0.0f;

    Mesh* ball = object2DTema1::CreateBall("ball", cornerSquare, ballRadius, glm::vec3(1, 0, 1), true);
    AddMeshToList(ball);

    // setare dim pentru caramizi si patrate pt animatie
    width = logicSpace.width / 12;
    height = logicSpace.height / 2 / 6;
    length = 0.3f;

    // contur caramizi
    Mesh* caramidaContur = object2DTema1::CreateRectangle("caramidaContur", cornerGrila, width, height, glm::vec3(0, 0, 0), false);
    AddMeshToList(caramidaContur);

    // generare random caramizi
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 12; i++) {
            Brick b;
            b.pos = glm::vec2(i * width, (logicSpace.height - 1) - (j + 1) * height);
            b.size = glm::vec2(width * 0.9f, height * 0.9f);
            b.broke = 0;
            b.hitsLeft = 1;

            // genereaza o culoare random
            int k = rand() % 6;

            switch (k) {
            case 0: {
                b.color = culori[0];
                b.hitsLeft = 1;
                Mesh* caramidaRosie = object2DTema1::CreateRectangle("caramidaRosie", cornerGrila, width, height, glm::vec3(1, 0, 0), true);
                AddMeshToList(caramidaRosie);
                Mesh* sqAnimatieRosu = object2DTema1::CreateSquare("sqAnimatieRosu", cornerGrila, length, culori[0], true);
                AddMeshToList(sqAnimatieRosu);
                break;
            }
            case 1: {
                b.color = culori[1];
                b.hitsLeft = 2;
                Mesh* caramidaAlbastru = object2DTema1::CreateRectangle("caramidaAlbastru", cornerGrila, width, height, b.color, true);
                AddMeshToList(caramidaAlbastru);
                Mesh* sqAnimatieAlbastru = object2DTema1::CreateSquare("sqAnimatieAlbastru", cornerGrila, length, culori[1], true);
                AddMeshToList(sqAnimatieAlbastru);
                break;
            }
            case 2: {
                b.color = culori[2];
                b.hitsLeft = 3;
                Mesh* caramidaVerde = object2DTema1::CreateRectangle("caramidaVerde", cornerGrila, width, height, b.color, true);
                AddMeshToList(caramidaVerde);
                Mesh* sqAnimatieVerde = object2DTema1::CreateSquare("sqAnimatieVerde", cornerGrila, length, culori[2], true);
                AddMeshToList(sqAnimatieVerde);
                break;
            }
            case 3: {
                b.color = culori[3];
                b.hitsLeft = 4;
                Mesh* caramidaMov = object2DTema1::CreateRectangle("caramidaMov", cornerGrila, width, height, b.color, true);
                AddMeshToList(caramidaMov);
                Mesh* sqAnimatieMov = object2DTema1::CreateSquare("sqAnimatieMov", cornerGrila, length, culori[3], true);
                AddMeshToList(sqAnimatieMov);
                break;
            }
            case 4: {
                b.color = culori[4];
                b.hitsLeft = 1;
                Mesh* caramidaPortocaliu = object2DTema1::CreateRectangle("caramidaPortocaliu", cornerGrila, width, height, b.color, true);
                AddMeshToList(caramidaPortocaliu);
                Mesh* sqAnimatiePortocaliu = object2DTema1::CreateSquare("sqAnimatiePortocaliu", cornerGrila, length, culori[4], true);
                AddMeshToList(sqAnimatiePortocaliu);
                break;
            }
            case 5: {
                b.color = culori[5];
                b.hitsLeft = 5;
                Mesh* caramidaRoz = object2DTema1::CreateRectangle("caramidaRoz", cornerGrila, width, height, b.color, true);
                AddMeshToList(caramidaRoz);
                Mesh* sqAnimatieRoz = object2DTema1::CreateSquare("sqAnimatieRoz", cornerGrila, length, culori[5], true);
                AddMeshToList(sqAnimatieRoz);
                break;
            }
            }
            bricks.push_back(b);
        }
    }

    // patrat pentru animatie
    length = 0.3f;
    Mesh* sqAnimatieAlb = object2DTema1::CreateSquare("sqAnimatieAlb", cornerGrila, length, glm::vec3(1, 1, 1), true);
    AddMeshToList(sqAnimatieAlb);
}


glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace) {
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear) {
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);
    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1::Update(float deltaTimeSeconds) {
    // afisare ultima fereastra, inchiere joc
    if (gameOver || numCaramizi <= 0) {
        DrawSceneGameOver(visMatrix);
        // reset
        if (window->KeyHold(GLFW_KEY_R)) {
            GameOver();
            reset = true;
        }
        // quit
        else if (window->KeyHold(GLFW_KEY_Q)) {
            exit(0);
        }
        return;
    }

    // editorul de nava spatiala
    if (beforeStart || reset) {
        if (reset) {
            beforeStart = true;
            reset = false;
        }

        glm::ivec2 resolution = window->GetResolution();

        viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
        SetViewportArea(viewSpace, glm::vec3(0), true);

        visMatrix = glm::mat3(1);
        visMatrix *= VisualizationTransf2D(logicSpace, viewSpace);

        // verificare constrangeri
        if (totalBlocks < 1 || totalBlocks >= 11)
            constraintsMet = false;
        else
            constraintsMet = true;

        if (!IsShipConnected())
            constraintsMet = false;

        if (!CheckMotorConstraint())
            constraintsMet = false;

        if (!CheckTunConstraint())
            constraintsMet = false;

        DrawScene(visMatrix);

        return;
    }

    if (!beforeStart) {
        glm::ivec2 resolution = window->GetResolution();

        viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
        SetViewportArea(viewSpace, glm::vec3(0), true);

        visMatrix = glm::mat3(1);
        visMatrix *= VisualizationTransf2D(logicSpace, viewSpace);

        if (!ballOnPaddle) {
            // calculez pana unde poate ajunge bila, adica nu vreau sa depaseasca
            // sa ajunga unde am scorul si vietile
            float margin = (logicSpace.height - 1) - logicSpace.height / 2 / 6;
            float top = logicSpace.height - 0.5f;
            ballPos += ballSpeedVect * deltaTimeSeconds;

            glm::vec2 normal;

            // verific cand se loveste de caramizi
            for (int j = 0; j < 5; j++) {
                for (int i = 0; i < 12; i++) {
                    int index = i + j * 12;
                    Brick& brick = bricks[index];

                    // verific daca e sparta
                    if (brick.broke == 1)
                        continue;

                    // recalculez centrul si dimensiunea caramizi pentru coliziune
                    glm::vec2 centerBrick = brick.pos + brick.size * 0.5f;
                    glm::vec2 sizeBrick = brick.size * 0.5f;

                    // verific daca a lovit caramida
                    if (CheckAABB(ballPos, ballRadius, centerBrick, sizeBrick, normal)) {
                        // calculez cum se duce mingea dupa ce loveste, adica isi pastreaza directia in functie
                        // de vect normal si cat se duce
                        ballSpeedVect = ballSpeedVect - 2.0f * glm::dot(ballSpeedVect, normal) * normal;

                        // daca a lovit, pun si efectul de shaking
                        isShaking = true;
                        shakeTime = shakeDuration;

                        // scad damage si verific daca a fost sparta
                        brick.hitsLeft--;
                        if (brick.hitsLeft <= 0 && brick.broke == 0) {
                            brick.broke = 2; // micsorare
                            score++;
                        }
                        else {
                            // scad in prioritate, adica se va afisa urmatoarea 
                            // caramida daca mai exista
                            brick.color = culori[brick.hitsLeft - 1];
                            score += brick.hitsLeft + 1;
                        }

                        // pentru animatie cand lovesc o caramida
                        int numAnimatie = 5;
                        for (int k = 0; k < numAnimatie; k++) {
                            Particle p;
                            // pozitia initiala a particulei
                            p.pos = brick.pos - glm::vec2(0, brick.size.y / 2) + glm::vec2(brick.size.x / 2, 0);
                            // unghi random
                            float angle = glm::radians((rand() % 360) * 1.0f);
                            // viteza random
                            float speed = (rand() % 50) / 10.0f;
                            // calculez vectorul viteza in functie de unghi si viteza
                            p.speed = glm::vec2(cos(angle), sin(angle)) * speed;
                            p.color = brick.color;
                            // cat dureaza
                            p.life = 2.0f;
                            particles.push_back(p);
                        }

                        numCaramizi--;

                        // verific daca scorul a crescut cu 5, pentru a creste
                        // viteza mingii
                        int currentScoreRest = score / 5;
                        int prevScoreRest = 0;
                        if (currentScoreRest > prevScoreRest) {
                            prevScoreRest = currentScoreRest;
                            ballSpeed += 0.5f;
                            ballSpeedVect = glm::normalize(ballSpeedVect) * ballSpeed;
                        }

                        break;
                    }
                }
            }

            // pentru micsorarea caramizilor cand se sparg
            for (Brick& b : bricks) {
                if (b.broke == 2) {
                    float offset = 1.0f * deltaTimeSeconds;
                    b.size.x -= offset;
                    b.size.y -= offset;

                    if (b.size.x <= 1.0f || b.size.y <= 1.0f) {
                        b.size.x = 0.0f;
                        b.size.y = 0.0f;
                        b.broke = 1;
                    }
                }
            }

            // dupa ce a lovit caramida, afisez animatia
            for (int i = 0; i < (int)particles.size(); ) {
                particles[i].pos += particles[i].speed * deltaTimeSeconds;
                particles[i].life -= deltaTimeSeconds;

                if (particles[i].life <= 0)
                    particles.erase(particles.begin() + i);
                else
                    i++;
            }

            // cand lovesc paleta
            if (CheckAABB(ballPos, ballRadius, glm::vec2(shipGameX, shipGameY), glm::vec2(widthPaddle * 0.5f, heightPaddle), normal)) {
                // calculez unde lovesc si unghiul cu care voi ricosa
                float intersection = (ballPos.x - shipGameX) / (widthPaddle / 2.0f);
                ballSpeedVect = glm::vec2(ballSpeed * sin(intersection), abs(ballSpeed * cos(intersection)));

                // la fel, pentru animatie
                int numAnimatie = 5;
                for (int i = 0; i < numAnimatie; i++) {
                    Particle p;
                    // pozitia initiala a particulei
                    p.pos = glm::vec2(shipGameX, shipGameY + heightPaddle);
                    // unghi random
                    float angle = glm::radians((rand() % 360) * 1.0f);
                    // viteza random
                    float speed = (rand() % 50) / 10.0f;
                    // calculez vectorul viteza in functie de unghi si viteza
                    p.speed = glm::vec2(cos(angle), sin(angle)) * speed;
                    p.color = culori[6];
                    // cat dureaza
                    p.life = 2.0f;
                    particles.push_back(p);
                }
            }

            // daca loveste peretii
            if (ballPos.x - ballRadius < 0 || ballPos.x + ballRadius > logicSpace.width)
                ballSpeedVect.x *= -1;
            if (ballPos.y + ballRadius > top)
                ballSpeedVect.y *= -1;

            // daca ajunge jos, trebuie sa resetez
            if (ballPos.y - ballRadius < 0) {
                lives--;
                if (lives == 0) {
                    gameOver = true;
                    return;
                }

                ballOnPaddle = true;
                ballPos = glm::vec2(shipGameX, shipGameY + heightPaddle + ballRadius + 0.01f);
                ballSpeedVect = glm::vec2(0, 0);
                shipGameX = logicSpace.width / 2;
                shipGameY = 1.0f;
            }

            // daca a lovit paleta, afisez animatiile
            for (int i = 0; i < (int)particles.size(); ) {
                particles[i].pos += particles[i].speed * deltaTimeSeconds;
                particles[i].life -= deltaTimeSeconds;

                if (particles[i].life <= 0)
                    particles.erase(particles.begin() + i);
                else
                    i++;
            }
        }
        else {
            // bila nu a fost lansata
            ballPos = glm::vec2(shipGameX, shipGameY + heightPaddle + ballRadius + 0.01f);
        }

        // daca a lovit o caramida, efectul de shaking
        if (isShaking) {
            shakeTime -= deltaTimeSeconds;
            if (shakeTime <= 0) {
                isShaking = false;
                shakeOffset = glm::vec2(0, 0);
            }
            else {
                // generez random coordonatele pentru shake
                float offsetX = ((rand() % 100) / 100.0f - 0.5f) * 2 * shakeIntensity;
                float offsetY = ((rand() % 100) / 100.0f - 0.5f) * 2 * shakeIntensity;
                shakeOffset = glm::vec2(offsetX, offsetY);
            }
        }

        // aplic efectul de shaking
        glm::mat3 shakenMatrix = visMatrix * transform2D::Translate(shakeOffset.x, shakeOffset.y);
        DrawSceneGame(shakenMatrix);
    }
}


void Tema1::FrameEnd()
{
}

void Tema1::DrawScene(glm::mat3 visMatrix) {
    // desenare contur unde se construieste nava spatiala
    modelMatrix = visMatrix * transform2D::Translate(gridStartX - 1, gridStartY - 1);
    RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

    // desenez blocuri cand se pun in grid, cand se construieste nava
    if (isMoving && currentBlock != EMPTY) {
        float offset = cellSize / 2.0f;
        modelMatrix = visMatrix * transform2D::Translate(mousePosition.x - offset, mousePosition.y - offset);

        switch (currentBlock) {
        case SOLID:
            RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
            break;
        case ENGINE:
            RenderMesh2D(meshes["motor"], shaders["VertexColor"], modelMatrix);
            break;
        case TUN:
            RenderMesh2D(meshes["tun"], shaders["VertexColor"], modelMatrix);
            break;
        case SOLID2:
            RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix);
            break;
        default:
            break;
        }
    }

    // desenare patratele albastre din grid
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            modelMatrix = visMatrix * transform2D::Translate(gridStartX + i * cellSize, gridStartY + j * cellSize);

            BlockType block = grid[j][i];
            float offset = 0.15f;

            if (block == EMPTY) {
                if (i == (int)(gridWidth / 2) && j == (int)(gridHeight / 2))
                    RenderMesh2D(meshes["squareCentru"], shaders["VertexColor"], modelMatrix);
                else
                    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
            }
            else if (block == SOLID) {
                modelMatrix = visMatrix * transform2D::Translate(gridStartX + i * cellSize - offset, gridStartY + j * cellSize - offset);
                RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
            }
            else if (block == SOLID2) {
                modelMatrix = visMatrix * transform2D::Translate(gridStartX + i * cellSize - offset, gridStartY + j * cellSize - offset);
                RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix);
            }
            else if (block == ENGINE) {
                modelMatrix = visMatrix * transform2D::Translate(gridStartX + i * cellSize - offset, gridStartY + j * cellSize - offset);
                RenderMesh2D(meshes["motor"], shaders["VertexColor"], modelMatrix);
            }
            else if (block == TUN) {
                modelMatrix = visMatrix * transform2D::Translate(gridStartX + i * cellSize - offset, gridStartY + j * cellSize - offset);
                RenderMesh2D(meshes["tun"], shaders["VertexColor"], modelMatrix);
            }
        }
    }

    // desenare piese in meniul din stanga
    modelMatrix = visMatrix * transform2D::Translate(5 - 0.5f, logicSpace.height / 4 / 2 - 0.5f);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(5 - 0.5f, 6.8f * logicSpace.height / 4 / 2);
    RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(5 - 0.5f, 3 * logicSpace.height / 4 / 2 - 0.5f);
    RenderMesh2D(meshes["motor"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(5 - 0.5f, 4.75 * logicSpace.height / 4 / 2 - 1);
    RenderMesh2D(meshes["tun"], shaders["VertexColor"], modelMatrix);

    // desenare patrate verzi pentru numararea pieselor
    float newGridStartX = gridStartX - 2;
    float newGridStartXTmp = newGridStartX;
    for (int i = 0; i < (10 - totalBlocks); i++) {
        newGridStartXTmp = newGridStartX + i * (cellSize + 1);
        modelMatrix = visMatrix * transform2D::Translate(newGridStartXTmp, 16);
        RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    }

    // desenez dreptunghiurile rosii
    float heightRedTmp = logicSpace.height / 4;
    float heightRed = logicSpace.height / 4;
    for (int i = 0; i < 4; i++) {
        heightRed = i * heightRedTmp;
        modelMatrix = visMatrix * transform2D::Translate(0, heightRed);
        RenderMesh2D(meshes["rectangle2"], shaders["VertexColor"], modelMatrix);
    }

    // pentru butonul start in functie de constrangeri
    newGridStartXTmp = newGridStartX + 10 * (1.2f + 1);
    modelMatrix = visMatrix * transform2D::Translate(newGridStartXTmp, 16);

    if (constraintsMet)
        RenderMesh2D(meshes["start2"], shaders["VertexColor"], modelMatrix);
    else
        RenderMesh2D(meshes["start1"], shaders["VertexColor"], modelMatrix);

    // contur ecran
    modelMatrix = visMatrix * transform2D::Translate(0.01f, 0.01f);
    RenderMesh2D(meshes["contur"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawSceneGame(glm::mat3 visMatrix) {
    // desenare nava spatiala, desenez bucata cu bucata
    if (!shipParts.empty()) {
        float half = cellSize * 0.5f;
        for (ShipPart ship : shipParts) {
            float drawX = shipGameX + ship.pos.x - half;
            modelMatrix = visMatrix * transform2D::Translate(drawX, shipGameY + ship.pos.y);
            switch (ship.type) {
            case SOLID:
                RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
                break;
            case SOLID2:
                RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix);
                break;
            case ENGINE:
                RenderMesh2D(meshes["motor"], shaders["VertexColor"], modelMatrix);
                break;
            case TUN:
                RenderMesh2D(meshes["tun"], shaders["VertexColor"], modelMatrix);
                break;
            default:
                break;
            }
        }
    }

    textRenderer->RenderText("Lives: ", 1075, 10, 0.5f, glm::vec3(1, 1, 1));

    // desenare inimi pentru vieti
    float spacing = 1;
    for (int i = 0; i < lives; i++) {
        modelMatrix = visMatrix * transform2D::Translate(logicSpace.width - 3 + i * spacing, logicSpace.height - 0.3f);
        RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix);
    }

    textRenderer->RenderText("Score: " + std::to_string(score), 1, 10, 0.5f, glm::vec3(1, 1, 1));

    // desenare minge
    modelMatrix = visMatrix * transform2D::Translate(ballPos.x, ballPos.y);
    RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);

    // desenare caramizi
    float spacingWidth = logicSpace.width / 12;
    float spacingHeight = logicSpace.height / 2 / 6;
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 12; i++) {
            int index = i + j * 12;
            Brick& brick = bricks[index];

            if (brick.broke == 1)
                continue;

            // calculare coordonate caramida
            float x = brick.pos.x;
            float y = brick.pos.y;

            float scaleX = brick.size.x / spacingWidth;
            float scaleY = brick.size.y / spacingHeight;

            // translatare si scalare pentru micsorare
            modelMatrix = visMatrix
                * transform2D::Translate(x + brick.size.x / 2.0f, y + brick.size.y / 2.0f)
                * transform2D::Scale(scaleX, scaleY)
                * transform2D::Translate(-brick.size.x / 2.0f, -brick.size.y / 2.0f);

            // contur
            glLineWidth(3.0f);
            RenderMesh2D(meshes["caramidaContur"], shaders["VertexColor"], modelMatrix);

            if (brick.color == glm::vec3(1, 0, 0)) {
                RenderMesh2D(meshes["caramidaRosie"], shaders["VertexColor"], modelMatrix);
            }
            else if (brick.color == glm::vec3(0, 0, 1)) {
                RenderMesh2D(meshes["caramidaAlbastru"], shaders["VertexColor"], modelMatrix);
            }
            else if (brick.color == glm::vec3(0, 1, 0)) {
                RenderMesh2D(meshes["caramidaVerde"], shaders["VertexColor"], modelMatrix);
            }
            else if (brick.color == glm::vec3(0.5f, 0.0f, 0.5f)) {
                RenderMesh2D(meshes["caramidaMov"], shaders["VertexColor"], modelMatrix);
            }
            else if (brick.color == glm::vec3(1.0f, 0.5f, 0.0f)) {
                RenderMesh2D(meshes["caramidaPortocaliu"], shaders["VertexColor"], modelMatrix);
            }
            else if (brick.color == glm::vec3(1.0f, 0.0f, 1.0f)) {
                RenderMesh2D(meshes["caramidaRoz"], shaders["VertexColor"], modelMatrix);
            }
        }
    }

    // desenare particule
    for (int i = 0; i < (int)particles.size(); i++) {
        Particle& p = particles[i];

        if (p.color == culori[0]) {
            glm::mat3 modelMatrix = visMatrix * transform2D::Translate(p.pos.x, p.pos.y);
            RenderMesh2D(meshes["sqAnimatieRosu"], shaders["VertexColor"], modelMatrix);
        }
        else if (p.color == culori[1]) {
            glm::mat3 modelMatrix = visMatrix * transform2D::Translate(p.pos.x, p.pos.y);
            RenderMesh2D(meshes["sqAnimatieAlbastru"], shaders["VertexColor"], modelMatrix);
        }
        else if (p.color == culori[2]) {
            glm::mat3 modelMatrix = visMatrix * transform2D::Translate(p.pos.x, p.pos.y);
            RenderMesh2D(meshes["sqAnimatieVerde"], shaders["VertexColor"], modelMatrix);
        }
        else if (p.color == culori[3]) {
            glm::mat3 modelMatrix = visMatrix * transform2D::Translate(p.pos.x, p.pos.y);
            RenderMesh2D(meshes["sqAnimatieMov"], shaders["VertexColor"], modelMatrix);
        }
        else if (p.color == culori[4]) {
            glm::mat3 modelMatrix = visMatrix * transform2D::Translate(p.pos.x, p.pos.y);
            RenderMesh2D(meshes["sqAnimatiePortocaliu"], shaders["VertexColor"], modelMatrix);
        }
        else if (p.color == culori[5]) {
            glm::mat3 modelMatrix = visMatrix * transform2D::Translate(p.pos.x, p.pos.y);
            RenderMesh2D(meshes["sqAnimatieRoz"], shaders["VertexColor"], modelMatrix);
        }
        else if (p.color == culori[6]) {
            glm::mat3 modelMatrix = visMatrix * transform2D::Translate(p.pos.x, p.pos.y);
            RenderMesh2D(meshes["sqAnimatieAlb"], shaders["VertexColor"], modelMatrix);
        }
    }
}

void Tema1::DrawSceneGameOver(glm::mat3 visMatrix) {
    textRenderer->RenderText("Game Over! ", 500, 100, 1.0f, glm::vec3(1, 1, 1));
    textRenderer->RenderText("HighScore: " + std::to_string(score), 450, 200, 1.0f, glm::vec3(1, 1, 1));
    textRenderer->RenderText("Score: " + std::to_string(score), 500, 300, 1.0f, glm::vec3(1, 1, 1));
    textRenderer->RenderText("Please press R to restart or Q to exit!", 400, 695, 0.5f, glm::vec3(1, 1, 1));
}


void Tema1::OnInputUpdate(float deltaTime, int mods) {
    // daca am intrat in joc, misc nava din sageti
    if (!beforeStart) {
        if (window->KeyHold(GLFW_KEY_LEFT)) {
            // la stanga
            shipGameX -= 10 * deltaTime;
            if (shipGameX < 0)
                shipGameX = logicSpace.width - widthPaddle / 2;
        }

        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            // la dreapta
            shipGameX += 10 * deltaTime;
            if (shipGameX + widthPaddle > logicSpace.width)
                shipGameX = 0;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods) {
    // daca mingea se afla pe paleta, o pot lansa cu space
    if (key == GLFW_KEY_SPACE && ballOnPaddle) {
        // cu normalize calculez vectorul care se afla la 45 de grade
        // adica iau 2 puncte pentru a obtine un triunghi dreptunghic
        // isoscel, si il fac de lungime 1 pe care il inmultesc cu 
        // viteza bilei
        ballSpeedVect = glm::normalize(glm::vec2(1.0f, 1.0f)) * ballSpeed;
        ballOnPaddle = false;
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
}

glm::vec2 Tema1::ConvertScreenToLogic(int mouseX, int mouseY) {
    // convertesc coordonatele mouse-ului in coordonate logice
    glm::ivec2 resolution = window->GetResolution();
    int screenY = resolution.y - mouseY;

    float sx = viewSpace.width / logicSpace.width;
    float sy = viewSpace.height / logicSpace.height;
    float tx = viewSpace.x - sx * logicSpace.x;
    float ty = viewSpace.y - sy * logicSpace.y;

    float logicX = (mouseX - tx) / sx;
    float logicY = (screenY - ty) / sy;

    return glm::vec2(logicX, logicY);
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    mousePosition = ConvertScreenToLogic(mouseX, mouseY);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    glm::vec2 logicPos = ConvertScreenToLogic(mouseX, mouseY);

    if (beforeStart) {
        if (button == 1) {
            // calculez aria unde pot da click
            float panelX = 4.5f;
            float panelWidth = 1.0f;

            // calculez inaltimea pe care pot da click
            float solidY = logicSpace.height / 8 - 0.5f;
            float engineY = 3 * logicSpace.height / 8 - 0.5f;
            float tunY = 4.75 * logicSpace.height / 8 - 0.5f;
            float solid2Y = 7 * logicSpace.height / 8 - 0.5f;

            if (logicPos.x >= panelX && logicPos.x <= panelX + panelWidth) {
                if (logicPos.y >= solidY && logicPos.y < solidY + cellSize) {
                    isMoving = true;
                    currentBlock = SOLID;
                    mousePosition = logicPos;
                }
                else if (logicPos.y >= engineY && logicPos.y < engineY + cellSize) {
                    isMoving = true;
                    currentBlock = ENGINE;
                    mousePosition = logicPos;
                }
                else if (logicPos.y >= tunY && logicPos.y < tunY + cellSize) {
                    isMoving = true;
                    currentBlock = TUN;
                    mousePosition = logicPos;
                }
                else if (logicPos.y >= solid2Y && logicPos.y < solid2Y + cellSize) {
                    isMoving = true;
                    currentBlock = SOLID2;
                    mousePosition = logicPos;
                }
            }

            // click pentru butonul start
            if (logicPos.x >= (gridStartX - 2) && logicPos.x <= (gridStartX - 2 + 10 * (1.2f + 1) + 1.2f) &&
                logicPos.y >= 16 && logicPos.y <= (16 + 1.2f)) {
                if (constraintsMet)
                    StartGame();
            }
        }

        if (button == 2) {
            if (logicPos.x >= gridStartX && logicPos.y >= gridStartY) {
                // calculez pozitia
                int col = (int)((logicPos.x - gridStartX) / cellSize);
                int row = (int)((logicPos.y - gridStartY) / cellSize);

                // logica pentru eliminare blocuri
                if (row >= 0 && row < gridHeight && col >= 0 && col < gridWidth) {
                    if (grid[row][col] != EMPTY) {
                        if (grid[row][col] == ENGINE) {
                            grid[row][col] = EMPTY;
                            grid[row - 1][col] = EMPTY;
                            underBlock = EMPTY;
                            totalBlocks--;
                            totalPieseOcupa -= 2;
                        }
                        else if (grid[row][col] == TUN) {
                            grid[row][col] = EMPTY;
                            grid[row + 1][col] = EMPTY;
                            grid[row + 2][col] = EMPTY;
                            totalBlocks--;
                            totalPieseOcupa -= 3;
                        }
                        else if (grid[row][col] == SOLID) {
                            grid[row][col] = EMPTY;
                            totalBlocks--;
                            totalPieseOcupa--;
                        }
                        else if (grid[row][col] == SOLID2) {
                            grid[row][col] = EMPTY;
                            totalBlocks--;
                            totalPieseOcupa--;
                        }
                    }
                }
            }
        }
    }
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    glm::vec2 logicPos = ConvertScreenToLogic(mouseX, mouseY);

    if (beforeStart) {
        // daca misc blocul
        if (button == 1 && isMoving) {
            isMoving = false;

            if (logicPos.x >= gridStartX && logicPos.y >= gridStartY) {
                // calculez pozitia
                int col = (int)((logicPos.x - gridStartX) / cellSize);
                int row = (int)((logicPos.y - gridStartY) / cellSize);

                // logica plasare bloc
                if (row >= 0 && row < gridHeight && col >= 0 && col < gridWidth) {
                    if (grid[row][col] == EMPTY && totalBlocks < 10) {
                        if (currentBlock == ENGINE) {
                            if (row - 1 >= 0 && grid[row - 1][col] == EMPTY) {
                                grid[row][col] = currentBlock;
                                grid[row - 1][col] = OCCUPIED;
                                underBlock = OCCUPIED;
                                totalBlocks++;
                                totalPieseOcupa += 2;
                            }
                        }
                        else if (currentBlock == TUN) {
                            if (row + 2 < gridHeight && grid[row + 1][col] == EMPTY &&
                                grid[row + 2][col] == EMPTY) {
                                grid[row][col] = currentBlock;
                                grid[row + 1][col] = OCCUPIED;
                                grid[row + 2][col] = OCCUPIED;
                                totalBlocks++;
                                totalPieseOcupa += 3;
                            }
                        }
                        else if (currentBlock == SOLID) {
                            grid[row][col] = currentBlock;
                            totalBlocks++;
                            totalPieseOcupa++;
                        }
                        else if (currentBlock == SOLID2) {
                            grid[row][col] = currentBlock;
                            totalBlocks++;
                            totalPieseOcupa++;
                        }
                    }
                }
            }
            currentBlock = EMPTY;
        }
    }
}


void Tema1::OnWindowResize(int width, int height) {
    UpdateLayout();
}


bool Tema1::IsShipConnected() {
    // nu am pus niciun bloc
    if (totalBlocks == 0) {
        return false;
    }

    // caut un bloc care a fost pus de unde sa incep bfs-ul
    int startRow = -1;
    int startCol = -1;
    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) {
            if (grid[i][j] != EMPTY) {
                startRow = i;
                startCol = j;
                break;
            }
        }

        if (startRow != -1)
            break;
    }

    std::queue<std::pair<int, int>> q;
    int blocksFound = 0;
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    bool visited[9][17];
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 17; j++) {
            visited[i][j] = false;
        }
    }

    q.push({ startRow, startCol });
    visited[startRow][startCol] = true;

    // aplic bfs pentru a verifica daca piesele sunt conectate
    while (!q.empty()) {
        std::pair<int, int> current = q.front();
        q.pop();
        blocksFound++;

        int fst = current.first;
        int snd = current.second;

        for (int i = 0; i < 4; i++) {
            int newRow = fst + dx[i];
            int newCol = snd + dy[i];

            if (newRow >= 0 && newRow < gridHeight && newCol >= 0 && newCol < gridWidth &&
                grid[newRow][newCol] != EMPTY &&
                !visited[newRow][newCol]) {
                visited[newRow][newCol] = true;
                q.push({ newRow, newCol });
            }
        }
    }

    // daca numarul de blocuri gasit e egal cu cel al pieselor pe care le-am 
    // plasat, returneaza true
    if (blocksFound == totalPieseOcupa)
        return true;

    return false;
}


bool Tema1::CheckMotorConstraint() {
    for (int i = 0; i < 17; i++) {
        for (int j = 0; j < 9; j++) {
            // daca am gasit un motor, verific toate liniile de dedesubt
            // pe coloana
            if (grid[j][i] == ENGINE) {
                for (int k = j - 2; k >= 0; k--) {
                    if (grid[k][i] != EMPTY)
                        return false;
                }
            }
        }
    }

    return true;
}


bool Tema1::CheckTunConstraint() {
    for (int j = 16; j >= 0; j--) {
        for (int i = 8; i >= 0; i--) {
            // daca dau de tun, verific toate liniile de deasupra pe coloana
            if (grid[i][j] == TUN) {
                for (int k = i + 3; k < gridHeight; k++) {
                    if (grid[k][j] != EMPTY)
                        return false;
                }
            }
        }
    }

    return true;
}


void Tema1::StartGame() {
    if (!beforeStart) return;

    // construire nava in functie de ce blocuri am
    shipParts.clear();
    for (int r = 0; r < gridHeight; ++r) {
        for (int c = 0; c < gridWidth; ++c) {
            BlockType b = grid[r][c];
            if (b != EMPTY && b != OCCUPIED)
                shipParts.push_back({ b, glm::vec2(c * cellSize, r * cellSize) });
        }
    }

    // calculez centrul navei
    float minX = shipParts[0].pos.x, maxX = shipParts[0].pos.x;
    float minY = shipParts[0].pos.y, maxY = shipParts[0].pos.y;
    for (ShipPart sp : shipParts) {
        if (sp.pos.x < minX)
            minX = sp.pos.x;
        if (sp.pos.x > maxX)
            maxX = sp.pos.x;
        if (sp.pos.y < minY)
            minY = sp.pos.y;
        if (sp.pos.y > maxY)
            maxY = sp.pos.y;
    }
    glm::vec2 center((minX + maxX) * 0.5f, (minY + maxY) * 0.5f);

    // recalculez cu noul centru obtinut
    for (ShipPart& sp : shipParts)
        sp.pos -= center;

    // pozitia initiala a paletei
    float midShip = (cellSize * shipParts.size()) / 2;
    shipGameX = logicSpace.width / 2.0f;
    shipGameY = 1.0f;
    beforeStart = false;

    // numar cate piese are paleta
    int nrPiese = 0;
    for (int r = 0; r < gridHeight; ++r)
        for (int c = 0; c < gridWidth; ++c)
            if (grid[r][c] == SOLID || grid[r][c] == SOLID2)
                nrPiese++;

    // in functie de cate piese am in paleta, calculez lungimea
    if (nrPiese > 0)
        widthPaddle = nrPiese * cellSize;
    else
        widthPaddle = cellSize * 3.0f;
}


bool Tema1::CheckAABB(glm::vec2 ballPos, float radius, glm::vec2 boxPos, glm::vec2 boxSize, glm::vec2& collisionNormal) {
    // calculez distanta de la dreptunghi la bila
    glm::vec2 diff = ballPos - boxPos;

    // verific daca e in lateralul paletei
    if (diff.x < -boxSize.x)
        diff.x = -boxSize.x;
    else if (diff.x > boxSize.x)
        diff.x = boxSize.x;

    // verific daca e pe paleta
    if (diff.y < -boxSize.y)
        diff.y = -boxSize.y;
    else if (diff.y > boxSize.y)
        diff.y = boxSize.y;

    // obtin cel mai apropiat punct de la paleta la bila, practic punctul tangent
    glm::vec2 closest = boxPos + diff;
    // distanta de la paleta la bila
    glm::vec2 dist = ballPos - closest;
    float lungime = glm::length(dist);

    if (lungime <= ballRadius) {
        // il transform intr-un vector de lungime 1 care isi pastreaza directia
        collisionNormal = glm::normalize(dist);
        return true;
    }

    return false;
}


void Tema1::GameOver() {
    // reinitializare alte variabile
    isMoving = false;
    currentBlock = EMPTY;
    underBlock = EMPTY;
    totalBlocks = 0;
    totalPieseOcupa = 0;
    constraintsMet = false;
    beforeStart = true;
    score = 0;
    lives = 3;
    numCaramizi = 60;
    reset = false;
    gameOver = false;

    // initializare grid
    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) {
            grid[i][j] = EMPTY;
        }
    }

    // sterg ce aveam inainte
    shipParts.clear();

    // reinitializare variabile pentru minge: pozitie, dimensiune, viteza, daca
    // se afla pe paleta
    shipGameX = 0.0f;
    shipGameY = 0.0f;
    ballRadius = 0.5f;
    heightPaddle = 1.2f;
    ballPos = glm::vec2(shipGameX, shipGameY + heightPaddle + ballRadius + 0.01f);
    ballSpeedVect = glm::vec2(0, 0);
    ballOnPaddle = true;
    ballSpeed = 5.0f;
    widthPaddle = 0.0f;

    // setare dim pentru caramizi si patrate pt animatie
    float width = logicSpace.width / 12;
    float height = logicSpace.height / 2 / 6;

    // generare random caramizi
    bricks.clear();
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 12; i++) {
            Brick b;
            b.pos = glm::vec2(i * width, (logicSpace.height - 1) - (j + 1) * height);
            b.size = glm::vec2(width, height);
            b.broke = 0;
            b.hitsLeft = 1;

            // genereaza o culoare random
            int k = rand() % 6;

            switch (k) {
            case 0: {
                b.color = culori[0];
                b.hitsLeft = 1;
                break;
            }
            case 1: {
                b.color = culori[1];
                b.hitsLeft = 2;
                break;
            }
            case 2: {
                b.color = culori[2];
                b.hitsLeft = 3;
                break;
            }
            case 3: {
                b.color = culori[3];
                b.hitsLeft = 4;
                break;
            }
            case 4: {
                b.color = culori[4];
                b.hitsLeft = 1;
                break;
            }
            case 5: {
                b.color = culori[5];
                b.hitsLeft = 5;
                break;
            }
            }
            bricks.push_back(b);
        }
    }
}
