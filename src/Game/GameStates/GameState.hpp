//
// Created by 33771 on 14/02/2021.
//

#ifndef LIFPROJET_GAMESTATE_HPP
#define LIFPROJET_GAMESTATE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System.hpp>

#include "../../Core/StateStack/State.hpp"
#include "../../Core/Util/Utility.hpp"
#include "../../Core/GUI/Button.hpp"
#include "../../Core/GUI/Container.hpp"
#include "../World/World.hpp"

#include <unordered_map>
#include <string>

class GameState : public State {
public:
    /**
     * @brief Parameter constructor
     * @param stack State stack where to push the state
     * @param context Context used to access useful structures
     */
    GameState(StateStack& stack, Context& context);

    /**
     * @brief Draw the state
     */
    void draw() override;
    /**
     * @brief Update the state
     * @param dt Time interval since the last update
     */
    bool update(sf::Time dt) override;
    /**
     * @brief Handle the events of the state
     * @param event Event to handle
     */
    bool handleEvent(const sf::Event& event) override;

private:
    /**
     * @brief Initialize the simulation after the scenario choice
     */
    void initializeSimulation();

    World mWorld;
    World::SimulationData& simData;

    // UI Containers :

    GUI::Container mUI;
    GUI::Container mMapSelectionUI;
    GUI::Container mVictoryUI;
    GUI::Container mBackButton;

    // Map selection UI :

    std::string mMapPath;
    bool isLoaded;
    sf::Sprite mMapSelectBackground;
    sf::Text mMapSelectText;

    // Victory UI :

    sf::Sprite mVictoryScreen;
    sf::Text mVictoryText;
    std::vector<std::string> mVictoryInfoTab;
    sf::Text mVictoryInfo;
    bool ended;
    bool closed;

    // Simulation UI :

    sf::Sprite mTimeUI;
    sf::Text mTrackText;
    sf::Text mTimeText;
    sf::RectangleShape mBlueDisplay, mRedDisplay;

    // Camera variables :

    sf::View mView;
    sf::Vector2f mDirection;
    float mSpeed;
    float mFactor;
    int mScroll;
    float mTimeSpeed;
    bool mTracking;
    sf::Time mTime;

};


#endif //LIFPROJET_GAMESTATE_HPP
