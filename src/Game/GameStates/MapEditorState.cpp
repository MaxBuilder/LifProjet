//
// Created by thibaut on 01/02/2021.
//

#include "MapEditorState.hpp"
#include <iostream>

#include "MapEditorState.hpp"

MapEditorState::MapEditorState(StateStack &stack, Context context)
        : State(stack, context)
        , map(context.textures.get(Textures::Map), 16.f,sf::Vector2i(92,90))
{

    background.setTexture(context.textures.get(Textures::MapEditorBackGround));
    map.setDrawBuildings(true);

    mapPath.setFont(context.fonts.get(Fonts::Main));
    mapPath.setPosition(8, 684);
    mapPath.setCharacterSize(20u);

    rotate = 0.f;
    mMapPath = std::string("data/Maps/demo2.map");
    ground_selection = Textures::ground::Sand;
    lastGround = Textures::ground::None;
    lastTileUpdate = {-1,-1};

    tool = map::tool::standard;

    // Edit buttons :

    auto backButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Back);
    backButton->setPosition(16, 10);
    //backButton->setText("Back");
    backButton->setCallback([this](){
        requestStackPop();
        requestStackPush(States::MainMenu);
        getContext().sounds.play(Sounds::Menu);
    });
    mEditBar.pack(backButton);

    auto newButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::New);
    newButton->setPosition(98, 10);
    newButton->setCallback([this] () {
        map.clear();
        getContext().sounds.play(Sounds::Menu);
    });
    mEditBar.pack(newButton);

    auto saveButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Save);
    saveButton->setPosition(170, 10);
    saveButton->setCallback([this] () {
        map.save(mMapPath);
        getContext().sounds.play(Sounds::Menu);
    });
    mEditBar.pack(saveButton);

    auto loadButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Load);
    loadButton->setPosition(242, 10);
    loadButton->setCallback([this] () {
        map.load(mMapPath);
        setBuildings();
        getContext().sounds.play(Sounds::Menu);
    });
    mEditBar.pack(loadButton);

    auto undoButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Undo);
    undoButton->setPosition(324, 10);
    undoButton->setCallback([this] () {
        getContext().sounds.play(Sounds::Menu);
    });
    mEditBar.pack(undoButton);

    auto redoButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Redo);
    redoButton->setPosition(396, 10);
    redoButton->setCallback([this] () {
        getContext().sounds.play(Sounds::Menu);
    });
    mEditBar.pack(redoButton);

    // Tool bar buttons :

    auto smallBrushButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::SmallBrush);
    smallBrushButton->setPosition(16, 98);
    smallBrushButton->setToggle(true);
    smallBrushButton->activate();
    smallBrushButton->setCallback([this](){
        tool = map::tool::standard;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(smallBrushButton);

    auto mediumBrushButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::MediumBrush);
    mediumBrushButton->setPosition(16, 164);
    mediumBrushButton->setToggle(true);
    mediumBrushButton->setCallback([this](){
        tool = map::tool::square3;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(mediumBrushButton);

    auto bigBrushButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::BigBrush);
    bigBrushButton->setPosition(16, 230);
    bigBrushButton->setToggle(true);
    bigBrushButton->setCallback([this](){
        tool = map::tool::circle5;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(bigBrushButton);

    auto eraserButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Eraser);
    eraserButton->setPosition(16, 296);
    eraserButton->setToggle(true);
    eraserButton->setCallback([this](){
        tool = map::tool::eraser;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(eraserButton);

    auto fillButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Fill);
    fillButton->setPosition(16, 362);
    fillButton->setToggle(true);
    fillButton->setCallback([this](){
        tool = map::tool::fill;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(fillButton);

    // Rotation bar buttons :

    auto rotateUpButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::RotateUp);
    rotateUpButton->setPosition(16, 532);
    rotateUpButton->setToggle(true);
    rotateUpButton->activate();
    rotateUpButton->setCallback([this] () {
        rotate = 0.f;
        getContext().sounds.play(Sounds::Menu);
    });
    mRotationBar.pack(rotateUpButton);

    auto rotateRightButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::RotateRight);
    rotateRightButton->setPosition(16, 598);
    rotateRightButton->setToggle(true);
    rotateRightButton->setCallback([this] () {
        rotate = 90.f;
        getContext().sounds.play(Sounds::Menu);
    });
    mRotationBar.pack(rotateRightButton);

    // Texture selection buttons :

    auto grassButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Grass);
    grassButton->setPosition(1134, 100);
    grassButton->setToggle(true);
    grassButton->activate();
    grassButton->setCallback([this](){
        ground_selection = Textures::ground::Grass;
        mBuild_selection = Textures::building::None;
        getContext().sounds.play(Sounds::Menu);
    });
    mPaletteBar.pack(grassButton);

    auto sandButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Sand);
    sandButton->setPosition(1202, 100);
    sandButton->setToggle(true);
    sandButton->setCallback([this](){
        ground_selection = Textures::ground::Sand;
        mBuild_selection = Textures::building::None;
        getContext().sounds.play(Sounds::Menu);
    });
    mPaletteBar.pack(sandButton);

    auto woodButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Wood);
    woodButton->setPosition(1134, 168);
    woodButton->setToggle(true);
    woodButton->setCallback([this](){
        ground_selection = Textures::ground::Wood;
        mBuild_selection = Textures::building::None;
        getContext().sounds.play(Sounds::Menu);
    });
    mPaletteBar.pack(woodButton);

    auto waterButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Water);
    waterButton->setPosition(1202, 168);
    waterButton->setToggle(true);
    waterButton->setCallback([this](){
        ground_selection = Textures::ground::Water;
        mBuild_selection = Textures::building::None;
        getContext().sounds.play(Sounds::Menu);
    });
    mPaletteBar.pack(waterButton);

    auto stoneButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Stone);
    stoneButton->setPosition(1134, 234);
    stoneButton->setToggle(true);
    stoneButton->setCallback([this](){
        ground_selection = Textures::ground::Wall;
        mBuild_selection = Textures::building::None;
        getContext().sounds.play(Sounds::Menu);
    });
    mPaletteBar.pack(stoneButton);

    auto barrierButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Barrier);
    barrierButton->setPosition(1134, 300);
    barrierButton->setToggle(true);
    barrierButton->setCallback([this](){
        ground_selection = Textures::ground::None;
        mBuild_selection = Textures::building::Barrier;
        getContext().sounds.play(Sounds::Menu);
    });
    mPaletteBar.pack(barrierButton);

    auto homeButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Home);
    homeButton->setPosition(1202, 300);
    homeButton->setToggle(true);
    homeButton->setCallback([this](){
        ground_selection = Textures::ground::None;
        mBuild_selection = Textures::building::Home;
        getContext().sounds.play(Sounds::Menu);
    });
    mPaletteBar.pack(homeButton);

    auto castleButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::Castle);
    castleButton->setPosition(1134, 366);
    castleButton->setToggle(true);
    castleButton->setCallback([this](){
        ground_selection = Textures::ground::None;
        mBuild_selection = Textures::building::Castle;
        getContext().sounds.play(Sounds::Menu);
    });
    mPaletteBar.pack(castleButton);

    /*
    auto rotateAleaButton = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    rotateAleaButton->setPosition(1536-130-30,1026-70.f/2 );
    rotateAleaButton->setText("0-90-180°");
    rotateAleaButton->setToggle(true);
    rotateAleaButton->setCallback([this](){
        rotate = -1;
        getContext().sounds.play(Sounds::Menu);
    });
    mTextureRotation.pack(rotateAleaButton);

    auto map1Button = std::make_shared<GUI::Button>(context, 329, 70, Textures::MenuButton);
    map1Button->setPosition(1536+29,1026-70.f/2 );
    map1Button->setText("map 1");
    map1Button->setToggle(true);
    map1Button->setCallback([this](){
        mMapPath = std::string("data/Maps/demo1.map");
        getContext().sounds.play(Sounds::Menu);
    });
    mCurrentMap.pack(map1Button);

    auto map2Button = std::make_shared<GUI::Button>(context, 329, 70, Textures::MenuButton);
    map2Button->setPosition(1536+29,1026-70.f/2 - 70-29);
    map2Button->setText("map 2");
    map2Button->setToggle(true);
    map2Button->setCallback([this](){
        mMapPath = std::string("data/Maps/demo2.map");
        getContext().sounds.play(Sounds::Menu);
    });
    mCurrentMap.pack(map2Button);

    auto map3Button = std::make_shared<GUI::Button>(context, 329, 70, Textures::MenuButton);
    map3Button->setPosition(1536+29,1026-70.f/2 -2*70-2*29 );
    map3Button->setText("map 3");
    map3Button->setToggle(true);
    map3Button->setCallback([this](){
        mMapPath = std::string("data/Maps/demo3.map");
        getContext().sounds.play(Sounds::Menu);
    });
    mCurrentMap.pack(map3Button);*/
}

void MapEditorState::draw() {

    sf::RenderWindow& window = getContext().window;
    window.draw(background);
    window.draw(map);
    window.draw(mEditBar);
    window.draw(mToolBar);
    window.draw(mPaletteBar);
    window.draw(mRotationBar);

    mapPath.setString(mMapPath);
    window.draw(mapPath);

    // Ajouter position
}

bool MapEditorState::update(sf::Time dt) {
    return true;
}

bool MapEditorState::handleEvent(const sf::Event& event) {

    mEditBar.handleEvent(event, getContext().window);
    mToolBar.handleEvent(event, getContext().window);
    mPaletteBar.handleEvent(event, getContext().window);
    mRotationBar.handleEvent(event, getContext().window);

    if (event.type == sf::Event::MouseMoved or event.type == sf::Event::MouseButtonPressed){

        sf::Vector2i WindowPosition = sf::Mouse::getPosition(getContext().window);
        sf::Vector2i pos = static_cast<sf::Vector2i>( getContext().window.mapPixelToCoords(WindowPosition));

        if (pos.x < 1116 and pos.x >= 92 and pos.y >= 90 and pos.y < 666 ){ // rectangle contenant la map

            auto caseSize = map.getBlockSize();
            sf::Vector2i origin = map.getOrigins();

            pos.y = pos.y - origin.y;
            pos.x = pos.x - origin.x;
            pos.y = pos.y/caseSize;
            pos.x = pos.x/caseSize;

            if (mBuild_selection != Textures::building::None and event.type == sf::Event::MouseButtonPressed ){
                if (event.mouseButton.button == sf::Mouse::Left)
                    createBuildings(pos);
                else if (event.mouseButton.button == sf::Mouse::Right)
                    supressBuildings(pos);
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and ground_selection !=Textures::ground::None) {

                if (lastTileUpdate == pos and lastGround == ground_selection)
                    return false;
                else {
                    lastTileUpdate = pos;
                    lastGround = ground_selection;
                }

                switch (tool) {

                    case map::tool::standard :
                        Paint::paintSprite(rotate, ground_selection, map.getTile(pos.x,pos.y));
                        break;

                    case map::tool::square3 :
                        paintSquare3(pos);
                        break;

                    case map::tool::circle5 :
                        paintCircle5(pos);
                        break;

                    case map::tool::fill :
                        paintFill(pos);
                        break;

                    case map::tool::eraser :
                        Paint::paintSprite(rotate, Textures::ground::None, map.getTile(pos.x, pos.y));

                    default :
                        break;
                }
            }
        }
    }
    return false;
}

void MapEditorState::paintSquare3(sf::Vector2i pos){
    Paint::paintSprite(rotate, ground_selection, map.getTile(pos.x,pos.y));

    if (pos.x > 0){
        Paint::paintSprite(rotate, ground_selection, map.getTile(pos.x-1,pos.y));
        if (pos.y > 0)
            Paint::paintSprite(rotate, ground_selection, map.getTile(pos.x-1,pos.y-1));
        if (pos.y < 35)
            Paint::paintSprite(rotate, ground_selection, map.getTile(pos.x-1,pos.y+1));
    }
    if(pos.x < 63){
        Paint::paintSprite(rotate, ground_selection, map.getTile(pos.x+1,pos.y));
        if (pos.y > 0)
            Paint::paintSprite(rotate, ground_selection, map.getTile(pos.x+1,pos.y-1));
        if (pos.y < 35)
            Paint::paintSprite(rotate, ground_selection, map.getTile(pos.x+1,pos.y+1));
    }
    if (pos.y > 0)
        Paint::paintSprite(rotate, ground_selection, map.getTile(pos.x,pos.y-1));
    if (pos.y < 35)
        Paint::paintSprite(rotate, ground_selection, map.getTile(pos.x,pos.y+1));

}

void MapEditorState::paintCircle5(sf::Vector2i pos){
    Paint::paintSprite(rotate, ground_selection, map.getTile(pos.x,pos.y-1));
}

void MapEditorState::paintFill(sf::Vector2i pos){
    bool isPaint[36*64] = {false};
    recPaintFill(pos, isPaint);
}

void MapEditorState::recPaintFill(sf::Vector2i co, bool* isPaint){

    isPaint[co.x+64*co.y] = true;

    if ( co.x > 0 and !isPaint[co.x-1+64*co.y] and
         map.getTile(co.x,co.y).getGround() ==  map.getTile(co.x-1,co.y).getGround() )

        recPaintFill( sf::Vector2i (co.x - 1, co.y), isPaint);

    if ( co.x < 63 and !isPaint[co.x+1+64*co.y] and
         map.getTile(co.x,co.y).getGround() ==  map.getTile(co.x+1,co.y).getGround() )

        recPaintFill(sf::Vector2i (co.x + 1, co.y), isPaint);

    if ( co.y > 0 and !isPaint[co.x+64*(co.y-1)] and
         map.getTile(co.x,co.y).getGround() ==  map.getTile(co.x,co.y-1).getGround() )

        recPaintFill( sf::Vector2i (co.x, co.y - 1), isPaint);

    if ( co.y < 35 and !isPaint[co.x+64*(co.y+1)] and
         map.getTile(co.x,co.y).getGround() ==  map.getTile(co.x,co.y+1).getGround() )

        recPaintFill(sf::Vector2i (co.x, co.y + 1), isPaint);

    Paint::paintSprite(rotate, ground_selection,  map.getTile(co.x,co.y));

}

void MapEditorState::createBuildings(sf::Vector2i pos){
    bool isConstructible = true;
    sf::IntRect rect1 = {pos.x, pos.y, 0, 0};
    sf::IntRect spritePos;

    switch(mBuild_selection){
        case Textures::building::Castle :  rect1.width = 3; rect1.height = 3;
            spritePos = {0,0,90,90};
            break;

        case Textures::building::Home :  rect1.width = 2; rect1.height = 2;
            spritePos = {90,0,60,60};
            break;

        case Textures::building::Barrier :
            spritePos = {90,60,60,30};
            if (rotate == 0 or rotate == -1){
                rect1.width = 2;
                rect1.height = 1;
            }
            else{
                rect1.width = 1;
                rect1.height = 2;
            }
            break;

        default : return;
    }

    sf::IntRect rect2;
    auto b = map.getBuildingsIt();
    for(; b.first !=  b.second; b.first++){
        rect2 = b.first->getPosition();
        if (rect1.left < rect2.left + rect2.width &&
            rect1.left + rect1.width > rect2.left &&
            rect1.top < rect2.top + rect2.height &&
            rect1.height + rect1.top > rect2.top) {
            isConstructible = false;
            break;
        }
    }

    if (!isConstructible) return;
    float r = rotate;
    if (r == -1) r = 0;

    BuildMap build(mBuild_selection, rect1,r);
    float blockSize = map.getBlockSize();
    if(r == 90) rect1.left += rect1.width;
    sf::Sprite &tmp = build.getSprite();
    tmp.setTexture(getContext().textures.get(Textures::Builds));
    tmp.setTextureRect(spritePos);
    tmp.setScale(blockSize/30.f,blockSize/30.f);
    tmp.setPosition(rect1.left*blockSize + map.getOrigins().x ,rect1.top*blockSize + map.getOrigins().y);
    tmp.setRotation(r);

    map.addBuildings(build);
}

void MapEditorState::supressBuildings(sf::Vector2i pos) {
    sf::IntRect rect2;
    auto b = map.getBuildingsIt();
    for(; b.first !=  b.second; b.first++){
        rect2 = b.first->getPosition();
        if (pos.x < rect2.left + rect2.width &&
            pos.x >= rect2.left &&
            pos.y < rect2.top + rect2.height &&
            pos.y >= rect2.top) {
            map.supBuildings(b.first);
            break;
        }
    }
}

void MapEditorState::setBuildings(){
    auto b = map.getBuildingsIt();
    sf::IntRect spritePos;
    sf::IntRect rect;
    float rot;
    for(;b.first != b.second; b.first++){
        rect = b.first->getPosition();
        rot = b.first->mRotation;
        switch(b.first->getID()){
            case Textures::building::Castle :  rect.width = 3; rect.height = 3;
                spritePos = {0,0,90,90};
                break;

            case Textures::building::Home :  rect.width = 2; rect.height = 2;
                spritePos = {90,0,60,60};
                break;

            case Textures::building::Barrier :
                spritePos = {90,60,60,30};
                if (rot == 0 or rot == -1){
                    rect.width = 2;
                    rect.height = 1;
                }
                else{
                    rect.width = 1;
                    rect.height = 2;
                }
                break;

            default : return;
        }
        b.first->setPosition(rect);

        if(rot == 90) rect.left += rect.width;
        sf::Sprite &tmp = b.first->getSprite();
        float blockSize = map.getBlockSize();
        tmp.setTexture(getContext().textures.get(Textures::Builds));
        tmp.setTextureRect(spritePos);
        tmp.setScale(blockSize/30.f,blockSize/30.f);
        tmp.setPosition(rect.left*blockSize+map.getOrigins().x,rect.top*blockSize+map.getOrigins().y);
        tmp.setRotation(rot);
    }
}