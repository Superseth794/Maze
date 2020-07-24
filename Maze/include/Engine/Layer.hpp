//
//  Layer.hpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#ifndef Layer_hpp
# define Layer_hpp

# include <vector>

# include <SFML/Graphics.hpp>

namespace mz {

class Layer {
public:
    Layer() noexcept = default;
    Layer(Layer const& layer) noexcept = delete;
    Layer(Layer && layer) noexcept = default;
    
    Layer& operator=(Layer && layer) noexcept = default;
    
    virtual ~Layer() = default;
    
    virtual void display(sf::RenderTexture& targetTexture); // TODO: add relative position to parents
    
private:
    std::vector<Layer*> m_subLayers;
};

}

#endif /* Layer_hpp */
