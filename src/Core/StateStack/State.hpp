//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_STATE_HPP
#define LIFPROJET_STATE_HPP

#include "../../Game/GameStates/StateIdentifiers.hpp"
#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "../Audio/SoundPlayer.hpp"
#include "../Audio/MusicPlayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

#include <memory>

class StateStack;

/**
 * @class State
 * @brief Parent class of a specialized state. Meant to have own update, render and event handling
 * @author SFML Game Development
 */
class State {
public:
    typedef std::unique_ptr<State> Ptr;

    /**
     * @struct Context
     * @brief Stores all references that need to be accessed across the program
     */
    struct Context { // Stocke toutes les refs utiles dans les states (à incrémenter)
        sf::RenderWindow& window;
        TextureHolder& textures;
        FontHolder& fonts;
        SoundPlayer& sounds;
        MusicPlayer& music;

        /**
         * @brief Parameter constructor
         * @param window Window where textures are drawn
         * @param textures Texture holder
         * @param fonts Font holder
         * @param sounds SoundManager
         * @param music MusicManager
         */
        Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, SoundPlayer& sounds, MusicPlayer& music);
    };


public:
    /**
     * @brief Paramer constructor
     * @param stack Structure holding the current states
     * @param context Context structure
     */
    State(StateStack& stack, Context& context);
    /**
     * @brief Virtual destructor (if needed)
     */
    virtual ~State();

    /**
     * @brief Drawing function
     */
    virtual void draw() = 0;
    /**
     * @brief Update function
     * @param dt Time interval since last update
     * @return true if the states under the current need to be updated
     */
    virtual bool update(sf::Time dt) = 0;
    /**
     * @brief Event handling function
     * @param event Event to handle
     * @return true if states under need their events handled
     */
    virtual bool handleEvent(const sf::Event & event) = 0;


protected:
    /**
     * @brief Request that next times changes are applied, a state will be pushed
     * @param stateID Identifier of the state to push
     */
    void requestStackPush(States::ID stateID);
    /**
     * @brief Request that next time changes are applied, the top state will be popped
     */
    void requestStackPop();
    /**
     * @brief Request that next time changes are applied, the stack will be cleared
     */
    void requestStackClear();

    /**
     * @brief Context getter
     * @return Context
     */
    Context getContext() const;


private:
    StateStack& mStack;
    Context mContext;

};


#endif //LIFPROJET_STATE_HPP
