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
        Background1,
        Background2,
        Background3,
        Background4,
        MapEditorBackGround,
        MenuTitle,
        MenuButton,
        MapEditorButton,
        Map,
        Builds,
        // MapEditor
        Back,
        SmallBrush,
        MediumBrush,
        BigBrush,
        Fill,
        Eraser,
        New,
        Load,
        Save,
        Undo,
        Redo,
        Grass,
        Sand,
        Stone,
        Wood,
        Water,
        Barrier,
        Home,
        Castle,
        RotateUp,
        RotateRight,
        SubEditorBackground

    };

    namespace ground {
        enum ID {
            None, Grass, Wood, Sand, Wall, Water
        };
    }

    namespace building {
        enum ID {
            None, Barrier, Castle, Home
        };
    }
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

namespace map{
    enum tool {
        standard, square3, circle5, fill, eraser, undo
    };
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID> SoundBufferHolder;

#endif //LIFPROJET_RESOURCEIDENTIFIERS_HPP
