//
//  Layer.hpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#ifndef Layer_hpp
#define Layer_hpp

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Drawable.hpp"
#include "Updatable.hpp"

namespace mz {

class Layer : public Drawable, public Updatable, public sf::Transformable {
public:
    Layer() = default;
    Layer(Layer const& layer) = delete;
    Layer(Layer && layer) noexcept = default;
    
    Layer& operator=(Layer const& layer) = delete;
    Layer& operator=(Layer && layer) noexcept = default;
    
    virtual ~Layer() = default;
    
    template <class LayerT>
    LayerT& addChild(std::unique_ptr<LayerT> && layer);
    
    virtual void draw(Camera const& camera) override;
    
    std::unique_ptr<Layer> extractChild(std::size_t childId);
    
    std::unique_ptr<Layer> extractChild(Layer & layer);
    
    std::unique_ptr<Layer> extractFromParent();
    
    inline Layer& getChild(std::size_t childId);
    
    inline std::size_t getChildsCount() const;
    
    inline sf::Transformable const& getGlobalTransform() const;

    inline std::size_t const& getIdInParentHierarchy() const;
    
    inline sf::Transformable const& getRelativeTransform() const;
    
    inline Layer* getParent();
    
    sf::Transform const& getTransform() const = delete;
    
    inline void removeAllChildrens();
    
    inline void removeChild(std::size_t childId);
    
    inline void removeChild(Layer & layer);
    
    inline void removeFromParent();
    
    virtual void update(std::uint64_t timeElapsed) override;
    
private:
    void clearAddBuffer();
    
    void clearRemoveBuffer();
    
    void stableRemoveChild(std::size_t childId);
    
private:
    std::vector<std::unique_ptr<Layer>>         m_childs;
    sf::Transformable                           m_globalTransform; // TODO: fix rotation
    std::size_t                                 m_idInParentHierarchy = 0;
    mutable Layer*                              m_parent;
    sf::Transformable                           m_relativeTransform;
    mutable std::vector<std::unique_ptr<Layer>> m_toAddChilds;
    mutable std::vector<std::size_t>            m_toRemoveChilds;
};

}

#include "Layer.inl"

#endif /* Layer_hpp */
