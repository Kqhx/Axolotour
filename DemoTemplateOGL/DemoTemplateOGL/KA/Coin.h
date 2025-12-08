#pragma once

#include "../Base/Model.h"

class Coin : public Model {
public:
    enum CoinID {
        NONE = 0,
        MONEDA_INICIAL = 1,
        MONEDA_ACUATICA = 2,
        MONEDA_PIRAMIDE = 3,
        MONEDA_ANGEL = 4,
        MONEDA_TULA = 5,
        MONEDA_SECRETA = 6,
        MONEDA_CNTOWER = 7,
        MONEDA_LIBERTY = 8,
        MONEDA_FALLS = 9,
        MONEDA_FINAL = 10
    };

private:
    int valor = 1;
    CoinID id = NONE;
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


    bool isTaken() const { return tomada; }

    int getValue() const { return valor; }

    void setCoinId(CoinID newId) { id = newId; }

    CoinID getCoinId() const { return id; }

    void sendCoinMsg() {
        switch (id) {
            case MONEDA_INICIAL: {
                INFO("¡Felicidades! Encontraste la Moneda Inicial.\nExplora el mapa y reúne las 10 monedas hasta llegar al estadio.", "¡MONEDA RECOGIDA!");
                break;
            }
            case MONEDA_ACUATICA: {
                INFO("Has recogido la Moneda Acuática. ¡No olvides salir a respirar!", "¡MONEDA RECOGIDA!");
                break;
            }
            case MONEDA_PIRAMIDE: {
                INFO("Moneda de Tenochtitlan obtenida. ¡Un vistazo al pasado!", "¡MONEDA RECOGIDA!");
                break;
            }
            case MONEDA_ANGEL: {
                INFO("Recogiste la Moneda del Ángel. ¡Ciudad de México te saluda!", "¡MONEDA RECOGIDA!");
                break;
            }
            case MONEDA_TULA: {
                INFO("Moneda de Tula recogida. Los Atlantes te observan orgullosos.", "¡MONEDA RECOGIDA!");
                break;
            }
            case MONEDA_SECRETA: {
                INFO("¡Encontraste la Moneda Secreta! No muchas personas lo logran.", "¡MONEDA RECOGIDA!");
                break;
            }
            case MONEDA_CNTOWER: {
                INFO("Conseguiste la Moneda de la CNTower. ¡Bienvenido al norte!", "¡MONEDA RECOGIDA!");
                break;
            }
            case MONEDA_LIBERTY: {
                INFO("Has obtenido la Moneda de la Libertad. ¡Disfruta la vista!", "¡MONEDA RECOGIDA!");
                break;
            }
            case MONEDA_FALLS: {
                INFO("Moneda de las Cataratas: ¡La naturaleza premia a quienes exploran!", "¡MONEDA RECOGIDA!");
                break;
            }
            case MONEDA_FINAL: {
                INFO("¡Has obtenido la Moneda Final! Axolotour World Cup 2026 te llama\nPara completar tu viaje recoge todas las monedas.", "¡MONEDA RECOGIDA!");
                break;
            }
            case NONE: {}
            default: {
                INFO("Has recogido una moneda misteriosa...", "¡MONEDA!");
                break;
            }
        }
    }

    void take() {
        tomada = true;
        sendCoinMsg();
    }
};