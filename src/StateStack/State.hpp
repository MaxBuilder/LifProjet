//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_STATE_HPP
#define LIFPROJET_STATE_HPP

#include "../GameStates/StateIdentifiers.hpp"
#include "../Resources/ResourceIdentifiers.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

#include <memory>

class StateStack;

class State {

public:
    typedef std::unique_ptr<State> Ptr;

    struct Context { // Sert à stocker toutes les refs utiles dans les states (à incrémenter)
        sf::RenderWindow& window;
        TextureHolder& textures;
        FontHolder& fonts;
        //MusicPlayer& music;
        //SoundPlayer& sounds;

        Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts);
    };


public:
    State(StateStack& stack, Context& context);
    ~State();

    virtual void draw() = 0;
    virtual bool update(sf::Time dt) = 0;
    virtual bool handleEvent(const sf::Event & event) = 0;


protected:
    void requestStackPush(States::ID stateID);
    void requestStackPop();
    void requestStateClear();

    Context getContext() const;


private:
    StateStack& mStack;
    Context mContext;

};


#endif //LIFPROJET_STATE_HPP