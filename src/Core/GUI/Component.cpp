//
// Created by 33771 on 01/02/2021.
//

#include "Component.hpp"

namespace GUI
{

    Component::Component()
    : mIsSelected(false)
    , mIsActive(false)
    ,mHeight(0)
    ,mWidth(0)
    {}

    Component::~Component() {
    }

    float Component::getWidth() const {
        return mWidth;
    }

    float Component::getHeight() const {
        return mHeight;
    }

    bool Component::isSelected() const {
        return mIsSelected;
    }

    void Component::select() {
        mIsSelected = true;
    }

    void Component::deselect() {
        mIsSelected = false;
    }

    bool Component::isActive() const {
        return mIsActive;
    }

    void Component::activate() {
        mIsActive = true;
    }

    void Component::deactivate() {
        mIsActive = false;
    }

}