//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_RESOURCEIDENTIFIERS_HPP
#define LIFPROJET_RESOURCEIDENTIFIERS_HPP

namespace sf
{
    class Texture;
    class Font;
    class SoundBuffer;
}

namespace Textures {
    enum ID {
        None,

        SubBackground,
        MenuBackground1,
        MenuBackground2,
        MenuBackground3,
        MenuBackground4,
        MenuTitle,
        MenuButton,
        SettingsButton,
        Checkbox,

        MapGround,
        MapBuildings,

        EditorBackground,
        EditorBackButton,
        ToolSmallBrush,
        ToolMediumBrush,
        ToolBigBrush,
        ToolFill,
        ToolEraser,
        EditorBlueTeam,
        EditorRedTeam,
        EditorSoldier,
        EditorAllSoldiers,
        EditorNewButton,
        EditorLoadButton,
        EditorSaveButton,
        EditorUndoButton,
        EditorRedoButton,
        EditorGrassButton,
        EditorSandButton,
        EditorStoneButton,
        EditorWoodButton,
        EditorWaterButton,
        EditorBarrierButton,
        EditorHomeButton,
        EditorCastleButton,
        EditorRotateUpButton,
        EditorRotateRightButton,

        EntitySoldier,
        EntityGlow
    };
}

namespace Buildings {
    enum ID {
        None, RedCastle, BlueCastle, RedVillage, BlueVillage, RedBarrier, BlueBarrier
    };
}


namespace Fonts {
    enum ID {
        Main
    };
}

namespace Sounds {
    enum ID {
        Menu,
    };
}

namespace Editor {
    enum Tool {
        Standard, Square3, Fill, Eraser, BlueTeam, RedTeam
    };

    enum Entity{
        Soldier, Archer, Tank
    };
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID> SoundBufferHolder;

#endif //LIFPROJET_RESOURCEIDENTIFIERS_HPP
