/*
 * @brief ルーレットクラス
 * @author Sekino
 */
#pragma once
#ifndef _ROULETTE_H_
#define _ROULETTE_H_

class Roulette {
private:
	float angle;
	int controllerPort;
	bool isRouletteStop;
	bool isStop;
	float rotateValue;
	float decelecation;

public:
	Roulette();
	~Roulette() = default;

public:
	void RouletteStart(int _controllerPort);

	void Update(float _t);

	bool IsStop() const { return isRouletteStop; }

	int GetRouletteNum(int _cellCount);

	float GetAngle() const { return angle; }
};

#endif // !_ROULETTE_H_