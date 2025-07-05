#pragma once
#include <string>;

class SaveFile
{
private:
	bool beatLevels[50];
	int bestTimes[50];
	int bestScores[50];

	int levelCount;
public:
	SaveFile(int levelCount);

	void setLevelBestTime(int level, int time);
	void setLevelBestScore(int level, int score);
	void beatLevel(int level);

	bool hasBeatLevel(int level);
	int getLevelScore(int level);
	int getLevelTime(int level);

	std::string toString();
};

