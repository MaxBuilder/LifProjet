//
// Created by thibaut on 01/02/2021.
//

#include "MapEditorState.hpp"

MapEditorState::MapEditorState(StateStack &stack, Context context)
        : State(stack, context)
        , map(context.textures.get(Textures::Map), 24.f,sf::Vector2i(0,108))
        , view(sf::FloatRect (0,0,1920,1080)){

    view.setViewport(sf::FloatRect(0, 0, 1, 1));
    background.setTexture(context.textures.get(Textures::MapEditorBackGround));
    map.setDrawBuildings(true);

    rotate = 0.f;
    mMapPath = std::string("data/Maps/demo1.map");
    ground_selection = Textures::ground::Grass;
    lastGround = Textures::ground::None;
    lastTileUpdate = {-1,-1};

    tool = map::tool::standard;

    auto backButton = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    backButton->setPosition(30, 54-70.f/2);
    backButton->setText("Back");
    backButton->setCallback([this](){
        requestStackPop();
        requestStackPush(States::MainMenu);
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(backButton);

    auto standardButton = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    standardButton->setPosition(768-(15+2*130), 1026-70.f/2);
    standardButton->setText("Standard");
    standardButton->setToggle(true);
    standardButton->setCallback([this](){
        tool = map::tool::standard;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(standardButton);

    auto square3x3Button = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    square3x3Button->setPosition(768-(5+130),1026-70.f/2 );
    square3x3Button->setText("Square3x3");
    square3x3Button->setToggle(true);
    square3x3Button->setCallback([this](){
        tool = map::tool::square3;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(square3x3Button);

    auto circle5x5Button = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    circle5x5Button->setPosition(768+5,1026-70.f/2 );
    circle5x5Button->setText("Circle5x5");
    circle5x5Button->setToggle(true);
    circle5x5Button->setCallback([this](){
        tool = map::tool::circle5;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(circle5x5Button);

    auto fillButton = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    fillButton->setPosition(768+15+130,1026-70.f/2 );
    fillButton->setText("Fill");
    fillButton->setToggle(true);
    fillButton->setCallback([this](){
        tool = map::tool::fill;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(fillButton);

    auto loadButton = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    loadButton->setPosition(30,1026-70.f/2 );
    loadButton->setText("Load");
    loadButton->setCallback([this](){
        map.load(mMapPath);
        setBuildings();
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(loadButton);

    auto saveButton = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    saveButton->setPosition(30+10+130,1026-70.f/2 );
    saveButton->setText("Save");
    saveButton->setCallback([this](){
        map.save(mMapPath);
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(saveButton);

    auto rotateAleaButton = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    rotateAleaButton->setPosition(1536-130-30,1026-70.f/2 );
    rotateAleaButton->setText("0-90-180°");
    rotateAleaButton->setToggle(true);
    rotateAleaButton->setCallback([this](){
        rotate = -1;
        getContext().sounds.play(Sounds::Menu);
    });
    mTextureRotation.pack(rotateAleaButton);

    auto rotate90Button = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    rotate90Button->setPosition(1536-2*130-30-10,1026-70.f/2 );
    rotate90Button->setText("90°");
    rotate90Button->setToggle(true);
    rotate90Button->setCallback([this](){
        rotate = 90.f;
        getContext().sounds.play(Sounds::Menu);
    });
    mTextureRotation.pack(rotate90Button);

    auto rotate0Button = std::make_shared<GUI::Button>(context, 130, 70, Textures::MapEditorButton);
    rotate0Button->setPosition(1536-3*130-30-2*10,1026-70.f/2 );
    rotate0Button->setText("0°");
    rotate0Button->setToggle(true);
    rotate0Button->setCallback([this](){
        rotate = 0.f;
        getContext().sounds.play(Sounds::Menu);
    });
    mTextureRotation.pack(rotate0Button);

    auto grassButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::MapEditorButton);
    grassButton->setPosition(1536+29,108+29 );
    grassButton->setText("grass");
    grassButton->setToggle(true);
    grassButton->setCallback([this](){
        ground_selection = Textures::ground::Grass;
        mBuild_selection = Textures::building::None;
        getContext().sounds.play(Sounds::Menu);
    });
    mPalette.pack(grassButton);

    auto sandButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::MapEditorButton);
    sandButton->setPosition(1536+2*29+60,108+29 );
    sandButton->setText("sand");
    sandButton->setToggle(true);
    sandButton->setCallback([this](){
        ground_selection = Textures::ground::Sand;
        mBuild_selection = Textures::building::None;
        getContext().sounds.play(Sounds::Menu);
    });
    mPalette.pack(sandButton);

    auto woodButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::MapEditorButton);
    woodButton->setPosition(1536+3*29+2*60,108+29 );
    woodButton->setText("wood");
    woodButton->setToggle(true);
    woodButton->setCallback([this](){
        ground_selection = Textures::ground::Wood;
        mBuild_selection = Textures::building::None;
        getContext().sounds.play(Sounds::Menu);
    });
    mPalette.pack(woodButton);

    auto wallButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::MapEditorButton);
    wallButton->setPosition(1536+4*29+3*60,108+29);
    wallButton->setText("wall");
    wallButton->setToggle(true);
    wallButton->setCallback([this](){
        ground_selection = Textures::ground::Wall;
        mBuild_selection = Textures::building::None;
        getContext().sounds.play(Sounds::Menu);
    });
    mPalette.pack(wallButton);

    auto waterButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::MapEditorButton);
    waterButton->setPosition(1536+29,108 + 2*29+60 );
    waterButton->setText("water");
    waterButton->setToggle(true);
    waterButton->setCallback([this](){
        ground_selection = Textures::ground::Water;
        mBuild_selection = Textures::building::None;
        getContext().sounds.play(Sounds::Menu);
    });
    mPalette.pack(waterButton);

    auto CastleButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::MapEditorButton);
    CastleButton->setPosition(1536+29,108 + 3*29+2*60 );
    CastleButton->setText("Castle");
    CastleButton->setToggle(true);
    CastleButton->setCallback([this](){
        ground_selection = Textures::ground::None;
        mBuild_selection = Textures::building::Castle;
        getContext().sounds.play(Sounds::Menu);
    });
    mPalette.pack(CastleButton);

    auto HomeButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::MapEditorButton);
    HomeButton->setPosition(1536+2*29+60,108 + 3*29+2*60 );
    HomeButton->setText("Home");
    HomeButton->setToggle(true);
    HomeButton->setCallback([this](){
        ground_selection = Textures::ground::None;
        mBuild_selection = Textures::building::Home;
        getContext().sounds.play(Sounds::Menu);
    });
    mPalette.pack(HomeButton);

    auto BarrierButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::MapEditorButton);
    BarrierButton->setPosition(1536+3*29+2*60,108 + 3*29+2*60 );
    BarrierButton->setText("Barrier");
    BarrierButton->setToggle(true);
    BarrierButton->setCallback([this](){
        ground_selection = Textures::ground::None;
        mBuild_selection = Textures::building::Barrier;
        getContext().sounds.play(Sounds::Menu);
    });
    mPalette.pack(BarrierButton);

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
    mCurrentMap.pack(map3Button);
}

void MapEditorState::draw() {

    sf::RenderWindow& window = getContext().window;
    window.setView(view);
    window.draw(background);
    window.draw(map);
    window.draw(mToolBar);
    window.draw(mTextureRotation);
    window.draw(mPalette);
    window.draw(mCurrentMap);
}

bool MapEditorState::update(sf::Time dt) {
    return true;
}

bool MapEditorState::handleEvent(const sf::Event& event) {

    getContext().window.setView(view);

    mToolBar.handleEvent(event, getContext().window);
    mTextureRotation.handleEvent(event,getContext().window);
    mPalette.handleEvent(event, getContext().window);
    mCurrentMap.handleEvent(event, getContext().window);

    if (event.type == sf::Event::MouseMoved or event.type == sf::Event::MouseButtonPressed){

        sf::Vector2i WindowPosition = sf::Mouse::getPosition(getContext().window);
        sf::Vector2u  size = getContext().window.getSize();

        if (WindowPosition.x < size.x*0.8f and WindowPosition.x >= 0 and WindowPosition.y >= size.y*0.1 and
            WindowPosition.y < size.y*0.9 ){ // rectangle contenant la map

            sf::Vector2i pos = static_cast<sf::Vector2i>( getContext().window.mapPixelToCoords(WindowPosition));

            auto caseSize = map.getBlockSize();
            sf::Vector2i origin = map.getOrigins();

            pos.y = pos.y - origin.y;
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
    if(r == 90) rect1.left += rect1.width;
    sf::Sprite &tmp = build.getSprite();
    tmp.setTexture(getContext().textures.get(Textures::Builds));
    tmp.setTextureRect(spritePos);
    tmp.setScale(24.f/30.f,24.f/30.f);
    tmp.setPosition(rect1.left*24.f,rect1.top*24.f+108.f);
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
        tmp.setTexture(getContext().textures.get(Textures::Builds));
        tmp.setTextureRect(spritePos);
        tmp.setScale(24.f/30.f,24.f/30.f);
        tmp.setPosition(rect.left*24.f,rect.top*24.f+108.f);
        tmp.setRotation(rot);
    }
}
