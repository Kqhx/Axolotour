#include "Vehicle.h"

void Vehicle::vehicleEnter(Axolotl* axo) {
    if (!isMountable || isActive) return;

    axolotlModel = axo;
    isActive = true;

    // mover mounted al vehículo real
    glm::vec3 pos = *emptyModel->getTranslate();
    mountedModel->setTranslate(&pos);
    mountedModel->setNextTranslate(&pos);

    // ocultar empty y ocultar player
    emptyModel->setActive(false);
    axo->setActive(false);
    Model* hitbox = (Model*)axo->getModelAttributes()->at(0).hitbox;
    hitbox->setActive(false);

    // activar mounted
    mountedModel->setActive(true);
    emptyModel->setActive(false);
}


void Vehicle::vehicleExit(Axolotl* axo) {
    if (!isActive) return;

    isActive = false;

    // pos donde se baja
    glm::vec3 pos = *mountedModel->getTranslate();
    pos.y += 2.0f;

    // mostrar jugador
    axo->setTranslate(&pos);
    axo->setNextTranslate(&pos);
    axo->setActive(true);
    Model* hitbox = (Model*)axo->getModelAttributes()->at(0).hitbox;
    hitbox->setActive(true);

    // mostrar empty
    emptyModel->setTranslate(&pos);
    emptyModel->setNextTranslate(&pos);
    emptyModel->setActive(true);

    // ocultar mounted
    mountedModel->setActive(false);

    axolotlModel = nullptr;
}

void Vehicle::Draw() {
    getCurrentModel()->Model::Draw();
}