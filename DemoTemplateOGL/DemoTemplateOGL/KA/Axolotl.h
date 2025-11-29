#pragma once

#include "../Base/Model.h"
#include "../Billboard2D.h"
#include "../Texto.h"

class Axolotl : public Model {
private:
	std::vector<Billboard2D*> hearts;
	int vidas = 3;
	float salud = 100;
	float oxigeno = 100;
	float puntos = 0;
	Texto* saludTexto;
	Texto* oxigenoTexto;
	Texto* puntosTexto;
	Billboard2D* healthBarBot;
	Billboard2D* healthBarTop;
	Billboard2D* oxygenBarBot;
	Billboard2D* oxygenBarTop;
	Billboard2D* puntosBarBot;
	Billboard2D* puntosBarTop;
	Billboard2D* gameOverBillboard;
	Billboard2D* gameWinBillboard;
	Billboard2D* locator;
	Billboard2D* minimap;
	bool gameOverVisible = false;
	bool gameWinVisible = false;

public:
	Axolotl() {
		showHearts();
		showHP();
		showOX();
		showPT();
		initTextDisplay();
		initMiniMap();
		gameOver();
		gameWin();
	}
	Axolotl(vector<Vertex>& vertices, unsigned int numVertices, vector<unsigned int>& indices, unsigned int numIndices, Camera* camera)
		: Model(vertices, numVertices, indices, numIndices, camera) {
		showHearts();
		showHP();
		showOX();
		showPT();
		initTextDisplay();
		initMiniMap();
		gameOver();
		gameWin();
	}
	Axolotl(string const& path, glm::vec3& actualPosition, Camera* cam, bool rotationX = false, bool rotationY = true, bool gamma = false)
		: Model(path, actualPosition, cam, rotationX, rotationY, gamma) {
		showHearts();
		showHP();
		showOX();
		showPT();
		initTextDisplay();
		initMiniMap();
		gameOver();
		gameWin();
	}
	Axolotl(string const& path, Camera* camera, bool rotationX = false, bool rotationY = true, bool gamma = false)
		: Model(path, camera, rotationX, rotationY, gamma) {
		showHearts();
		showHP();
		showOX();
		showPT();
		initTextDisplay();
		initMiniMap();
		gameOver();
		gameWin();
	}

	//Getters and Setters
	//VIDA
	float getHealth() const {
		return salud;
	}
	void setHealth(float newHealth) {
		salud = newHealth;
		updateHP();
	}
	//OXIGENO
	float getOxygen() const {
		return oxigeno;
	}
	void setOxygen(float newOxygen) {
		oxigeno = newOxygen;
		updateOX();
	}
	//PUNTOS
	float getPoints() const {
		return puntos;
	}
	void setPoints(float newPuntos) {
		puntos = newPuntos;
		updatePT();
	}
	
	//Mostrar billboards
	//CORAZONES
	void showHearts() {
		// Clear existing hearts
		for (auto heart : hearts) {
			delete heart;
		}
		hearts.clear();

		glm::vec3 scale;
		int heartposx = 100;

		for (int i = 0; i < vidas; i++) {
			hearts.emplace_back(new Billboard2D((WCHAR*)L"KA/Billboards/Axo.png", 2, 2, heartposx, 100, 0, this->cameraDetails));
			scale = glm::vec3(125.0f, 125.0f, 0.0f);
			hearts.back()->setScale(&scale);
			heartposx += 125;
		}
	}
	void showHP() {
		glm::vec3 hpscale;
		healthBarBot = new Billboard2D((WCHAR*)L"KA/GUI/Bars/Graybar.png", 2, 2, 220, 200, 0, this->cameraDetails);
		healthBarTop = new Billboard2D((WCHAR*)L"KA/GUI/Bars/HPbar.png", 2, 2, 220, 200, 0, this->cameraDetails);
		hpscale = glm::vec3(320.0f, 32.0f, 0.0f);
		healthBarBot->setScale(&hpscale);
		healthBarTop->setScale(&hpscale);
	}
	void showOX() {
		glm::vec3 oxscale;
		oxygenBarBot = new Billboard2D((WCHAR*)L"KA/GUI/Bars/Graybar.png", 2, 2, 220, 250, 0, this->cameraDetails);
		oxygenBarTop = new Billboard2D((WCHAR*)L"KA/GUI/Bars/OXbar.png", 2, 2, 220, 250, 0, this->cameraDetails);
		oxscale = glm::vec3(320.0f, 32.0f, 0.0f);
		oxygenBarBot->setScale(&oxscale);
		oxygenBarTop->setScale(&oxscale);
	}
	void showPT() {
		glm::vec3 ptscale1, ptscale2;
		puntosBarBot = new Billboard2D((WCHAR*)L"KA/GUI/Bars/Graybar.png", 2, 2, ((SCR_WIDTH + 200) / 2), 40, 0, this->cameraDetails);
		puntosBarTop = new Billboard2D((WCHAR*)L"KA/GUI/Bars/PTbar.png", 2, 2, ((SCR_WIDTH+200) / 2), 40, 0, this->cameraDetails);
		ptscale1 = glm::vec3(320.0f, 32.0f, 0.0f);
		ptscale2 = glm::vec3(0.0f, 32.0f, 0.0f);
		puntosBarBot->setScale(&ptscale1);
		puntosBarTop->setScale(&ptscale2);
	}

	void reduceHealth(float damage) {
		salud -= damage; // Decrease health by damage amount
		if (salud <= 0) {
			deductLife(); // Deduct a life if health is 0 or below
		}
		if (salud > 100) {
			setHealth(100);
		}
		updateHP();
	}

	void deductLife() {
		if (vidas > 0) {
			vidas--; // Decrease the number of lives
			showHearts();
			if (vidas > 0) {
				string msg = "Perdiste una vida, pero te quedan " + to_string(vidas);
				INFO(msg, "CONTINUE");
			}
			else {
				INFO("Perdiste tu última vida", "CONTINUE");
			}
			salud = 100; // Reset health after losing a life
			oxigeno = 100; // Reset oxygen after losing a life
			updateHP();
			updateOX();
		}
		if (vidas <= 0) {
			showGameOver(); // Show Game Over when lives reach 0
		}
	}

	void updateHP() {
		// Calculate health percentage
		float hpPercent = salud / 100.0f;
		glm::vec3 scale = glm::vec3(320.0f * hpPercent, 32.0f, 0.0f);
		healthBarTop->setScale(&scale);
		updateHPDisplay();
	}

	void updateOX() {
		//Calculate Oxygen percentage
		float oxPercent = oxigeno / 100.0f;
		glm::vec3 scale = glm::vec3(320.0f * oxPercent, 32.0f, 0.0f);
		oxygenBarTop->setScale(&scale);
		updateOXDisplay();
	}

	void updatePT() {
		//Calculate progress percentage
		float ptPercent = puntos / 10.0f;
		glm::vec3 scale = glm::vec3(320.0f * ptPercent, 32.0f, 0.0f);
		puntosBarTop->setScale(&scale);
		updatePTDisplay();
		if ((puntos/10.0f) == 1) {
			showGameWin();
		}
	}

	void initTextDisplay() {
		std::wstring initialText1 = L"Puntos: 0";
		puntosTexto = new Texto(initialText1, 20.0f, 0.0f, (SCR_WIDTH)/2, 30.0f, 0.0f, this);
		updatePTDisplay();
		std::wstring initialText2 = L"100/100";
		saludTexto = new Texto(initialText2, 20.0f, 0.0f, 150.0f, 190.0f, 0.0f, this);
		updateHPDisplay();
		std::wstring initialText3 = L"100/100";
		oxigenoTexto = new Texto(initialText3, 20.0f, 0.0f, 150.0f, 240.0f, 0.0f, this);
		updateOXDisplay();
	}

	void initMiniMap() {
		// Crear Locator
		locator = new Billboard2D(
			(WCHAR*)L"KA/Billboards/Locator.png",
			2.5, 2.5,
			1920 - 250, 250, 0,
			this->cameraDetails
		);
		glm::vec3 locScale = glm::vec3(25, 25, 0);
		locator->setScale(&locScale);

		// Crear Mapa
		minimap = new Billboard2D(
			(WCHAR*)L"KA/Billboards/Map.png",
			12, 12,
			1920 - 250, 250, 0,
			this->cameraDetails
		);
		glm::vec3 mapScale = glm::vec3(450, 450, 0);
		minimap->setScale(&mapScale);
	}


	void updatePTDisplay() {
		std::wstring updatedText = L"Puntos: " + std::to_wstring(static_cast<int>(puntos));
		if (puntosTexto) {
			puntosTexto->initTexto(updatedText); // Update the text
		}
	}

	void updateHPDisplay() {
		std::wstring updatedText = std::to_wstring(static_cast<int>(salud)) + L"/100";
		if (saludTexto) {
			saludTexto->initTexto(updatedText); // Update the text
		}
	}

	void updateOXDisplay() {
		std::wstring updatedText = std::to_wstring(static_cast<int>(oxigeno)) + L"/100";
		if (oxigenoTexto) {
			oxigenoTexto->initTexto(updatedText); // Update the text
		}
	}

	void updateOxygenByPosition(float deltaTime)
	{
		glm::vec3 pos = *getTranslate();

		// Si está debajo de y10 -> bajo el agua
		if (pos.y < 10.0f)
		{
			oxigeno -= 0.5f * deltaTime;   // Ajusta velocidad según prefieras

			if (oxigeno <= 0.0f)
			{
				oxigeno = 0.0f;
				reduceHealth(0.1f);  // dañarlo si no tiene oxígeno
			}

			updateOX();
		}
		else
		{
			// Opcional: si sale del agua, recupera oxígeno lentamente
			oxigeno += 0.5f * deltaTime;
			if (oxigeno > 100.0f)
				oxigeno = 100.0f;

			updateOX();
		}
	}

	void updateLocatorPosition() {

		glm::vec3 pos = *getTranslate();  // posición del axolotl
		float ax = pos.x;
		float az = pos.z;

		float worldSize = 800.0f;
		float mapPixels = 2048.0f;
		float canvasPixels = 2500.0f;

		float border = (canvasPixels - mapPixels) / 2.0f;
		float mapScale = 450.0f / canvasPixels;

		float normX = (ax + worldSize / 2) / worldSize;
		float normZ = (az + worldSize / 2) / worldSize;

		float px = border + normX * mapPixels;
		float pz = border + normZ * mapPixels;

		px *= mapScale;
		pz *= mapScale;

		float mapCenterX = 1920 - 250;
		float mapCenterY = 250;

		float finalX = mapCenterX - 225 + px;
		float finalY = mapCenterY - 225 + pz;

		glm::vec3 newPos(finalX, finalY, 0);
		locator->setTranslate(&newPos);
	}


	void gameOver() {
		gameOverBillboard = new Billboard2D((WCHAR*)L"KA/GUI/Menus/GameOver.png", 2, 2, (SCR_WIDTH - 200) / 2, (SCR_HEIGHT - 200) / 2, 0, this->cameraDetails); // Centered on screen
		glm::vec3 scale = glm::vec3(1920.0f, 1080.0f, 0.0f);
		gameOverBillboard->setScale(&scale);
	}

	void gameWin() {
		gameWinBillboard = new Billboard2D((WCHAR*)L"KA/GUI/Menus/GameWin.png", 2, 2, (SCR_WIDTH - 200) / 2, (SCR_HEIGHT - 200) / 2, 0, this->cameraDetails); // Centered on screen
		glm::vec3 scale = glm::vec3(1920.0f, 1080.0f, 0.0f);
		gameWinBillboard->setScale(&scale);
	}

	void showGameOver() {
		gameOverVisible = true;
	}

	void showGameWin() {
		gameWinVisible = true;
	}

	~Axolotl() {
		for (int i = 0; i < hearts.size(); i++) {
			delete hearts[i];
		}
		delete healthBarTop;
		delete healthBarBot;
		delete oxygenBarTop;
		delete oxygenBarBot;
		delete puntosBarTop;
		delete puntosBarBot;
		delete gameOverBillboard;
		delete gameWinBillboard;
		delete saludTexto;
		delete oxigenoTexto;
		delete puntosTexto;
	}

	void Draw() {
		if (gameOverVisible) {
			gameOverBillboard->Draw();

			if (MessageBox(NULL, L"No te rindas! tus esfuerzos no han sido en vano. ¿Deseas volver a intentarlo?",
				L"GAME OVER", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK) {
				exit(0);
			}
		}
		if (gameWinVisible) {
			gameWinBillboard->Draw();

			if (MessageBox(NULL, L"Felicidades! Has ganado el Axolotour",
				L"CONGRATS!", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK) {
				exit(0);
			}
		}
		saludTexto->Draw();
		oxigenoTexto->Draw();
		puntosTexto->Draw();

		for (auto vida : hearts) {
			vida->Draw();
		}

		locator->Draw();
		minimap->Draw();

		healthBarTop->Draw();
		healthBarBot->Draw();
		oxygenBarTop->Draw();
		oxygenBarBot->Draw(); 
		puntosBarTop->Draw();
		puntosBarBot->Draw();

		Model::Draw();
	}
};