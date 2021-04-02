//
// Created by thibaut on 01/02/2021.
//

#include "MapEditorState.hpp"

MapEditorState::MapEditorState(StateStack &stack, Context context)
        : State(stack, context)
        , map(context.textures.get(Textures::MapGround), context.textures.get(Textures::EditorAllSoldiers), 16.f)
        , mPaletteBar(sf::IntRect(1130,100,150,550),4)
        , subBackground(getContext().textures.get(Textures::SubBackground))
{
    mBuild_selection = Buildings::None;
    background.setTexture(context.textures.get(Textures::EditorBackground));
    map.setPosition(92,90);

    mapPath.setFont(context.fonts.get(Fonts::Main));
    mapPath.setPosition(8, 684);
    mapPath.setCharacterSize(20u);

    rotate = 0.f;
    MapEditorState::mMapPath = "Unsaved";
    ground_selection = sf::Vector2i (1,0);
    lastGround = sf::Vector2i (0,0);
    lastTileUpdate = {-1,-1};

    tool = Editor::Tool::Standard;

    // Edit buttons :

    auto backButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::EditorBackButton);
    backButton->setPosition(16, 10);
    backButton->setCallback([this](){
        requestStackPop();
        requestStackPush(States::MainMenu);
        getContext().sounds.play(Sounds::Menu);
    });
    mEditBar.pack(backButton);

    auto newButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::EditorNewButton);
    newButton->setPosition(98, 10);
    newButton->setCallback([this] () {
        map.clear();
        getContext().sounds.play(Sounds::Menu);
    });
    mEditBar.pack(newButton);

    auto saveButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::EditorSaveButton);
    saveButton->setPosition(170, 10);
    saveButton->setCallback([this] () {
        subMenu = true;
        saveload = true;
        getContext().sounds.play(Sounds::Menu);
    });
    mEditBar.pack(saveButton);

    auto loadButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::EditorLoadButton);
    loadButton->setPosition(242, 10);
    loadButton->setCallback([this] () {
        subMenu = true;
        saveload = false;
        getContext().sounds.play(Sounds::Menu);
    });
    mEditBar.pack(loadButton);

    auto undoButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::EditorUndoButton);
    undoButton->setPosition(324, 10);
    undoButton->setCallback([this] () {
        getContext().sounds.play(Sounds::Menu);
    });
    mEditBar.pack(undoButton);

    auto redoButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::EditorRedoButton);
    redoButton->setPosition(396, 10);
    redoButton->setCallback([this] () {
        getContext().sounds.play(Sounds::Menu);
    });
    mEditBar.pack(redoButton);

    // Soldier selection :

    auto soldier = std::make_shared<GUI::Button>(context, 60, 60, Textures::EditorSoldier);
    soldier->setToggle(true);
    soldier->setPosition(495, 10);
    soldier->activate();
    soldier->setCallback([this] () {
        mSoldier = Editor::Entity::Soldier;
        getContext().sounds.play(Sounds::Menu);
    });
    mSoldierBar.pack(soldier);

    // Tool bar buttons :

    auto smallBrushButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::ToolSmallBrush);
    smallBrushButton->setPosition(16, 98);
    smallBrushButton->setToggle(true);
    smallBrushButton->activate();
    smallBrushButton->setCallback([this](){
        tool = Editor::Tool::Standard;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(smallBrushButton);

    auto mediumBrushButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::ToolMediumBrush);
    mediumBrushButton->setPosition(16, 164);
    mediumBrushButton->setToggle(true);
    mediumBrushButton->setCallback([this](){
        tool = Editor::Tool::Square3;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(mediumBrushButton);

    auto eraserButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::ToolEraser);
    eraserButton->setPosition(16, 230);
    eraserButton->setToggle(true);
    eraserButton->setCallback([this](){
        tool = Editor::Tool::Eraser;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(eraserButton);

    auto fillButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::ToolFill);
    fillButton->setPosition(16, 296);
    fillButton->setToggle(true);
    fillButton->setCallback([this](){
        tool = Editor::Tool::Fill;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(fillButton);

    auto blue_team = std::make_shared<GUI::Button>(context, 60, 60, Textures::EditorBlueTeam);
    blue_team->setToggle(true);
    blue_team->setPosition(16, 362);
    blue_team->setCallback([this] () {
        tool = Editor::Tool::BlueTeam;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(blue_team);

    auto red_team = std::make_shared<GUI::Button>(context, 60, 60, Textures::EditorRedTeam);
    red_team->setToggle(true);
    red_team->setPosition(16, 428);
    red_team->setCallback([this] () {
        tool = Editor::Tool::RedTeam;
        getContext().sounds.play(Sounds::Menu);
    });
    mToolBar.pack(red_team);

    // Rotation bar buttons :

    auto rotateUpButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::EditorRotateUpButton);
    rotateUpButton->setPosition(16, 532);
    rotateUpButton->setToggle(true);
    rotateUpButton->activate();
    rotateUpButton->setCallback([this] () {
        rotate = 0.f;
        getContext().sounds.play(Sounds::Menu);
    });
    mRotationBar.pack(rotateUpButton);

    auto rotateRightButton = std::make_shared<GUI::Button>(context, 60, 60, Textures::EditorRotateRightButton);
    rotateRightButton->setPosition(16, 598);
    rotateRightButton->setToggle(true);
    rotateRightButton->setCallback([this] () {
        rotate = 90.f;
        getContext().sounds.play(Sounds::Menu);
    });
    mRotationBar.pack(rotateRightButton);

    // Texture selection buttons :

    for(int y(0); y < 46; y++){
        for (int x(0); x < 3;x++){

            addButtonTexture(sf::Vector2i(x,y), sf::Vector2i(1134+44*x,100+44*y));
        }
    }

    // Boutons du sous-menu :

    auto map1 = std::make_shared<GUI::Button>(context, 500, 70, Textures::MenuButton);
    map1->setPosition(380, 200);
    map1->setText("Save 1");
    map1->setCallback([this] () {
        mMapPath = "data/Maps/demo1.map";
        subMenu = false;
        if(saveload) map.save(MapEditorState::mMapPath);
        else {
            map.load(MapEditorState::mMapPath);
        }
        getContext().sounds.play(Sounds::Menu);
    });
    mSubMenu.pack(map1);

    auto map2 = std::make_shared<GUI::Button>(context, 500, 70, Textures::MenuButton);
    map2->setPosition(380, 280);
    map2->setText("Save 2");
    map2->setCallback([this] () {
        mMapPath = "data/Maps/demo2.map";
        subMenu = false;
        if(saveload) map.save(MapEditorState::mMapPath);
        else {
            map.load(MapEditorState::mMapPath);
        }
        getContext().sounds.play(Sounds::Menu);
    });
    mSubMenu.pack(map2);

    auto map3 = std::make_shared<GUI::Button>(context, 500, 70, Textures::MenuButton);
    map3->setPosition(380, 360);
    map3->setText("Save 3");
    map3->setCallback([this] () {
        mMapPath = "data/Maps/demo3.map";
        subMenu = false;
        if(saveload) map.save(MapEditorState::mMapPath);
        else {
            map.load(MapEditorState::mMapPath);
        }
        getContext().sounds.play(Sounds::Menu);
    });
    mSubMenu.pack(map3);

    auto ret = std::make_shared<GUI::Button>(context, 500, 70, Textures::MenuButton);
    ret->setPosition(380, 500);
    ret->setText("Back");
    ret->setCallback([this] () {
        subMenu = false;
        getContext().sounds.play(Sounds::Menu);
    });
    mSubMenu.pack(ret);

}

void MapEditorState::draw() {
    sf::RenderWindow& window = getContext().window;

    window.draw(background);
    window.draw(map);
    window.draw(mEditBar);
    window.draw(mSoldierBar);
    window.draw(mToolBar);
    window.draw(mPaletteBar);
    window.draw(mRotationBar);

    mapPath.setString(mMapPath);
    window.draw(mapPath);

    if(subMenu) {
        window.draw(subBackground);
        window.draw(mSubMenu);
    }

    // Ajouter position
}

bool MapEditorState::update(sf::Time dt) {
    return true;
}

bool MapEditorState::handleEvent(const sf::Event& event) {

    if(subMenu) {
        mSubMenu.handleEvent(event, getContext().window);
        return false;
    }

    if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::L){
        auto build = map.getBuildingsIt();
        for (; build.first != build.second; build.first++)
            std::cout<<build.first->getID();
        std::cout<<std::endl;
    }

    mEditBar.handleEvent(event, getContext().window);
    mSoldierBar.handleEvent(event, getContext().window);
    mToolBar.handleEvent(event, getContext().window);
    mPaletteBar.handleEvent(event, getContext().window);
    mRotationBar.handleEvent(event, getContext().window);

    if (event.type == sf::Event::MouseMoved or event.type == sf::Event::MouseButtonPressed){

        sf::Vector2i WindowPosition = sf::Mouse::getPosition(getContext().window);
        sf::Vector2i pos = static_cast<sf::Vector2i>( getContext().window.mapPixelToCoords(WindowPosition));
        if (pos.x < 1116 and pos.x >= 92 and pos.y >= 90 and pos.y < 666 ){ // rectangle contenant la Editor

            auto caseSize = map.getBlockSize();
            sf::Vector2f origin = map.getPosition();

            pos.y = pos.y - origin.y;
            pos.x = pos.x - origin.x;
            pos.y = pos.y/caseSize;
            pos.x = pos.x/caseSize;

            if (mBuild_selection != Buildings::None){
                if (event.mouseButton.button == sf::Mouse::Left)
                    createBuildings(pos);
                else if (event.mouseButton.button == sf::Mouse::Right)
                    supressBuildings(pos);
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

                if (lastTileUpdate == pos and lastGround == ground_selection)
                    return false;
                else {
                    lastTileUpdate = pos;
                    lastGround = ground_selection;
                }

                switch (tool) {

                    case Editor::Tool::Standard :
                        map.getTile(pos.x,pos.y).paint(ground_selection,rotate);
                        break;

                    case Editor::Tool::Square3 :
                        paintSquare3(pos);
                        break;

                    case Editor::Tool::Fill :
                        paintFill(pos);
                        break;

                    case Editor::Tool::Eraser :
                        if(not supressSoldier(pos))
                            map.getTile(pos.x,pos.y).paint(sf::Vector2i (0,0),0);
                        break;

                    case Editor::Tool::BlueTeam :
                    case Editor::Tool::RedTeam :
                        createSoldier(pos);
                        break;

                    default :
                        break;
                }
            }
        }
    }
    return false;
}

void MapEditorState::addButtonTexture(sf::Vector2i id, sf::Vector2i pos){


    auto button = std::make_shared<GUI::ButtonTexture>(getContext(), 40, 40, Textures::MapGround,id);
    button->setPosition(pos.x, pos.y);
    button->setToggle(true);
    button->setCallback([this](sf::Vector2i bId){

        Buildings::ID buildId = Buildings::None;
        if ((bId.y == 37 or bId.y == 36) and (bId.x == 0 or bId.x == 1)) {
            bId.y = 36; bId.x = 0;
            buildId = Buildings::RedVillage;
        }
        else if ((bId.y == 36 or bId.y == 37) and bId.x == 2) {
            buildId = Buildings::RedBarrier;
        }
        else if ((bId.y == 39 or bId.y == 38) and (bId.x == 0 or bId.x == 1)){
            bId.y = 38; bId.x = 0;
            buildId = Buildings::BlueVillage;
        }
        else if ((bId.y == 39 or bId.y == 38) and bId.x == 2) {
            buildId = Buildings::BlueBarrier;
        }
        else if(bId.y >= 40 and bId.y <= 42) {
            bId.y = 40; bId.x = 0;
            buildId = Buildings::RedCastle;
        }
        else if(bId.y >= 43) {
            bId.y = 43; bId.x = 0;
            buildId = Buildings::BlueCastle;
        }

        ground_selection = bId;
        mBuild_selection = buildId;
        getContext().sounds.play(Sounds::Menu);
    });
    mPaletteBar.pack(button);

}

void MapEditorState::paintSquare3(sf::Vector2i pos){
    map.getTile(pos.x,pos.y).paint(ground_selection,rotate);

    if (pos.x > 0){
        map.getTile(pos.x-1,pos.y).paint(ground_selection,rotate);
        if (pos.y > 0)
            map.getTile(pos.x-1,pos.y-1).paint(ground_selection,rotate);
        if (pos.y < 35)
            map.getTile(pos.x-1,pos.y+1).paint(ground_selection,rotate);
    }
    if(pos.x < 63){
        map.getTile(pos.x+1,pos.y).paint(ground_selection,rotate);
        if (pos.y > 0)
            map.getTile(pos.x+1,pos.y-1).paint(ground_selection,rotate);
        if (pos.y < 35)
            map.getTile(pos.x+1,pos.y+1).paint(ground_selection,rotate);
    }
    if (pos.y > 0)
        map.getTile(pos.x,pos.y-1).paint(ground_selection,rotate);
    if (pos.y < 35)
        map.getTile(pos.x,pos.y+1).paint(ground_selection,rotate);

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

    map.getTile(co.x,co.y).paint(ground_selection,rotate);

}

void MapEditorState::createBuildings(sf::Vector2i pos){
    sf::IntRect rect1 = {pos.x, pos.y, 0, 0};
    bool construtible = true;

    if (mBuild_selection == Buildings::RedBarrier or mBuild_selection == Buildings::BlueBarrier){
        rect1.height = 1;
        rect1.width = 1;
    }else if (mBuild_selection == Buildings::RedVillage or mBuild_selection == Buildings::BlueVillage){
        rect1.height = 2;
        rect1.width = 2;
    }else if (mBuild_selection == Buildings::RedCastle or mBuild_selection == Buildings::BlueCastle){
        rect1.height = 3;
        rect1.width = 3;
    }
    for (int y(pos.y); y < pos.y+rect1.height;y++){
        for (int x(pos.x); x < pos.x+rect1.width;x++){
            if (x >= 64 or y >= 36) {
                std::cout<<" hors limite\n";
                construtible = false;
                break;
            }else if (map.getTile(x, y).haveTop()) {
                construtible = false;
                break;
            }
        }
        if(!construtible)
            break;
    }

    if (construtible){

        for (int y(0); y < rect1.height;y++){
            for (int x(0); x <rect1.width;x++){
                map.getTile(x+pos.x,y+pos.y).paint(sf::Vector2i(ground_selection.x+x,ground_selection.y+y),0);
            }
        }

        map.addBuildings(BuildInfo(mBuild_selection, rect1));
    }
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

            for (int y(0); y < b.first->getPosition().height;y++){
                for (int x(0); x <b.first->getPosition().width;x++){
                    map.getTile(x+b.first->getPosition().left,y+b.first->getPosition().top).paint(sf::Vector2i(0,0),0);
                }
            }

            map.supBuildings(b.first);
            break;
        }
    }
}

void MapEditorState::createSoldier(sf::Vector2i pos){
    std::cout<<"enter Create\n";
    if(not map.getTile(pos).isCrossable()) return;

    auto posf = static_cast<sf::Vector2f>(pos);
    auto other = map.getEntitiesIt();

    for(;other.first != other.second; other.first++)
        if(other.first->getPosition() == posf) return;

    map.addEntity(EntityInfo(posf,mSoldier,tool));
    std::cout<<"exit Create\n";
}

bool MapEditorState::supressSoldier(sf::Vector2i pos){
    bool ret = false;
    auto posf = static_cast<sf::Vector2f>(pos);
    auto other = map.getEntitiesIt();

    for(;other.first != other.second; other.first++)
        if(other.first->getPosition() == posf){
            map.supEntity(other.first);
            ret = true;
            break;
        }

    return ret;
}