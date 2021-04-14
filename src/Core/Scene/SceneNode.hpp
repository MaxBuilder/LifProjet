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

/**
 * @class SceneNode
 * @brief Element of a scene that can be rendered to the screen and can have children
 * @author SFML Game Development
 */
class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable {
public:
    typedef std::unique_ptr<SceneNode> Ptr;
    typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
    /**
     * @brief Default constructor
     */
    explicit SceneNode();

    /**
     * @brief Attachs a child the node
     * @param child Child to attach
     */
    void attachChild(Ptr child);
    /**
     * @brief Finds an detached a child of the node
     * @param node Node to detach
     * @return Detached node
     */
    Ptr detachChild(const SceneNode* node);

    /**
     * @brief Updates the node and all it's children
     * @param dt
     */
    void update(sf::Time dt);

    /**
     * @brief Returns the bounds of the node
     * @return Bounds
     */
    virtual sf::FloatRect getBoundingRect() const;

private:
    /**
     * @brief Updates the current node
     * @param dt Time interval since last update
     */
    virtual void updateCurrent(sf::Time dt);
    /**
     * @brief Updates the node's children
     * @param dt Time interval since last update
     */
    void updateChildren(sf::Time dt);

    /**
     * Draws the node and all it's children to the target
     * @param target Target to render the object to
     * @param states Informations on how to render the object
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    /**
     * Draws the current node to the target
     * @param target Target to render the object to
     * @param states Informations on how to render the object
     */
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    /**
     * Draws the node's children to the target
     * @param target Target to render the object to
     * @param states Informations on how to render the object
     */
    void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

    /**
     * @brief Draws the node's bounds
     * @param target Target to render the object to
     * @param states Informations on how to render the object
     */
    void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
    std::vector<Ptr> mChildren;
    SceneNode* mParent;

};


#endif //LIFPROJET_SCENENODE_HPP
