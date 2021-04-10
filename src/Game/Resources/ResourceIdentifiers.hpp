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
        SettingsButton,
        MenuButton,

        SettingsBack,
        SettingsBackground,
        SettingsSave,
        SettingsApply,
        Checkbox,

        MapGround,

        EditorBackground,
        EditorBackButton,
        ToolSmallBrush,
        ToolMediumBrush,
        ToolBigBrush,
        ToolFill,
        ToolEraser,
        EditorTeamColor,
        EditorKnight,
        EditorArcher,
        EditorTank,
        EditorBuilding,
        EditorAllSoldiers,
        EditorNewButton,
        EditorLoadButton,
        EditorSaveButton,
        EditorUndoButton,
        EditorRedoButton,
        EditorRotateUpButton,
        EditorRotateRightButton,

        EntityKnightBlue,
        EntityKnightRed,
        EntityArcherBlue,
        EntityArcherRed,
        EntityPaladinBlue,
        EntityPaladinRed,
        EntityGlow,

        GamePause,
        GameSlow,
        Game1x,
        Game3x,
        Game5x,
        GameTimeUI,
        GameMapSelectionButton,
        GameMapSelectionConfButton,
        GameMapSelectionBackground,
        GameVictoryScreen
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


template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID> SoundBufferHolder;

#endif //LIFPROJET_RESOURCEIDENTIFIERS_HPP
