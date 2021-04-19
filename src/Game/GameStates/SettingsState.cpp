//
// Created by thibaut on 15/02/2021.
//

#include "SettingsState.hpp"

SettingsState::SettingsState(StateStack& stack, Context context) :
 State(stack, context)
, mGUIContainer()
, mResolutionList()
, mVolumeList()
, mCheckBox()
, mWidth(1280)
, mHeight(720)
, mBackground(context.textures.get(Textures::SettingsBackground))
, mWindowStyle(sf::Style::Close)
{
    mTextWindowSize.setFont(context.fonts.get(Fonts::Main));
    mTextFullscreen.setFont(context.fonts.get(Fonts::Main));
    mTextVolume.setFont(context.fonts.get(Fonts::Main));


    mTextWindowSize.setPosition(250,180);
    mTextFullscreen.setPosition(250,240);
    mTextVolume.setPosition(250, 430);

    mTextWindowSize.setCharacterSize(20u);
    mTextFullscreen.setCharacterSize(20u);
    mTextVolume.setCharacterSize(20u);

    mTextWindowSize.setString("Size of Window");
    mTextFullscreen.setString("Fullscreen");
    mTextVolume.setString("Volume");

    auto backButton = std::make_shared<GUI::Button>(context, 50, 50, Textures::SettingsBack);
    backButton->setPosition(10, 10);
    backButton->setText("");
    backButton->setCallback([this] () {
        requestStackPop();
        getContext().sounds.play(Sounds::Menu);
    });
    mGUIContainer.pack(backButton);

    auto applyButton = std::make_shared<GUI::Button>(context, 130, 40, Textures::SettingsApply);
    applyButton->setPosition(1280-130-20,720-40-20);
    applyButton->setText("");
    applyButton->setCallback([this] () {
        apply();
        getContext().sounds.play(Sounds::Menu);
    });
    mGUIContainer.pack(applyButton);

    auto saveButton = std::make_shared<GUI::Button>(context, 130, 40, Textures::SettingsSave);
    saveButton->setPosition(1280-2*130-2*20,720-40-20);
    saveButton->setText("");
    saveButton->setCallback([this] () {
        saveSettings();
        getContext().sounds.play(Sounds::Menu);
    });
    mGUIContainer.pack(saveButton);

    auto resolutionList = std::make_shared<GUI::ButtonList>(context, 200, 60, 20u, Textures::SettingsSave);
    resolutionList->setPosition(800, 150);
    resolutionList->setCallback([this] (int count) {
        switch (count) {
            case 0 : mWidth = 1280; mHeight = 720; break;
            case 1 : mWidth = 1600; mHeight = 900; break;
            case 2 : mWidth = 1920; mHeight = 1080; break;
            default: break;
        }
        getContext().sounds.play(Sounds::Menu);
    });
    resolutionList->addOption("1280x720");
    resolutionList->addOption("1600x900");
    resolutionList->addOption("1920x1080");
    resolutionList->setOption(0);
    mResolutionList.pack(resolutionList);

    auto volumeList = std::make_shared<GUI::ButtonList>(context, 200, 60, 20u, Textures::SettingsSave);
    volumeList->setPosition(800, 400);
    volumeList->setCallback([this] (int count) {
        mVolume = count * 10;
        getContext().sounds.play(Sounds::Menu);
    });
    for(int i = 0 ; i <= 10 ; i++)
        volumeList->addOption(std::to_string(i * 10));
    volumeList->setOption(5);
    mVolumeList.pack(volumeList);

    auto fullscreen = std::make_shared<GUI::CheckBox>(context,40,40,Textures::Checkbox);
    fullscreen->setPosition(880,240);
    fullscreen->setCallback([this] (bool check) {
        if(check)
            mWindowStyle = sf::Style::Fullscreen;
        else
            mWindowStyle = sf::Style::Close;
        getContext().sounds.play(Sounds::Menu);
    });
    mCheckBox.pack(fullscreen);
}

void SettingsState::saveSettings() const {
    std::ofstream settings("data/Settings/Settings.txt");

    if(!settings.is_open()) {
        std::cout << "Cannot open Settings file!" << std::endl;
        return;
    }

    settings << "WindowStyle ";
    settings << mWindowStyle;
    settings << "\nWidth ";
    settings << mWidth;
    settings << "\nHeight ";
    settings << mHeight;
    settings << "\nVolume ";
    settings << mVolume;

    settings.close();
}

void SettingsState::apply() {
    saveSettings();

    sf::RenderWindow &window = getContext().window;

    window.create(sf::VideoMode(mWidth, mHeight), "LifProjet", mWindowStyle);

    sf::Image icon;
    icon.loadFromFile("data/Menu/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    getContext().music.setVolume((float)mVolume);
}

void SettingsState::draw() {
    sf::RenderWindow &window = getContext().window;
    window.draw(mBackground);
    window.draw(mGUIContainer);
    window.draw(mTextWindowSize);
    window.draw(mTextFullscreen);
    window.draw(mResolutionList);
    window.draw(mVolumeList);
    window.draw(mCheckBox);
}

bool SettingsState::update(sf::Time dt) {
    return false;
}

bool SettingsState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event,getContext().window);
    mResolutionList.handleListEvent(event, getContext().window);
    mVolumeList.handleListEvent(event, getContext().window);
    mCheckBox.handleCheckBoxEvent(event,getContext().window);
    return false;
}

SettingsState::~SettingsState() = default;
