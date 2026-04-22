#include <iostream>
#include "PlayerData.h"
#include "FileManager.h"
#include "GemSystem.h"

int main() {
    // --- Test 1: Create a player ---
    PlayerData player;
    player.setUsername("Maryam");
    std::cout << "=== NEW PLAYER ===" << std::endl;
    std::cout << "Username: " << player.getUsername() << std::endl;
    std::cout << "Level: " << player.getCurrentLevel() << std::endl;
    std::cout << "Lives: " << player.getLives() << std::endl;
    std::cout << "Gems: " << player.getGemCount() << std::endl;

    // --- Test 2: Gem system ---
    GemSystem gems(player);
    std::cout << "\n=== GEM TESTS ===" << std::endl;

    gems.enemykilled();
    gems.enemykilled();
    gems.enemykilled();
    std::cout << "After 3 enemy kills: " << player.getGemCount() << " gems" << std::endl;

    gems.Mogerakilled();
    std::cout << "After Mogera kill: " << player.getGemCount() << " gems" << std::endl;

    gems.gamakichikilled();
    std::cout << "After Gamakichi kill: " << player.getGemCount() << " gems" << std::endl;

    gems.addGems(50);
    std::cout << "After bonus level (+50): " << player.getGemCount() << " gems" << std::endl;

    // --- Test 3: Save ---
    std::cout << "\n=== SAVE TEST ===" << std::endl;
    player.setCurrentLevel(3);
    player.setLives(2);
    bool saved = FileManager::savePlayerData(player);
    std::cout << "Save successful: " << (saved ? "YES" : "NO") << std::endl;

    // --- Test 4: Load ---
    std::cout << "\n=== LOAD TEST ===" << std::endl;
    PlayerData loaded;
    bool loadSuccess = FileManager::loadPlayerData("alice", loaded);
    std::cout << "Load successful: " << (loadSuccess ? "YES" : "NO") << std::endl;
    std::cout << "Username: " << loaded.getUsername() << std::endl;
    std::cout << "Level: " << loaded.getCurrentLevel() << std::endl;
    std::cout << "Lives: " << loaded.getLives() << std::endl;
    std::cout << "Gems: " << loaded.getGemCount() << std::endl;

    // --- Test 5: Save exists ---
    std::cout << "\n=== SAVE EXISTS TEST ===" << std::endl;
    std::cout << "alice save exists: " << (FileManager::saveExists("alice") ? "YES" : "NO") << std::endl;
    std::cout << "bob save exists: " << (FileManager::saveExists("bob") ? "YES" : "NO") << std::endl;

    return 0;
}