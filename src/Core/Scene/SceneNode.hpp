//
// Created by 33771 on 13/02/2021.
//

#ifndef LIFPROJET_SCENENODE_HPP
#define LIFPROJET_SCENENODE_HPP

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <set>
#include <memory>
#include <utility>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable {
public:
    typedef std::unique_ptr<SceneNode> Ptr;
    typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
    explicit SceneNode();

    void attachChild(Ptr child);
    Ptr detachChild(const SceneNode* node);

    void update(sf::Time dt);

    //sf::Vector2f getWorldPosition();
    virtual sf::FloatRect getBoundingRect() const;

private:
    virtual void updateCurrent(sf::Time dt);
    void updateChildren(sf::Time dt);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
    std::vector<Ptr> mChildren;
    SceneNode* mParent;

};


#endif //LIFPROJET_SCENENODE_HPP
