#pragma once

#include "../Base/Model.h"
#include "../KA/Axolotl.h"

class Vehicle : public Model {
private:
	bool isActive = false;
	bool isFlyable = false;
	bool isMountable = false;
	Model* emptyModel = nullptr;
	Model* mountedModel = nullptr;
	Axolotl* axolotlModel = nullptr;
	float speedMult = 1.0f;
public:
	Vehicle() {

	}
	Vehicle(vector<Vertex>& vertices, unsigned int numVertices, vector<unsigned int>& indices, unsigned int numIndices, Camera* camera)
		: Model(vertices, numVertices, indices, numIndices, camera) {

	}
	Vehicle(string const& path, glm::vec3& actualPosition, Camera* cam, bool rotationX = false, bool rotationY = true, bool gamma = false)
		: Model(path, actualPosition, cam, rotationX, rotationY, gamma) {

	}
	Vehicle(string const& path, Camera* camera, bool rotationX = false, bool rotationY = true, bool gamma = false)
		: Model(path, camera, rotationX, rotationY, gamma) {

	}

	// GETTERS
	bool getIsActive() { return isActive; }
	bool getIsFlyable() { return isFlyable; }
	bool getIsMountable() { return isMountable; }
	float getSpeedMult() { return speedMult; }
	Model* getCurrentModel() { return isActive ? mountedModel : emptyModel; }

	// SETTERS
	void setIsActive(bool newActive) { isActive = newActive; }
	void setIsFlyable(bool newFlyable) { isFlyable = newFlyable; }
	void setIsMountable(bool newMountable) { isMountable = newMountable; }
	void setSpeedMult(float newSpeedMult) { speedMult = newSpeedMult; }
	void setEmptyModel(Model* m) { emptyModel = m; }
	void setMountedModel(Model* m) { mountedModel = m; }
	
	// FUNCTIONS
	void vehicleEnter(Axolotl* axoPlayer);
	void vehicleExit(Axolotl* axoPlayer);

};