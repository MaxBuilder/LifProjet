//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_RESOURCEHOLDER_HPP
#define LIFPROJET_RESOURCEHOLDER_HPP

#include <stdexcept>
#include <cassert>
#include <string>
#include <memory>
#include <map>

/**
 * @class ResourceHolder
 * @brief Structure used to associate a resource to and identifier
 * @tparam Resource
 * @tparam Identifier
 * @author SFML Game Development
 */
template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
    /**
     * @brief Links a resource from a file path to an identifier
     * @param id Identifier of the resource
     * @param filename Path to the resource
     */
    void load(Identifier id, const std::string& filename);

    /**
     * @brief Links a resource from a file path to an identifier
     * @param id Identifier of the resource
     * @param filename Path to the resource
     * @param secondParam Second parameter if needed
     */
    template <typename Parameter>
    void load(Identifier id, const std::string& filename, const Parameter& secondParam);

    /**
     * @brief Retrieves the resource from an identifier
     * @param id Identifier of the resource to fetch
     * @return Reference to the resource
     */
    Resource& get(Identifier id);
    /**
     * @brief Retrieves the resource from an identifier
     * @param id Identifier of the resource to fetch
     * @return Const reference to the resource
     */
    const Resource&	get(Identifier id) const;


private:
    /**
     * @brief Helper function to insert the resource
     * @param id Identifier of the resource
     * @param resource Pointer to the resource allocation
     */
    void insertResource(Identifier id, std::unique_ptr<Resource> resource);


private:
    std::map<Identifier, std::unique_ptr<Resource>>	mResourceMap;
};


template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

    // If loading successful, insert resource to Editor
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename, secondParam))
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

    // If loading successful, insert resource to Editor
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());

    return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
    // Insert and check success
    auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

#endif //LIFPROJET_RESOURCEHOLDER_HPP
