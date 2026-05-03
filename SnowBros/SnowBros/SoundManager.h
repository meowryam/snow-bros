//#pragma once
//#include <SFML/Audio.hpp>
//#include <string>
//#include <optional>
//
//using namespace std;
//
//// Manages all game audio — sound effects + background music
//// No STL maps, uses parallel arrays instead
//class SoundManager {
//private:
//    static const int MAX_SOUNDS = 16;
//
//    // Sound effects
//    sf::SoundBuffer buffers[MAX_SOUNDS];
//    optional<sf::Sound> sounds[MAX_SOUNDS];
//    string          soundNames[MAX_SOUNDS];
//    int             soundCount;
//
//    // Background music (sf::Music streams from file — better for large files)
//    sf::Music music;
//    bool      musicLoaded;
//    float     musicVolume;
//    float     sfxVolume;
//
//    // Find index of a loaded sound by name
//    int findSound(const string& name) const {
//        for (int i = 0; i < soundCount; i++)
//            if (soundNames[i] == name) return i;
//        return -1;
//    }
//
//public:
//
//    SoundManager() : soundCount(0), musicLoaded(false),
//        musicVolume(50.f), sfxVolume(100.f) 
//    {
//    }
//
//    // Load a sound effect from file, give it a name
//    // e.g. loadSound("throw",    "assets/sounds/throw.wav")
//    //      loadSound("encase",   "assets/sounds/encase.wav")
//    //      loadSound("roll",     "assets/sounds/roll.wav")
//    //      loadSound("death",    "assets/sounds/death.wav")
//    //      loadSound("levelup",  "assets/sounds/levelup.wav")
//    //      loadSound("gemcollect","assets/sounds/gem.wav")
//    bool loadSound(const string& name, const string& path) {
//        if (soundCount >= MAX_SOUNDS) return false;
//        if (!buffers[soundCount].loadFromFile(path)) return false;
//        sounds[soundCount].emplace(buffers[soundCount]);
//        soundNames[soundCount] = name;
//        soundCount++;
//        return true;
//    }
//
//    // Play a sound effect by name
//    void playSound(const string& name) {
//        int i = findSound(name);
//        if (i != -1) {
//            sounds[i]->setVolume(sfxVolume);
//            sounds[i]->play();
//        }
//    }
//
//    // Load and start background music
//    bool loadMusic(const string& path) {
//        if (!music.openFromFile(path)) return false;
//        musicLoaded = true;
//        music.setLooping(true);
//        music.setVolume(musicVolume);
//        return true;
//    }
//
//    void playMusic() { if (musicLoaded) music.play(); }
//    void stopMusic() { if (musicLoaded) music.stop(); }
//    void pauseMusic() { if (musicLoaded) music.pause(); }
//
//    void setMusicVolume(float v) {
//        musicVolume = v;
//        music.setVolume(v);
//    }
//
//    void setSfxVolume(float v) { sfxVolume = v; }
//
//    float getMusicVolume() const { return musicVolume; }
//    float getSfxVolume()   const { return sfxVolume; }
//};








#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <optional>
using namespace std;

class SoundManager {
private:
    static const int MAX_SOUNDS = 16;

    sf::SoundBuffer buffers[MAX_SOUNDS];
    optional<sf::Sound> sounds[MAX_SOUNDS];
    string          soundNames[MAX_SOUNDS];
    int             soundCount;

    // Separate music streams for each context
    sf::Music menuMusic;      // plays on main menu
    sf::Music gameMusic;      // plays during gameplay, loops per level
    sf::Music gameOverMusic;  // plays when player dies and game ends

    bool menuMusicLoaded = false;
    bool gameMusicLoaded = false;
    bool gameOverMusicLoaded = false;

    float musicVolume = 50.f;
    float sfxVolume = 100.f;

    int findSound(const string& name) const {
        for (int i = 0; i < soundCount; i++)
            if (soundNames[i] == name) return i;
        return -1;
    }

public:
    SoundManager() : soundCount(0) {}

    // ?? SFX ????????????????????????????????????????????????????
    bool loadSound(const string& name, const string& path) {
        if (soundCount >= MAX_SOUNDS) return false;
        if (!buffers[soundCount].loadFromFile(path)) return false;
        sounds[soundCount].emplace(buffers[soundCount]);
        soundNames[soundCount] = name;
        soundCount++;
        return true;
    }

    void playSound(const string& name) {
        int i = findSound(name);
        if (i != -1) {
            sounds[i]->setVolume(sfxVolume);
            sounds[i]->play();
        }
    }

    // ?? Music loaders ???????????????????????????????????????????
    bool loadMenuMusic(const string& path) {
        if (!menuMusic.openFromFile(path)) return false;
        menuMusic.setLooping(true);
        menuMusic.setVolume(musicVolume);
        menuMusicLoaded = true;
        return true;
    }

    bool loadGameMusic(const string& path) {
        if (!gameMusic.openFromFile(path)) return false;
        gameMusic.setLooping(true);
        gameMusic.setVolume(musicVolume);
        gameMusicLoaded = true;
        return true;
    }

    bool loadGameOverMusic(const string& path) {
        if (!gameOverMusic.openFromFile(path)) return false;
        gameOverMusic.setLooping(false);
        gameOverMusic.setVolume(musicVolume);
        gameOverMusicLoaded = true;
        return true;
    }

    // ?? Music controls ??????????????????????????????????????????

    // Call when entering main menu
    void playMenuMusic() {
        stopAll();
        if (menuMusicLoaded) menuMusic.play();
    }

    // Call when game starts AND when next level begins (restarts from top)
    void playGameMusic() {
        stopAll();
        if (gameMusicLoaded) {
            gameMusic.stop();   // restart from beginning
            gameMusic.play();
        }
    }

    // Call when game over screen shows
    void playGameOverMusic() {
        stopAll();
        if (gameOverMusicLoaded) gameOverMusic.play();
    }

    void stopAll() {
        if (menuMusicLoaded)    menuMusic.stop();
        if (gameMusicLoaded)    gameMusic.stop();
        if (gameOverMusicLoaded) gameOverMusic.stop();
    }

    void pauseGameMusic() { if (gameMusicLoaded)  gameMusic.pause(); }
    void resumeGameMusic() { if (gameMusicLoaded)  gameMusic.play(); }

    void setMusicVolume(float v) {
        musicVolume = v;
        menuMusic.setVolume(v);
        gameMusic.setVolume(v);
        gameOverMusic.setVolume(v);
    }
    void setSfxVolume(float v) { sfxVolume = v; }
    float getMusicVolume() const { return musicVolume; }
    float getSfxVolume()   const { return sfxVolume; }
};