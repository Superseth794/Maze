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
    
    Layer& addChild(Layer && layer);
    
    virtual void draw(sf::RenderTarget& target) override;
    
    std::unique_ptr<Layer> extractChild(std::size_t childId);
    
    std::unique_ptr<Layer> extractChild(Layer & layer);
    
    std::unique_ptr<Layer> extractFromParent();
    
    inline Layer& getChild(std::size_t childId);
    
    inline std::size_t getChildsCount() const;

    inline std::size_t const& getIdInParentHierarchy() const;
    
    inline Layer* getParent();
    
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
    std::size_t                                         m_idInParentHierarchy = 0;
    mutable Layer*                                      m_parent;
    std::vector<std::unique_ptr<Layer>>                 m_childs;
    mutable std::vector<std::unique_ptr<Layer>>         m_toAddChilds;
    mutable std::vector<std::size_t>                    m_toRemoveChilds;
};

}

#include "Layer.inl"

#endif /* Layer_hpp */
