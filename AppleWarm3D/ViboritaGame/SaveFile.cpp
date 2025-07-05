#include "SaveFile.h"

SaveFile::SaveFile(int levelCount)
{
	this->levelCount = levelCount;
	for (int i = 0; i < levelCount;i++) {
		bestTimes[i] = std::numeric_limits<int>::max();
		bestScores[i] = 0;
		beatLevels[i] = false;
	}
}

void SaveFile::setLevelBestTime(int level, int time)
{
	if (level >= levelCount || time > bestTimes[level])
		return;

	bestTimes[level] = time;
}

void SaveFile::setLevelBestScore(int level, int score)
{
	if (level >= levelCount || score < bestScores[level])
		return;

	bestScores[level] = score;
}

void SaveFile::beatLevel(int level)
{
	if (level >= levelCount)
		return;

	beatLevels[level] = true;
}

bool SaveFile::hasBeatLevel(int level)
{
	return level < levelCount && beatLevels[level];
}

int SaveFile::getLevelScore(int level)
{
	return level < levelCount ? bestScores[level] : 0;
}

int SaveFile::getLevelTime(int level)
{
	return level < levelCount ? bestTimes[level] : 0;
}

std::string SaveFile::toString()
{
	std::string beatLevelsLine = "";
	std::string levelTimesLine = "";
	std::string levelScoresLine = "";
	for (int i = 0;i < levelCount;i++) {
		beatLevelsLine += beatLevels[i] ? "true," : "false,";
		levelTimesLine += std::to_string(bestTimes[i]) + ",";
		levelScoresLine += std::to_string(bestScores[i]) + ",";
	}
	
	return beatLevelsLine + "\n" + levelTimesLine + "\n" + levelScoresLine;
}
