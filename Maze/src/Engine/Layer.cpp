//
//  Layer.cpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#include "../../include/Engine/Layer.hpp"

void mz::Layer::display(sf::RenderTexture& targetTexture) {
    for (auto & layer: m_subLayers) {
        layer->display(targetTexture);
    }
}
