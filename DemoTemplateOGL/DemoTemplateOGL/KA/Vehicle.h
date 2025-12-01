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
	virtual glm::vec3* getTranslate(int idx = 0) { return getCurrentModel()->Model::getTranslate(idx); }
	virtual glm::vec3* getNextTranslate(int idx = 0) { return getCurrentModel()->Model::getNextTranslate(idx); }
	virtual glm::vec3* getScale(int idx = 0) { return getCurrentModel()->Model::getScale(idx); }
	virtual float getRotX(int idx = 0) { return getCurrentModel()->Model::getRotX(idx); }
	virtual float getRotY(int idx = 0) { return getCurrentModel()->Model::getRotY(idx); }
	virtual float getRotZ(int idx = 0) { return getCurrentModel()->Model::getRotZ(idx); }
	virtual float getNextRotX(int idx = 0) { return getCurrentModel()->Model::getNextRotX(idx); }
	virtual float getNextRotY(int idx = 0) { return getCurrentModel()->Model::getNextRotY(idx); }
	virtual float getNextRotZ(int idx = 0) { return getCurrentModel()->Model::getNextRotZ(idx); }

	// SETTERS
	void setIsActive(bool newActive) { isActive = newActive; }
	void setIsFlyable(bool newFlyable) { isFlyable = newFlyable; }
	void setIsMountable(bool newMountable) { isMountable = newMountable; }
	void setSpeedMult(float newSpeedMult) { speedMult = newSpeedMult; }
	void setEmptyModel(Model* m) { emptyModel = m; }
	void setMountedModel(Model* m) { mountedModel = m; }
	virtual void setNextTranslate(glm::vec3* translate, int idx = 0) { getCurrentModel()->Model::setNextTranslate(translate, idx); }
	virtual void setTranslate(glm::vec3* translate, int idx = 0) { getCurrentModel()->Model::setTranslate(translate, idx); }
	virtual void setScale(glm::vec3* scale, int idx = 0) { getCurrentModel()->Model::setScale(scale, idx); }
	virtual void setRotX(float rotationAngle, int idx = 0) { getCurrentModel()->Model::setRotX(rotationAngle, idx); }
	virtual void setRotY(float rotationAngle, int idx = 0) { getCurrentModel()->Model::setRotY(rotationAngle, idx); }
	virtual void setRotZ(float rotationAngle, int idx = 0) { getCurrentModel()->Model::setRotZ(rotationAngle, idx); }
	virtual void setNextRotX(float rotationAngle, int idx = 0) { getCurrentModel()->Model::setNextRotX(rotationAngle, idx); }
	virtual void setNextRotY(float rotationAngle, int idx = 0) { getCurrentModel()->Model::setNextRotY(rotationAngle, idx); }
	virtual void setNextRotZ(float rotationAngle, int idx = 0) { getCurrentModel()->Model::setNextRotZ(rotationAngle, idx); }
	// FUNCTIONS
	void vehicleEnter(Axolotl* axoPlayer);
	void vehicleExit(Axolotl* axoPlayer);
	void Draw();

	~Vehicle() {
		if (mountedModel != NULL) delete mountedModel;
	}
};