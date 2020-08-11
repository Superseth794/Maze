//
//  Camera.inl
//  Maze
//
//  Created by Jaraxus on 11/08/2020.
//

namespace mz {

inline void Camera::clear(sf::Color const& color) {
    m_texture->clear(color);
}

inline void Camera::draw(sf::Drawable const& drawable, sf::RenderStates const& states) const {
    m_texture->draw(drawable, states);
}

inline void Camera::draw(sf::Drawable const& drawable, sf::Transform const& transform, sf::RenderStates const& states) const {
    sf::RenderStates transformedStates = states;
    transformedStates.transform *= transform;
    m_texture->draw(drawable, transformedStates);
}

inline void Camera::draw(sf::Drawable const& drawable, sf::Transformable const& transform, sf::RenderStates const& states) const {
    sf::RenderStates transformedStates = states;
    transformedStates.transform *= transform.getTransform();
    m_texture->draw(drawable, transformedStates);
}

}
