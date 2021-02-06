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
        MenuTitle,
        MenuButton,
        Map
    };

    namespace ground {
        enum ID {
            None, Grass, Wood, Sand, Wall, Water
        };
    }

    namespace building {
        enum ID {
            None, Barrier
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
    enum mode {
        mapEditor, game, preview
    };
    enum tool {
        standard, square3, circle5, fill, undo
    };
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID> SoundBufferHolder;

#endif //LIFPROJET_RESOURCEIDENTIFIERS_HPP
