//
//  Camera.hpp
//  Maze
//
//  Created by Jaraxus on 10/08/2020.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <SFML/Graphics.hpp>

namespace mz {

class Camera {
public:
    Camera(unsigned int width, unsigned int heigth);
    Camera(Camera const& camera) = delete;
    Camera(Camera && camera) noexcept = default;
    
    Camera& operator=(Camera const& camera) = delete;
    Camera& operator=(Camera && camera) noexcept = default;
    
    void clear(sf::Color const& color = sf::Color::Transparent);
    
    void display(sf::RenderTarget& texture);
    
private:
    unsigned int                                    m_height;
    mutable std::unique_ptr<sf::RenderTexture>      m_texture;
    unsigned int                                    m_width;
};

}

#endif /* Camera_hpp */
