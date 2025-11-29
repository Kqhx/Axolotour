#pragma once

#include "../Base/Model.h"

class Coin : public Model {
private:
    int valor = 1;
    bool tomada = false;

public:
    Coin() {

    }
	Coin(vector<Vertex>& vertices, unsigned int numVertices, vector<unsigned int>& indices, unsigned int numIndices, Camera* camera)
		: Model(vertices, numVertices, indices, numIndices, camera) {
		
	}
	Coin(string const& path, glm::vec3& actualPosition, Camera* cam, bool rotationX = false, bool rotationY = true, bool gamma = false)
		: Model(path, actualPosition, cam, rotationX, rotationY, gamma) {
		
	}
	Coin(string const& path, Camera* camera, bool rotationX = false, bool rotationY = true, bool gamma = false)
		: Model(path, camera, rotationX, rotationY, gamma) {
		
	}
    bool isTaken() const {
        return tomada;
    }

    int getValue() const {
        return valor;
    }

    void take() {
        tomada = true;
    }
};
