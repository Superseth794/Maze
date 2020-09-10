//
//  Camera.hpp
//  Maze
//
//  Created by Jaraxus on 10/08/2020.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <SFML/Graphics.hpp>

#include "Drawable.hpp"

namespace mz {

class Drawable;

class Camera {
public:
    Camera(unsigned int width, unsigned int heigth);
    Camera(Camera const& camera) = delete;
    Camera(Camera && camera) noexcept = default;
    
    Camera& operator=(Camera const& camera) = delete;
    Camera& operator=(Camera && camera) noexcept = default;
    
    virtual ~Camera() = default;
    
    inline void clear(sf::Color const& color = sf::Color::Transparent);
    
    virtual void display(sf::RenderTarget& texture);
    
    template <class DrawableT, typename _ = typename std::enable_if<std::is_base_of<Drawable, DrawableT>::value>::type>
    inline void draw(DrawableT* drawingObject, sf::Drawable const& drawable, sf::RenderStates const& states = sf::RenderStates::Default) const;
    
    template <class DrawableT, typename _ = typename std::enable_if<std::is_base_of<Drawable, DrawableT>::value>::type>
    inline void draw(DrawableT* drawingObject, sf::Drawable const& drawable, sf::Transform const& transform, sf::RenderStates const& states = sf::RenderStates::Default) const;
    
    template <class DrawableT, typename _ = typename std::enable_if<std::is_base_of<Drawable, DrawableT>::value>::type>
    inline void draw(DrawableT* drawingObject, sf::Drawable const& drawable, sf::Transformable const& transform, sf::RenderStates const& states = sf::RenderStates::Default) const;
    
private:
    unsigned int                                    m_height;
    mutable std::unique_ptr<sf::RenderTexture>      m_texture;
    unsigned int                                    m_width;
};

}

#include "Camera.inl"

#endif /* Camera_hpp */
