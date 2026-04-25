#pragma once
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// Stores top 10 scores globally — spec 9.4
// Each entry: player name, score, level reached, date
struct LeaderboardEntry {
    string username;
    int    score;
    int    levelReached;
    string date;

    LeaderboardEntry() : score(0), levelReached(0) {}
    LeaderboardEntry(string u, int s, int l, string d)
        : username(u), score(s), levelReached(l), date(d) {
    }
};

class Leaderboard {
private:
    static const int MAX_ENTRIES = 10;
    LeaderboardEntry entries[MAX_ENTRIES];
    int count;

    string filePath = "saves/leaderboard.txt";

public:
    Leaderboard() : count(0) {}

    void load() {
        count = 0;
        ifstream file(filePath);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line) && count < MAX_ENTRIES) {
            istringstream ss(line);
            string username, score, level, date;
            if (getline(ss, username, ',') &&
                getline(ss, score, ',') &&
                getline(ss, level, ',') &&
                getline(ss, date, ',')) {
                entries[count++] = LeaderboardEntry(
                    username, stoi(score), stoi(level), date);
            }
        }
    }

    void save() {
        ofstream file(filePath);
        if (!file.is_open()) return;
        for (int i = 0; i < count; i++) {
            file << entries[i].username << ","
                << entries[i].score << ","
                << entries[i].levelReached << ","
                << entries[i].date << "\n";
        }
    }

    // Try to insert a new score — keeps top 10 sorted
    void submitScore(const string& username, int score, int level, const string& date) {
        // check if qualifies
        if (count == MAX_ENTRIES && score <= entries[count - 1].score)
            return;

        // insert
        if (count < MAX_ENTRIES) count++;
        int i = count - 1;
        // shift down
        while (i > 0 && entries[i - 1].score < score) {
            entries[i] = entries[i - 1];
            i--;
        }
        entries[i] = LeaderboardEntry(username, score, level, date);
        save();
    }

    int getCount() const { return count; }
    const LeaderboardEntry& getEntry(int i) const { return entries[i]; }

    bool isHighScore(int score) const {
        if (count < MAX_ENTRIES) return true;
        return score > entries[count - 1].score;
    }
};