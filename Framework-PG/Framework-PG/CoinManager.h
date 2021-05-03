#pragma once

#include "Coin.h"
#include "Timer.h"
#include "Sprite.h"
#include "SpriteFactory.h"
#include "Shader.h"

#include <vector>

class CoinManager
{
public:
	CoinManager(int totalCoins)
	{
		this->totalCoins = totalCoins;
		coinTimer = new Timer;
		coinSpawnInterval = 2;
		lastSpawnInSeconds = 0;
		coinFallingSpeedFactor = 1;
		collectedCoins = 0;
		fallenCoins = 0;
	};

	void start() { coinTimer->start(); }
	void markSpawnTime() { lastSpawnInSeconds = coinTimer->getTimeInSeconds(); }

	void dropCoin();
	void checkForCollections(Sprite* collector);
	void render();
	void generateCoins(SpriteFactory* factory, Shader* shader);

	bool allCoinsAreGone() { return fallenCoins == totalCoins; }

	bool shouldDropCoin();
	bool lastCoinStillFalling();

	int getCollectedCoins() { return collectedCoins; }
	int getTotalCoins() { return totalCoins;  }

private:
	//Timer para controle de spawn
	Timer *coinTimer;

	// Total de moedas que serão geradas
	int totalCoins;

	//Intervalo entre a descida de cada moeda, em segundos
	double coinSpawnInterval;

	//Tempo do spawn da moeda anterior
	double lastSpawnInSeconds;

	//Fator de multiplicação para descida da moeda (incrementado a cada 5 moedas para aumento de dificuldade)
	double coinFallingSpeedFactor;

	//Contador de moedas coletadas
	int collectedCoins;

	//Contador de moedas que já cairam
	int fallenCoins;

	//Vetor para armazenamento das moedas
	vector <Coin*> coins;
};