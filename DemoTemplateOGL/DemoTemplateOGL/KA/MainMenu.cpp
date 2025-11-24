#include "MainMenu.h"

MainMenu::MainMenu(Camera* cam) {
    InitGraph(cam);
}

MainMenu::MainMenu(Model* camIni) {
    InitGraph(camIni->cameraDetails);
}

void MainMenu::InitGraph(Camera* camara) {
    glm::vec3 translate;
    glm::vec3 scale;
    glm::vec3 rotation;

    // Tamaño de flecha y menu en px
    float arrowW = 100.0f;
    float arrowH = 100.0f;
    float bgW = 1920.0f;
    float bgH = 1080.0f;

    // Centro actual
    float centerX = (float)SCR_WIDTH * 0.5f;
    float centerY = (float)SCR_HEIGHT * 0.5f;

    // Creamos la flecha
    Billboard2D* main = new Billboard2D((WCHAR*)L"textures/arrow.png", 6, 6, centerX, centerY * 0.5f + 25.0f, 0, camara);
    scale = glm::vec3(arrowW, arrowH, 0.0f);
    main->setScale(&scale);
    main->setRotZ(90);
    this->camara = main;
    billBoard2D.push_back(main);

    // --- Creamos el menú centrado (pivote en centro)
    billBoard2D.push_back(
        new Billboard2D((WCHAR*)L"KA/GUI/GameStart.png", 6, 6, centerX, centerY, 0, camara)
    );
    scale = glm::vec3(bgW, bgH, 0.0f);
    billBoard2D.back()->setScale(&scale);
}

int MainMenu::update(int& menuOption) {
    int winW = SCR_WIDTH;
    int winH = SCR_HEIGHT;
    float menuW = 600.0f;
    float menuH = 600.0f;
    float arrowW = 100.0f;
    float arrowH = 100.0f;
    float centerX = winW * 0.5f;
    float centerY = winH * 0.5f;

    // Posición del menú (pivote en el centro del billboard)
    glm::vec3 menuCenter(centerX, centerY, 0.0f);
    // Asegurar que el billboard del menu esté en el centro
    // (asumimos que billBoard2D[1] es el Menu.png)
    if (billBoard2D.size() > 1) {
        billBoard2D[1]->setTranslate(&menuCenter);
        billBoard2D[1]->setNextTranslate(&menuCenter);
    }

    // Calculamos la columna de la flecha (a la derecha del menu)
    float arrowX = centerX + (menuW * 0.5f) + (arrowW * 0.5f) + 20.0f; // +20px separación
    // Coordenada Y base del primer botón: arriba dentro del menú con algo de padding
    // Ajusta estos offsets según el sprite real del menu
    float firstButtonOffsetY = -(menuH * 0.5f) + 120.0f; // 120px desde el tope interno del menu
    float rowSpacing = 105.0f; // separación vertical entre filas (ajustable)

    glm::vec3 translate;

    switch (menuOption) {
    case 1:
        translate = glm::vec3(arrowX, centerY + firstButtonOffsetY + (2.4 * rowSpacing), 0);
        break;
    case 2:
        translate = glm::vec3(arrowX, centerY + firstButtonOffsetY + (4.1 * rowSpacing), 0);
        break;
    default:
        menuOption = 1;
        translate = glm::vec3(arrowX, centerY + firstButtonOffsetY + (2.4 * rowSpacing), 0);
        break;
    }

    // Ajusta la flecha (billBoard2D[0]) para que esté a la derecha del menú y alineada verticalmente
    if (!billBoard2D.empty()) {
        billBoard2D[0]->setTranslate(&translate);
        billBoard2D[0]->setNextTranslate(&translate);
    }

    return 0;
}

MainMenu* MainMenu::Render() {
    for (Billboard2D* b : billBoard2D)
        b->Draw();
    return this;
}

std::vector<Billboard2D*>* MainMenu::getLoadedBillboards2D() {
    return &billBoard2D;
}

Model* MainMenu::getMainModel() {
    return camara;
}

void MainMenu::setMainModel(Billboard2D* mainModel) {
    this->camara = mainModel;
}

//    void update() override;
MainMenu::~MainMenu() {
    for (Billboard2D* b : billBoard2D)
        delete b;
}
