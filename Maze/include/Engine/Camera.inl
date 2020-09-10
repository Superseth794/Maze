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

template <class DrawableT, typename _>
inline void Camera::draw(DrawableT* drawingObject, sf::Drawable const& drawable, sf::RenderStates const& states) const {
    if (drawingObject->isHidden())
        return;
    
    m_texture->draw(drawable, states);
}

template <class DrawableT, typename _>
inline void Camera::draw(DrawableT* drawingObject, sf::Drawable const& drawable, sf::Transform const& transform, sf::RenderStates const& states) const {
    if (drawingObject->isHidden())
        return;
    
    sf::RenderStates transformedStates = states;
    transformedStates.transform *= transform;
    m_texture->draw(drawable, transformedStates);
}

template <class DrawableT, typename _>
inline void Camera::draw(DrawableT* drawingObject, sf::Drawable const& drawable, sf::Transformable const& transform, sf::RenderStates const& states) const {
    if (drawingObject->isHidden())
        return;
    
    sf::RenderStates transformedStates = states;
    transformedStates.transform *= transform.getTransform();
    m_texture->draw(drawable, transformedStates);
}

}
