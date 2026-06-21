
#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <optional>
using namespace std;

class SoundManager 
{
private:
    static const int MAX_SOUNDS = 16;

    sf::SoundBuffer buffers[MAX_SOUNDS];
    optional<sf::Sound> sounds[MAX_SOUNDS];
    string          soundNames[MAX_SOUNDS];
    int             soundCount;
    sf::Music splashMusic;
    bool splashMusicLoaded = false;
    sf::Music menuMusic;      
    sf::Music gameMusic;    
    sf::Music gameOverMusic;  
    sf::Music loginMusic;

    bool loginMusicLoaded = false;
    bool menuMusicLoaded = false;
    bool gameMusicLoaded = false;
    bool gameOverMusicLoaded = false;

    float musicVolume = 50.f;
    float sfxVolume = 100.f;

    int findSound(const string& name) const
    {
        for (int i = 0; i < soundCount; i++)
            if (soundNames[i] == name) return i;
        return -1;
    }

public:
    SoundManager() : soundCount(0) {}
    bool loadSplashMusic(const string& path)
    {
        if (!splashMusic.openFromFile(path)) return false;
        splashMusic.setLooping(true);          // loops forever
        splashMusic.setVolume(musicVolume);
        splashMusicLoaded = true;
        return true;
    }
    bool loadLoginMusic(const string& path)
    {
        if (!loginMusic.openFromFile(path))
            return false;
        loginMusic.setLooping(true);
        loginMusic.setVolume(musicVolume);
        loginMusicLoaded = true;
        return true;
    }
    void playLoginMusic()
    {
        stopAll();
        if (loginMusicLoaded) loginMusic.play();
    }
    void playSplashMusic()
    {
        if (splashMusicLoaded) splashMusic.play();
    }
    bool loadSound(const string& name, const string& path)
    {
        if (soundCount >= MAX_SOUNDS)
            return false;

        if (!buffers[soundCount].loadFromFile(path))
            return false;
        sounds[soundCount].emplace(buffers[soundCount]);
        soundNames[soundCount] = name;
        soundCount++;
        return true;
    }
    void setSoundLooping(const string& name, bool loop)
    {
        int i = findSound(name);
        if (i != -1) sounds[i]->setLooping(loop);
    }
    void playSound(const string& name)
    {
        int i = findSound(name);
        if (i != -1) {
            sounds[i]->setVolume(sfxVolume);
            sounds[i]->play();
        }
    }

    bool loadMenuMusic(const string& path)
    {
        if (!menuMusic.openFromFile(path)) 
            return false;

        menuMusic.setLooping(true);
        menuMusic.setVolume(musicVolume);
        menuMusicLoaded = true;
        return true;
    }

    bool loadGameMusic(const string& path)
    {
        if (!gameMusic.openFromFile(path)) 
            return false;
        gameMusic.setLooping(true);
        gameMusic.setVolume(musicVolume);
        gameMusicLoaded = true;
        return true;
    }

    bool loadGameOverMusic(const string& path)
    {
        if (!gameOverMusic.openFromFile(path)) 
            return false;
        gameOverMusic.setLooping(false);
        gameOverMusic.setVolume(musicVolume);
        gameOverMusicLoaded = true;
        return true;
    }

    void playMenuMusic() 
    {
        stopAll();
        if (menuMusicLoaded) menuMusic.play();
    }

    void playGameMusic()
    {
        stopAll();
        if (gameMusicLoaded)
        {
            gameMusic.stop();   
            gameMusic.play();
        }
    }

    void playGameOverMusic() 
    {
        stopAll();
        if (gameOverMusicLoaded) 
            gameOverMusic.play();
    }

    void stopAll()
    {
        if (splashMusicLoaded)   splashMusic.stop();
        if (menuMusicLoaded)     menuMusic.stop();
        if (gameMusicLoaded)     gameMusic.stop();
        if (gameOverMusicLoaded) gameOverMusic.stop();
        if (loginMusicLoaded)    loginMusic.stop();   // ADD
    }

    void setMusicVolume(float v)
    {
        musicVolume = v;
        menuMusic.setVolume(v);
        gameMusic.setVolume(v);
        gameOverMusic.setVolume(v);
        loginMusic.setVolume(v);   // ADD
    }
    void pauseGameMusic()
    {
        if (gameMusicLoaded)
        gameMusic.pause(); 
    }

    void resumeGameMusic() 
    { if (gameMusicLoaded) 
        gameMusic.play();
    }

    
    void setSfxVolume(float v) 
    { 
        sfxVolume = v;
    }
    float getMusicVolume() const
    {
        return musicVolume;
    }
    float getSfxVolume()   const
    {
        return sfxVolume;
    }
};