//
// Created by thibaut on 15/02/2021.
//

#include "SettingsState.hpp"

SettingsState::SettingsState(StateStack& stack, Context context) :
 State(stack, context)
, mGUIContainer()
, mHeight(720)
, mWindowStyle(sf::Style::Close)
, mWidth(1280){


    mTextGraphisme.setFont(context.fonts.get(Fonts::Main));
    mTextWindowSize.setFont(context.fonts.get(Fonts::Main));
    mTextFullscreen.setFont(context.fonts.get(Fonts::Main));
    mTextSound.setFont(context.fonts.get(Fonts::Main));

    mTextGraphisme.setPosition(200,100);
    mTextWindowSize.setPosition(250,180);
    mTextFullscreen.setPosition(250,240);
    mTextSound.setPosition(200,340);

    mTextGraphisme.setCharacterSize(40u);
    mTextWindowSize.setCharacterSize(20u);
    mTextFullscreen.setCharacterSize(20u);
    mTextSound.setCharacterSize(40u);

    mTextGraphisme.setString("Graphisme : ");
    mTextWindowSize.setString("Size of Window");
    mTextFullscreen.setString("Fullscreen");
    mTextSound.setString("Sound : ");


    auto backButton = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    backButton->setPosition(20, 20);
    backButton->setText("Back");
    backButton->setCallback([this](){
        requestStackPop();
        requestStackPush(States::MainMenu);
        getContext().sounds.play(Sounds::Menu);
    });
    mGUIContainer.pack(backButton);

    auto applyButton = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    applyButton->setPosition(1280-130-20,720-70-20 );
    applyButton->setText("Apply");
    applyButton->setCallback([this](){
        apply();
        getContext().sounds.play(Sounds::Menu);
    });
    mGUIContainer.pack(applyButton);

    auto saveButton = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    saveButton->setPosition(1280-2*130-2*20,720-70-20 );
    saveButton->setText("Save");
    saveButton->setCallback([this](){
        saveSettings();
        getContext().sounds.play(Sounds::Menu);
    });
    mGUIContainer.pack(saveButton);

    auto vidMode = std::make_shared<GUI::ButtonList>(context, 200, 60,20u, Textures::MapEditorButton);
    vidMode->setPosition(900-100,180-30 );
    vidMode->setCallback([this](int count){
        switch (count) {
            case 0 : mWidth = 1280; mHeight = 720; break;
            case 1 : mWidth = 1600; mHeight = 900; break;
            case 2 : mWidth = 1920; mHeight = 1080; break;
        }
        getContext().sounds.play(Sounds::Menu);
    });
    vidMode->addOption("1280x720");
    vidMode->addOption("1600x900");
    vidMode->addOption("1920x1080");
    vidMode->setOption(0);
    mButtonList.pack(vidMode);

    auto fullscreen = std::make_shared<GUI::CheckBox>(context,40,40,Textures::Checkbox);
    fullscreen->setPosition(900-20,240);
    fullscreen->setCallback([this](bool check){
        if(check)
            mWindowStyle = sf::Style::Fullscreen;
        else
            mWindowStyle = sf::Style::Close;
        getContext().sounds.play(Sounds::Menu);
    });
    mCheckBox.pack(fullscreen);


}

void SettingsState::saveSettings(){

    std::ofstream settings("data/Settings/Settings.txt");

    if(!settings.is_open()) {
        std::cout << "Cannot open Settings file!" << std::endl;
        return ;
    }

    settings << "WindowStyle ";
    settings << mWindowStyle;
    settings << "\nWidth ";
    settings << mWidth;
    settings << "\nHeight ";
    settings << mHeight;

    settings.close();
}

void SettingsState::apply(){


    saveSettings();

    sf::RenderWindow &window = getContext().window;

    window.create(sf::VideoMode(mWidth, mHeight),"LifProjet",mWindowStyle);

}

void SettingsState::draw() {
    sf::RenderWindow &window = getContext().window;
    window.draw(mGUIContainer);
    window.draw(mTextGraphisme);
    window.draw(mTextWindowSize);
    window.draw(mTextFullscreen);
    window.draw(mTextSound);
    window.draw(mButtonList);
    window.draw(mCheckBox);
}

bool SettingsState::update(sf::Time dt){
    return true;
}

bool SettingsState::handleEvent(const sf::Event& event){
    mGUIContainer.handleEvent(event,getContext().window);
    mButtonList.handleListEvent(event,getContext().window);
    mCheckBox.handleCheckBoxEvent(event,getContext().window);
    return false;
}
