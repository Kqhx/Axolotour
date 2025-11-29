#include "Scenario.h"
#ifdef __linux__ 
#define ZeroMemory(x,y) memset(x,0,y)
#define wcscpy_s(x,y,z) wcscpy(x,z)
#define wcscat_s(x,y,z) wcscat(x,z)
#endif

Scenario::Scenario(Camera *cam) {
    glm::vec3 translate;
	glm::vec3 scale;
    Model* model = new Model("models/Cube/Cube.obj", cam);
	translate = glm::vec3(0.0f, 0.0f, 3.0f);
	scale = glm::vec3(0.25f, 0.25f, 0.25f);	// it's a bit too big for our scene, so scale it down
	model->setScale(&scale);
	model->setTranslate(&translate);
	model->setNextTranslate(&translate);
	InitGraph(model);
}
Scenario::Scenario(Model *camIni) {
    InitGraph(camIni);
}
void Scenario::InitGraph(Model *main) {
    float matAmbient[] = { 1,1,1,1 };
	float matDiff[] = { 1,1,1,1 };
	angulo = 0;
	camara = main;
	Model* model;
	ModelAttributes m;
	Axolotl* axo = getMainAxolotl();
	this->player = axo;
	

	// -------------------------------
	// -----     LOAD SKYBOX     -----
	// -------------------------------
    sky = new SkyDome(36, 36, 24,
		(WCHAR*)L"KA/Textures/Terrain/Sky_Day.jpg",   // Textura día
        (WCHAR*)L"KA/Textures/Terrain/Sky_Dusk.jpg",   // Textura tarde
		(WCHAR*)L"KA/Textures/Terrain/Sky_Night.jpg",   // Textura noche
        main->cameraDetails,
        8.0f);  // Inicia en hora 0 (medianoche) para ver ciclo completo
/*  std::cout << "\n=== SISTEMA DIA/NOCHE INICIADO ===" << std::endl;
    std::cout << "Ciclo completo: 6 minutos (360 segundos)" << std::endl;
    std::cout << "Cada periodo: 2 minutos (120 segundos)" << std::endl;
    std::cout << "DIA: 0:00-8:00 (0-120s)" << std::endl;
    std::cout << "TARDE: 8:00-16:00 (120-240s)" << std::endl;
    std::cout << "NOCHE: 16:00-24:00 (240-360s)" << std::endl;
    std::cout << "===================================\n" << std::endl;*/


	// -------------------------------
	// -----    LOAD TERRAINS    -----
	// -------------------------------
	terreno = new Terreno((WCHAR*)L"KA/Textures/Terrain/heightmap_JPG.jpg", (WCHAR*)L"KA/Textures/Terrain/texture_terrain_grass1.jpg", 800, 800, main->cameraDetails);
	water = new Water((WCHAR*)L"KA/Textures/Terrain/terrain_plain.bmp", (WCHAR*)L"KA/Textures/Terrain/texture_water.bmp", 800, 800, camara->cameraDetails);
	glm::vec3 translate, scale, rotation;
	translate = glm::vec3(0.0f, 11.5f, 0.0f);
	water->setRotX(180);
	water->setTranslate(&translate);
	ourModel.emplace_back(main);


	// -------------------------------
	// -----     LOAD MODELS     -----
	// -------------------------------
	// BOTE
	Vehicle* boat = new Vehicle("KA/Models/Vehicles/boat.fbx", main->cameraDetails);
	glm::vec3 boatPos = glm::vec3(-205.0f, 11.0f, -260.0f);
	boat->setTranslate(&boatPos);
	boat->setNextTranslate(&boatPos);
	boat->walkeable = true;
	boat->setIsActive(false);
	boat->setIsMountable(true);
	boat->setIsFlyable(false);
	boat->setSpeedMult(1.5f);
	glm::vec3 boatScale = glm::vec3(5.0f, 5.0f, 5.0f);	// it's a bit too big for our scene, so scale it down
	boat->setScale(&boatScale);
	boat->setNextRotY(180);
	this->vehicle = boat;
	ourModel.emplace_back(boat);

	// Modelo montado
	Model* boatMounted = new Model("KA/Models/Vehicles/boatmounted.fbx", main->cameraDetails);
	boatMounted->setScale(&boatScale);

	boat->setMountedModel(boatMounted);
	boat->setEmptyModel(boat);

	ourModel.emplace_back(boatMounted);


	// PYRAMID
	float y_pyramid = terreno->Superficie(165, -240) - 5;
	Model *pyr= new Model("KA/Models/Structures/PyramidInside.fbx", main->cameraDetails);
	translate = glm::vec3(165.0f, y_pyramid, -240.0f);
	pyr->ignoreAABB = true;
	pyr->walkeable = false;
	pyr->setTranslate(&translate);
	pyr->setNextTranslate(&translate);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	pyr->setScale(&scale);
	pyr->setNextRotY(45);
	for (auto& attr : *pyr->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(pyr);
	pyr = new Model("KA/Models/Structures/PyramidOutside.fbx", main->cameraDetails);
	pyr->ignoreAABB = true;
	pyr->walkeable = false;
	pyr->setTranslate(&translate);
	pyr->setNextTranslate(&translate);
	for (auto& attr : *pyr->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(pyr);

	// ANGEL
	placeModelAngel(main);


	// TULA
	float y_tula = terreno->Superficie(170, -130);
	Model* tula = new Model("KA/Models/Structures/Tula.fbx", main->cameraDetails);
	translate = glm::vec3(170.0f, y_tula, -130.0f);
	tula->setTranslate(&translate);
	tula->setNextTranslate(&translate);
	scale = glm::vec3(2.0f, 2.0f, 2.0f);	// it's a bit too big for our scene, so scale it down
	tula->setScale(&scale);
	//for (auto& attr : *tula->getModelAttributes()) {
	//	attr.hitbox = NULL;
	//}
	ourModel.emplace_back(tula);

	// LIBERTY
	float y_liberty = terreno->Superficie(-225, 235) + 15;
	Model* liberty = new Model("KA/Models/Structures/LibertyStatue.fbx", main->cameraDetails);
	translate = glm::vec3(-225.0f, y_liberty, 235.0f);
	liberty->setTranslate(&translate);
	liberty->setNextTranslate(&translate);
	scale = glm::vec3(4.0f, 4.0f, 4.0f);
	liberty->setScale(&scale);
	liberty->setRotX(90);
	for (auto& attr : *liberty->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(liberty);

	// CNTOWER
	float y_cntower = terreno->Superficie(0, 180)-2.5;
	Model* tower = new Model("KA/Models/Structures/CNTower.fbx", main->cameraDetails);
	translate = glm::vec3(0.0f, y_cntower, 180.0f);
	tower->setTranslate(&translate);
	tower->setNextTranslate(&translate);
	scale = glm::vec3(4.0f, 4.0f, 4.0f);
	tower->setScale(&scale);
	tower->setRotX(-90);
	tower->setNextRotX(-90);
	for (auto& attr : *tower->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(tower);

	// STADIUM
	float y_stadium = terreno->Superficie(250, 200) - 50;
	Model* stadium = new Model("KA/Models/Structures/stadium.obj", main->cameraDetails);
	translate = glm::vec3(250, y_stadium, 200.0f);
	stadium->setTranslate(&translate);
	stadium->setNextTranslate(&translate);
	scale = glm::vec3(5.0f, 7.0f, 5.0f);
	stadium->setScale(&scale);
	ourModel.emplace_back(stadium);


	// -------------------------------
	// -----   LOAD BILLBOARDS   -----
	// -------------------------------
	// Letrero Bienvenida
	float y_sign = terreno->Superficie(-140, -180) + 1;
	billBoard.emplace_back( new Billboard((WCHAR*)L"KA/Billboards/Signs/Welcome.png", 3, 3, -140, y_sign, -180, camara->cameraDetails)
	);
	// Letrero Monedas
	y_sign = terreno->Superficie(-220, -220) + 1;
	billBoard.emplace_back(new Billboard((WCHAR*)L"KA/Billboards/Signs/SignCoin.png", 3, 3, -220, y_sign, -220, camara->cameraDetails)
	);
	// Letrero Fun Fact #1
	y_sign = terreno->Superficie(-140, -195) + 1;
	billBoard.emplace_back(new Billboard((WCHAR*)L"KA/Billboards/FF/FF1.png", 3, 3, -140, y_sign, -195, camara->cameraDetails)
	);
	// Letrero Fun Fact #2
	y_sign = terreno->Superficie(240, -160) + 1;
	billBoard.emplace_back(new Billboard((WCHAR*)L"KA/Billboards/FF/FF2.png", 3, 3, 240, y_sign, -160, camara->cameraDetails)
	);

	// árbol 1
	float y_tree = terreno->Superficie(10, 15);
	Billboard* arbol = new Billboard((WCHAR*)L"KA/Billboards/Trees/Tree1.png", 12, 12, 10, y_tree+5, 15, camara->cameraDetails);
	billBoard.emplace_back(arbol);
	// árbol 2
	y_tree = terreno->Superficie(22, -14);
	arbol = new Billboard((WCHAR*)L"KA/Billboards/Trees/Tree2.png", 12, 12, 22, y_tree+5, -14, camara->cameraDetails);
	billBoard.emplace_back(arbol);
	// árbol 3
	y_tree = terreno->Superficie(-5, -15);
	arbol = new Billboard((WCHAR*)L"KA/Billboards/Trees/Tree3.png", 12, 12, -5, y_tree+5, -15, camara->cameraDetails);
	billBoard.emplace_back(arbol);
	// árbol 4
	y_tree = terreno->Superficie(-15, 0);
	arbol = new Billboard((WCHAR*)L"KA/Billboards/Trees/Tree4.png", 12, 12, -15, y_tree+5, 0, camara->cameraDetails);
	billBoard.emplace_back(arbol);
	// árbol 5
	y_tree = terreno->Superficie(0, 5);
	arbol = new Billboard((WCHAR*)L"KA/Billboards/Trees/Tree5.png", 12, 12, 0, y_tree+5, 5, camara->cameraDetails);
	billBoard.emplace_back(arbol);
	


	// -------------------------------
	// -----  LOAD WORLDBORDERS  -----
	// -------------------------------
	Model* worldBorder = new CollitionBox(0.0f, 0.0f, 400.0f, 400.0f, 50.0f, 10.0f, main->cameraDetails);
	worldBorder->setRotY(0);
	worldBorder->setNextRotY(0);
	ourModel.emplace_back(worldBorder);
	worldBorder = new CollitionBox(400.0f, 0.0f, 0.0f, 400.0f, 50.0f, 10.0f, main->cameraDetails);
	worldBorder->setRotY(90);
	worldBorder->setNextRotY(90);
	ourModel.emplace_back(worldBorder);
	worldBorder = new CollitionBox(0.0f, 0.0f, -400.0f, 400.0f, 50.0f, 10.0f, main->cameraDetails);
	worldBorder->setRotY(0);
	worldBorder->setNextRotY(0);
	ourModel.emplace_back(worldBorder);
	worldBorder = new CollitionBox(-400.0f, 0.0f, 0.0f, 400.0f, 50.0f, 10.0f, main->cameraDetails);
	worldBorder->setRotY(90);
	worldBorder->setNextRotY(90);
	ourModel.emplace_back(worldBorder);


	// -------------------------------
	// -----     LOAD OTHERS     -----
	// -------------------------------
	
	glm::vec3 posCoin = glm::vec3(155, terreno->Superficie(155, -110) + 2, -110);
	Coin* coin = new Coin("KA/Models/Deco/Coin.fbx", main->cameraDetails);
	coin->setTranslate(&posCoin);
	coin->setNextTranslate(&posCoin);
	scale = glm::vec3(5, 5, 5);
	coin->setScale(&scale);
	for (auto& attr : *coin->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(coin);
	try {
		std::vector<Animation> animations = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::vector<Animation> animation = Animation::loadAllAnimations( "KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::move(animation.begin(), animation.end(), std::back_inserter(animations));
		for (Animation animation : animations)
			coin->setAnimator(Animator(animation));
		coin->setAnimation(1);
	}
	catch (...) {
		ERRORL("Could not load spin animation!", "ANIMACION COIN");
	}

	posCoin = glm::vec3(270, terreno->Superficie(270, -190) + 2, -190);
	coin = new Coin("KA/Models/Deco/Coin.fbx", main->cameraDetails);
	coin->setTranslate(&posCoin);
	coin->setNextTranslate(&posCoin);
	scale = glm::vec3(5, 5, 5);
	coin->setScale(&scale);
	for (auto& attr : *coin->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(coin);
	try {
		std::vector<Animation> animations = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::vector<Animation> animation = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::move(animation.begin(), animation.end(), std::back_inserter(animations));
		for (Animation animation : animations)
			coin->setAnimator(Animator(animation));
		coin->setAnimation(1);
	}
	catch (...) {
		ERRORL("Could not load spin animation!", "ANIMACION COIN");
	}

	posCoin = glm::vec3(165, terreno->Superficie(165, -240) + 2, -240);
	coin = new Coin("KA/Models/Deco/Coin.fbx", main->cameraDetails);
	coin->setTranslate(&posCoin);
	coin->setNextTranslate(&posCoin);
	scale = glm::vec3(5, 5, 5);
	coin->setScale(&scale);
	for (auto& attr : *coin->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(coin);
	try {
		std::vector<Animation> animations = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::vector<Animation> animation = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::move(animation.begin(), animation.end(), std::back_inserter(animations));
		for (Animation animation : animations)
			coin->setAnimator(Animator(animation));
		coin->setAnimation(1);
	}
	catch (...) {
		ERRORL("Could not load spin animation!", "ANIMACION COIN");
	}

	posCoin = glm::vec3(10, terreno->Superficie(10, -5) + 2, -5);
	coin = new Coin("KA/Models/Deco/Coin.fbx", main->cameraDetails);
	coin->setTranslate(&posCoin);
	coin->setNextTranslate(&posCoin);
	scale = glm::vec3(5, 5, 5);
	coin->setScale(&scale);
	coin->setNextRotY(180);
	for (auto& attr : *coin->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(coin);
	try {
		std::vector<Animation> animations = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::vector<Animation> animation = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::move(animation.begin(), animation.end(), std::back_inserter(animations));
		for (Animation animation : animations)
			coin->setAnimator(Animator(animation));
		coin->setAnimation(1);
	}
	catch (...) {
		ERRORL("Could not load spin animation!", "ANIMACION COIN");
	}

	posCoin = glm::vec3(-230, terreno->Superficie(-230, -230) + 2, -230);
	coin = new Coin("KA/Models/Deco/Coin.fbx", main->cameraDetails);
	coin->setTranslate(&posCoin);
	coin->setNextTranslate(&posCoin);
	scale = glm::vec3(5, 5, 5);
	coin->setScale(&scale);
	coin->setNextRotY(90);
	for (auto& attr : *coin->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(coin);
	try {
		std::vector<Animation> animations = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::vector<Animation> animation = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::move(animation.begin(), animation.end(), std::back_inserter(animations));
		for (Animation animation : animations)
			coin->setAnimator(Animator(animation));
		coin->setAnimation(1);
	}
	catch (...) {
		ERRORL("Could not load spin animation!", "ANIMACION COIN");
	}

	posCoin = glm::vec3(165, terreno->Superficie(165, 190) + 2, 190);
	coin = new Coin("KA/Models/Deco/Coin.fbx", main->cameraDetails);
	coin->setTranslate(&posCoin);
	coin->setNextTranslate(&posCoin);
	scale = glm::vec3(5, 5, 5);
	coin->setScale(&scale);
	for (auto& attr : *coin->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(coin);
	try {
		std::vector<Animation> animations = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::vector<Animation> animation = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::move(animation.begin(), animation.end(), std::back_inserter(animations));
		for (Animation animation : animations)
			coin->setAnimator(Animator(animation));
		coin->setAnimation(1);
	}
	catch (...) {
		ERRORL("Could not load spin animation!", "ANIMACION COIN");
	}

	posCoin = glm::vec3(-210, terreno->Superficie(-210, 215) + 2, 215);
	coin = new Coin("KA/Models/Deco/Coin.fbx", main->cameraDetails);
	coin->setTranslate(&posCoin);
	coin->setNextTranslate(&posCoin);
	scale = glm::vec3(5, 5, 5);
	coin->setScale(&scale);
	for (auto& attr : *coin->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(coin);
	try {
		std::vector<Animation> animations = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::vector<Animation> animation = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::move(animation.begin(), animation.end(), std::back_inserter(animations));
		for (Animation animation : animations)
			coin->setAnimator(Animator(animation));
		coin->setAnimation(1);
	}
	catch (...) {
		ERRORL("Could not load spin animation!", "ANIMACION COIN");
	}

	posCoin = glm::vec3(-20, terreno->Superficie(-20, -185) + 2, -185);
	coin = new Coin("KA/Models/Deco/Coin.fbx", main->cameraDetails);
	coin->setTranslate(&posCoin);
	coin->setNextTranslate(&posCoin);
	scale = glm::vec3(5, 5, 5);
	coin->setScale(&scale);
	for (auto& attr : *coin->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(coin);
	try {
		std::vector<Animation> animations = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::vector<Animation> animation = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::move(animation.begin(), animation.end(), std::back_inserter(animations));
		for (Animation animation : animations)
			coin->setAnimator(Animator(animation));
		coin->setAnimation(1);
	}
	catch (...) {
		ERRORL("Could not load spin animation!", "ANIMACION COIN");
	}

	posCoin = glm::vec3(35, terreno->Superficie(35, 170) + 2, 170);
	coin = new Coin("KA/Models/Deco/Coin.fbx", main->cameraDetails);
	coin->setTranslate(&posCoin);
	coin->setNextTranslate(&posCoin);
	scale = glm::vec3(5, 5, 5);
	coin->setScale(&scale);
	for (auto& attr : *coin->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(coin);
	try {
		std::vector<Animation> animations = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::vector<Animation> animation = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::move(animation.begin(), animation.end(), std::back_inserter(animations));
		for (Animation animation : animations)
			coin->setAnimator(Animator(animation));
		coin->setAnimation(1);
	}
	catch (...) {
		ERRORL("Could not load spin animation!", "ANIMACION COIN");
	}

	posCoin = glm::vec3(-25, terreno->Superficie(-25, 290) + 2, 290);
	coin = new Coin("KA/Models/Deco/Coin.fbx", main->cameraDetails);
	coin->setTranslate(&posCoin);
	coin->setNextTranslate(&posCoin);
	scale = glm::vec3(5, 5, 5);
	coin->setScale(&scale);
	for (auto& attr : *coin->getModelAttributes()) {
		attr.hitbox = NULL;
	}
	ourModel.emplace_back(coin);
	try {
		std::vector<Animation> animations = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::vector<Animation> animation = Animation::loadAllAnimations("KA/Animations/Deco/Coin_Spin.fbx", coin->GetBoneInfoMap(), coin->getBonesInfo(), coin->GetBoneCount());
		std::move(animation.begin(), animation.end(), std::back_inserter(animations));
		for (Animation animation : animations)
			coin->setAnimator(Animator(animation));
		coin->setAnimation(1);
	}
	catch (...) {
		ERRORL("Could not load spin animation!", "ANIMACION COIN");
	}

	
}

	void Scenario::placeModelAngel(Model* main)
	{
		float y_angel = terreno->Superficie(260, -200);
		Model* angel = new Model("KA/Models/Structures/IndependenceAngel.fbx", main->cameraDetails);
		glm::vec3 translate = glm::vec3(260.0f, y_angel, -200.0f);
		angel->setTranslate(&translate);
		angel->setNextTranslate(&translate);
		glm::vec3 scale = glm::vec3(2.0f, 2.0f, 2.0f);	// it's a bit too big for our scene, so scale it down
		angel->setScale(&scale);
		for (auto& attr : *angel->getModelAttributes()) {
			attr.hitbox = NULL;
		}
		ourModel.emplace_back(angel);

		translate = glm::vec3(260.0f, y_angel + 0.25, -200.0f);
		int mainhitboxrot = 45;
		int hitboxrot1 = 0;
		int hitboxrot2 = 0;
		Model* hitBox = NULL;

		// HITBOX FIRST LAYER
		glm::vec3 hitboxTrans = translate;
		for (int i = 1; i <= 16; i++) {
			hitboxTrans = glm::vec3(translate.x, translate.y - 0.2, translate.z);
			hitBox = new CollitionBox(hitboxTrans.x, hitboxTrans.y, hitboxTrans.z, 29.0f, 0.5f, 3.5f, main->cameraDetails);
			hitBox->walkeable = true;
			hitBox->setRotY(hitboxrot1);
			hitBox->setNextRotY(hitboxrot1);
			hitBox->setTranslate(&hitboxTrans);
			hitBox->setNextTranslate(&hitboxTrans);
			hitBox->ignoreAABB = true;
			ourModel.emplace_back(hitBox);

			hitboxrot1 += 24;
		}

		// HITBOX STAIRS
		for (int j = 1; j <= 30; j++) {

			hitboxTrans = glm::vec3(translate.x, translate.y + 0.5, translate.z);
			hitBox = new CollitionBox(hitboxTrans.x, hitboxTrans.y, hitboxTrans.z, 26.4f, 0.22f, 3.0f, main->cameraDetails);
			hitBox->walkeable = true;
			hitBox->setRotY(hitboxrot2);
			hitBox->setNextRotY(hitboxrot2);
			hitBox->setTranslate(&hitboxTrans);
			hitBox->setNextTranslate(&hitboxTrans);
			hitBox->ignoreAABB = true;
			ourModel.emplace_back(hitBox);

			hitboxTrans = glm::vec3(translate.x, translate.y + 0.94, translate.z);
			hitBox = new CollitionBox(hitboxTrans.x, hitboxTrans.y, hitboxTrans.z, 25.6f, 0.22f, 2.9f, main->cameraDetails);
			hitBox->walkeable = true;
			hitBox->setRotY(hitboxrot2);
			hitBox->setNextRotY(hitboxrot2);
			hitBox->setTranslate(&hitboxTrans);
			hitBox->setNextTranslate(&hitboxTrans);
			hitBox->ignoreAABB = true;
			ourModel.emplace_back(hitBox);

			hitboxTrans = glm::vec3(translate.x, translate.y + 1.38, translate.z);
			hitBox = new CollitionBox(hitboxTrans.x, hitboxTrans.y, hitboxTrans.z, 24.8f, 0.22f, 2.8f, main->cameraDetails);
			hitBox->walkeable = true;
			hitBox->setRotY(hitboxrot2);
			hitBox->setNextRotY(hitboxrot2);
			hitBox->setTranslate(&hitboxTrans);
			hitBox->setNextTranslate(&hitboxTrans);
			hitBox->ignoreAABB = true;
			ourModel.emplace_back(hitBox);

			hitboxTrans = glm::vec3(translate.x, translate.y + 1.82, translate.z);
			hitBox = new CollitionBox(hitboxTrans.x, hitboxTrans.y, hitboxTrans.z, 24.0f, 0.22f, 2.7f, main->cameraDetails);
			hitBox->walkeable = true;
			hitBox->setRotY(hitboxrot2);
			hitBox->setNextRotY(hitboxrot2);
			hitBox->setTranslate(&hitboxTrans);
			hitBox->setNextTranslate(&hitboxTrans);
			hitBox->ignoreAABB = true;
			ourModel.emplace_back(hitBox);

			hitboxTrans = glm::vec3(translate.x, translate.y + 2.26, translate.z);
			hitBox = new CollitionBox(hitboxTrans.x, hitboxTrans.y, hitboxTrans.z, 23.2f, 0.22f, 2.6f, main->cameraDetails);
			hitBox->walkeable = true;
			hitBox->setRotY(hitboxrot2);
			hitBox->setNextRotY(hitboxrot2);
			hitBox->setTranslate(&hitboxTrans);
			hitBox->setNextTranslate(&hitboxTrans);
			hitBox->ignoreAABB = true;
			ourModel.emplace_back(hitBox);

			hitboxrot2 += 12;
		}

		// HITBOX AT CENTER
		glm::vec3 mainHitboxTrans = translate;
		mainHitboxTrans = glm::vec3(translate.x, translate.y + 2.5, translate.z);
		hitBox = new CollitionBox(mainHitboxTrans.x, mainHitboxTrans.y, mainHitboxTrans.z, 10.0f, 7.0f, 10.0f, main->cameraDetails);
		hitBox->walkeable = true;
		hitBox->setRotY(mainhitboxrot);
		hitBox->setNextRotY(mainhitboxrot);
		hitBox->setTranslate(&mainHitboxTrans);
		hitBox->setNextTranslate(&mainHitboxTrans);
		hitBox->ignoreAABB = true;
		ourModel.emplace_back(hitBox);
	}



	//el metodo render toma el dispositivo sobre el cual va a dibujar
	//y hace su tarea ya conocida
Scene* Scenario::Render() {
	//borramos el biffer de color y el z para el control de profundidad a la 
	//hora del render a nivel pixel.
    // Obtener información de iluminación ANTES de limpiar
    glm::vec3 ambient = sky->getAmbientLight();
    glm::vec3 diffuse = sky->getDiffuseLight();
    glm::vec3 sunDir = sky->getSunDirection();

    // Color de fondo basado en iluminación ambiental
    glm::vec3 clearColor = ambient * 0.9f;
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
//	glClearColor(255.0f, 255.0f, 255.0f, 255.0f);

	if (this->animacion > 10) { // Timer se ejecuta cada 1000/30 = 33.333 ms
		for (BillboardAnimation *b : billBoardAnim){
			b->nextAnimation();
		}
		this->animacion = 0;
	} else {
		animacion = animacion + (1 * gameTime.deltaTime/100);
	}
	Axolotl* axo = getMainAxolotl();
	if (axo) {
		axo->updateOxygenByPosition(gameTime.deltaTime/100);
		axo->updateLocatorPosition();
	}
	for (auto& m : ourModel) {
		Coin* c = dynamic_cast<Coin*>(m);
		if (c && !c->isTaken()) {

			glm::vec3 a = *axo->getTranslate();
			glm::vec3 b = *c->getTranslate();

			float d = glm::distance(a, b);

			if (d < 4.0f) { // radio de recogida
				c->take();
				c->setScale(new glm::vec3(0, 0, 0));  // desaparecer
				axo->setPoints(axo->getPoints() + c->getValue());
			}
		}
	}
	// Decimos que dibuje la media esfera
	sky->Draw();
	// Ahora el terreno
	terreno->Draw();
	water->Draw();
	// Dibujamos cada billboard que este cargado en el arreglo de billboards.
	for (int i = 0; i < billBoard.size(); i++)
		billBoard[i]->Draw();
	for (int i = 0; i < billBoardAnim.size(); i++)
		billBoardAnim[i]->Draw();
	for (int i = 0; i < billBoard2D.size(); i++)
		billBoard2D[i]->Draw();
	// Dibujamos cada modelo que este cargado en nuestro arreglo de modelos
	for (int i = 0; i < ourModel.size(); i++) {
    // APLICAR ILUMINACIÓN DINÁMICA A TODOS LOS MODELOS
        if (ourModel[i]->gpuDemo != NULL) {
            ourModel[i]->gpuDemo->use();

            // Pasar información de iluminación dinámica
            ourModel[i]->gpuDemo->setVec3("dirLight.ambient", ambient);
            ourModel[i]->gpuDemo->setVec3("dirLight.diffuse", diffuse);
            ourModel[i]->gpuDemo->setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            ourModel[i]->gpuDemo->setVec3("dirLight.direction", -sunDir);

            // Pasar hora del día
            ourModel[i]->gpuDemo->setFloat("timeOfDay", sky->getTimeOfDay());
            ourModel[i]->gpuDemo->setBool("useDynamicLighting", true);

            ourModel[i]->gpuDemo->desuse();
        }
			ourModel[i]->Draw();
	}
	for (int i = 0; i < ourText.size(); i++) {
		ourText[i]->Draw();
	}
		// Le decimos a winapi que haga el update en la ventana
	return this;
}
	
std::vector<Model*> *Scenario::getLoadedModels() {
	return &ourModel;
}
std::vector<Billboard*> *Scenario::getLoadedBillboards() {
	return &billBoard;
}
std::vector<Billboard2D*> *Scenario::getLoadedBillboards2D(){
	return &billBoard2D;
}
std::vector<Texto*> *Scenario::getLoadedText(){
	return &ourText;
}
std::vector<BillboardAnimation*> *Scenario::getLoadedBillboardsAnimation(){
	return &billBoardAnim;
}

Model* Scenario::getMainModel() {
	return this->camara;
}
Axolotl* Scenario::getMainAxolotl() {
	return dynamic_cast<Axolotl*>(this->camara);
}
void Scenario::setMainModel(Model* mainModel){
	this->camara = mainModel;
}
float Scenario::getAngulo() {
	return this->angulo;
}
void Scenario::setAngulo(float angulo) {
	this->angulo = angulo;
}
SkyDome* Scenario::getSky() {
	return sky;
}
Terreno* Scenario::getTerreno() {
	return terreno;
}

Scenario::~Scenario() {
	if (this->sky != NULL) {
		delete this->sky;
		this->sky = NULL;
	}
	if (this->terreno != NULL) {
		delete this->terreno;
		this->terreno = NULL;
	}
	if (billBoard.size() > 0)
		for (int i = 0; i < billBoard.size(); i++)
			delete billBoard[i];
	if (billBoardAnim.size() > 0)
		for (int i = 0; i < billBoardAnim.size(); i++)
			delete billBoardAnim[i];
	if (billBoard2D.size() > 0)
		for (int i = 0; i < billBoard2D.size(); i++)
			delete billBoard2D[i];
	this->billBoard.clear();
	if (ourText.size() > 0)
		for (int i = 0; i < ourText.size(); i++)
			if (!(ourText[i]->name.compare("FPSCounter") || ourText[i]->name.compare("Coordenadas")))
				delete ourText[i];
	this->ourText.clear();
	if (ourModel.size() > 0)
		for (int i = 0; i < ourModel.size(); i++)
			if (ourModel[i] != camara)
			delete ourModel[i];
	this->ourModel.clear();
}
