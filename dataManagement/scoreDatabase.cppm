// Module
// File: scoreDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:20:00
// Description:
//
export module cs:database.score;

import std;
import :entity.score;


export class ScoreDatabase{
public:
    static ScoreDatabase singleton();
    bool saveScore(const Score &score);
    bool deleteScore(std::string scoreId);
    bool updateScore(const Course &Score);
    bool findScoreById(std::string scoreId);

private:
    std::vector<std::shared_ptr<Score>> _scores;
};
