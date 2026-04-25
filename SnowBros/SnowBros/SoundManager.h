#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <optional>

using namespace std;

// Manages all game audio — sound effects + background music
// No STL maps, uses parallel arrays instead
class SoundManager {
private:
    static const int MAX_SOUNDS = 16;

    // Sound effects
    sf::SoundBuffer buffers[MAX_SOUNDS];
    optional<sf::Sound> sounds[MAX_SOUNDS];
    string          soundNames[MAX_SOUNDS];
    int             soundCount;

    // Background music (sf::Music streams from file — better for large files)
    sf::Music music;
    bool      musicLoaded;
    float     musicVolume;
    float     sfxVolume;

    // Find index of a loaded sound by name
    int findSound(const string& name) const {
        for (int i = 0; i < soundCount; i++)
            if (soundNames[i] == name) return i;
        return -1;
    }

public:

    SoundManager() : soundCount(0), musicLoaded(false),
        musicVolume(50.f), sfxVolume(100.f) 
    {
    }

    // Load a sound effect from file, give it a name
    // e.g. loadSound("throw",    "assets/sounds/throw.wav")
    //      loadSound("encase",   "assets/sounds/encase.wav")
    //      loadSound("roll",     "assets/sounds/roll.wav")
    //      loadSound("death",    "assets/sounds/death.wav")
    //      loadSound("levelup",  "assets/sounds/levelup.wav")
    //      loadSound("gemcollect","assets/sounds/gem.wav")
    bool loadSound(const string& name, const string& path) {
        if (soundCount >= MAX_SOUNDS) return false;
        if (!buffers[soundCount].loadFromFile(path)) return false;
        sounds[soundCount].emplace(buffers[soundCount]);
        soundNames[soundCount] = name;
        soundCount++;
        return true;
    }

    // Play a sound effect by name
    void playSound(const string& name) {
        int i = findSound(name);
        if (i != -1) {
            sounds[i]->setVolume(sfxVolume);
            sounds[i]->play();
        }
    }

    // Load and start background music
    bool loadMusic(const string& path) {
        if (!music.openFromFile(path)) return false;
        musicLoaded = true;
        music.setLooping(true);
        music.setVolume(musicVolume);
        return true;
    }

    void playMusic() { if (musicLoaded) music.play(); }
    void stopMusic() { if (musicLoaded) music.stop(); }
    void pauseMusic() { if (musicLoaded) music.pause(); }

    void setMusicVolume(float v) {
        musicVolume = v;
        music.setVolume(v);
    }

    void setSfxVolume(float v) { sfxVolume = v; }

    float getMusicVolume() const { return musicVolume; }
    float getSfxVolume()   const { return sfxVolume; }
};