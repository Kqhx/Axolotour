#include "Vehicle.h"
#include "../Terreno.h"

void Vehicle::setTerrain(Terreno* t) {
    terrain = t;
}

void Vehicle::setNextTranslate(glm::vec3* translate, int idx) {

    if (isActive && terrain) {
        glm::vec3 fixed = *translate;

        // altura real del terreno en esa X,Z
        float terrY = terrain->Superficie(fixed.x, fixed.z);

        // altura mínima para flotar
        float boatY = 11.0f;

        if (terrY > boatY)
            boatY = terrY;

        fixed.y = boatY;

        getCurrentModel()->Model::setNextTranslate(&fixed, idx);
        return;
    }

    getCurrentModel()->Model::setNextTranslate(translate, idx);
}

void Vehicle::setTranslate(glm::vec3* translate, int idx) {
    if (isActive && terrain) {
        glm::vec3 fixed = *translate;
        float terrY = terrain->Superficie(fixed.x, fixed.z);
        float boatY = 11.0f;
        if (terrY > boatY) boatY = terrY;
        fixed.y = boatY;
        getCurrentModel()->Model::setTranslate(&fixed, idx);
        return;
    }
    else {
        getCurrentModel()->Model::setTranslate(translate, idx);
    }
}

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
    pos.y -= 0.25f;

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