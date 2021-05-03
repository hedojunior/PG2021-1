#include "CoinManager.h"

bool CoinManager::shouldDropCoin()
{
	return coinTimer->getTimeInSeconds() - lastSpawnInSeconds >= coinSpawnInterval;
}

bool CoinManager::lastCoinStillFalling()
{
	return coins.front()->isFalling;
}

void CoinManager::dropCoin()
{
	//Aumenta a dificuldade
	if (fallenCoins > 0 && fallenCoins % 5 == 0 && coinFallingSpeedFactor <= 2.4)
	{
		coinFallingSpeedFactor = coinFallingSpeedFactor + 0.2;
	}

	//Inicia a animação e a queda da moeda
	Coin* coin = coins[totalCoins - 1 - fallenCoins];
	coin->startFalling(coinFallingSpeedFactor);

	fallenCoins += 1;
}

void CoinManager::checkForCollections(Sprite *collector)
{
	if (fallenCoins > 0 && fallenCoins <= totalCoins)
	{
		for (int i = 0; i < fallenCoins; i++)
		{
			int index = totalCoins - 1 - i;
			Coin* coin = coins[index];

			if (coin->isFalling && collector->collidesWith(coin) && !coin->isCollected)
			{
				coin->isCollected = true;
				collectedCoins += 1;
			}
		}
	}
}

void CoinManager::render()
{
	for (int i = 0; i < coins.size(); i++)
	{
		coins[i]->update();
		coins[i]->draw();
	}
}

void CoinManager::generateCoins(SpriteFactory* factory, Shader* shader)
{
	for (int i = 0; i <= totalCoins; i++)
	{
		Coin* coin = factory->coin(shader);
		coins.push_back(coin);
	}
};