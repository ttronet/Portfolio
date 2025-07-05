#pragma once
class GameStats
{
private:
	int score;
	int maxScore;
	int timerStartTime; //En segundos
	int timer; //Tambi[en en segundos en efecto
	bool levelAlreadyBeat;
public:
	GameStats(int maxScore,bool levelAlreadyBeat);
	void ateAnApple();
	void levelJustStarted();
	void addSecond();
	void beatLevel(int level);
	
	//Agregar getters
	int getScore();
	int getTimer();
	int getMaxScore();

};

