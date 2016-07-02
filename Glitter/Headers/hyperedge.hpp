//
//  hyperedge.h
//  O5C435
//
//  Created by alexxx on 02/07/2016.
//
//
#define M_PI        3.14159265358979323846264338327950288   /* pi */

#ifndef hyperedge_h
#define hyperedge_h
#pragma once

#endif /* hyperedge_h */

#include <glm/glm.hpp>
//#include <cmath>
#include <cstdlib>


class hyperedge{
    std::vector<GLfloat> vertices_, normals_;
    std::vector<GLuint> triangles_;
    
public:
    
    hyperedge(double, double, int);
    
    const std::vector<GLfloat>& get_vertices() const{
        return vertices_;
    }
    const std::vector<GLuint>& get_indices() const {
        return triangles_;
    }
    const std::vector<GLfloat>& get_normals() const{
        return normals_;
    }
};

hyperedge::hyperedge(double length, double start_R, int frequency){
    double y = 0.0;
    double R = start_R;
    int layer = 0;
    int go = -1;
    while(go--){
        for(int k=0;k<frequency;k++){
            GLfloat angle = (k+(layer%2)/2.0)*2.0*M_PI/frequency;
            vertices_.push_back(R * glm::cos(angle));
            vertices_.push_back(y);
            vertices_.push_back(R * glm::sin(angle));

            normals_.push_back(glm::cos(angle));
            normals_.push_back(0.0f);
            normals_.push_back(glm::sin(angle));
            
            if(layer){
                triangles_.push_back(layer*frequency+k);
                triangles_.push_back((layer-1)*frequency+((k+1)%frequency));
                triangles_.push_back((layer-1)*frequency+k);

                triangles_.push_back(layer*frequency+k);
                triangles_.push_back(layer*frequency+((k+1)%frequency));
                triangles_.push_back((layer-1)*frequency+((k+1)%frequency));
                
            }
        }
        y = y + 2 * R * glm::sin(M_PI/frequency);
        if(go && y > length){
            y = length;
            go = 1;
        }
        R = glm::sqrt(start_R*start_R * (1 - y*y) * (1 - start_R*start_R - y*y) / (1-start_R*start_R));
        layer++;
    }
}
