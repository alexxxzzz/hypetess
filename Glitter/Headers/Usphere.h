#define M_E         2.71828182845904523536028747135266250   /* e */
#define M_LOG2E     1.44269504088896340735992468100189214   /* log 2e */
#define M_LOG10E    0.434294481903251827651128918916605082  /* log 10e */
#define M_LN2       0.693147180559945309417232121458176568  /* log e2 */
#define M_LN10      2.30258509299404568401799145468436421   /* log e10 */
#define M_PI        3.14159265358979323846264338327950288   /* pi */
#define M_PI_2      1.57079632679489661923132169163975144   /* pi/2 */
#define M_PI_4      0.785398163397448309615660845819875721  /* pi/4 */
#define M_1_PI      0.318309886183790671537767526745028724  /* 1/pi */
#define M_2_PI      0.636619772367581343075535053490057448  /* 2/pi */
#define M_2_SQRTPI  1.12837916709551257389615890312154517   /* 2/sqrt(pi) */
#define M_SQRT2     1.41421356237309504880168872420969808   /* sqrt(2) */
#define M_SQRT1_2   0.707106781186547524400844362104849039  /* 1/sqrt(2) */
//
//  Usphere.h
//  Glitter
//
//  Created by alexxx on 31/03/2016.
//
//

#ifndef Usphere_h
#define Usphere_h
#pragma once

#include <glm/glm.hpp>
//#include <cstdio>
#include <cstdlib>
#include <map>


class unit_sphere{
    std::vector<GLfloat> vertices_, colors_, normals_;
    std::vector<GLuint> triangles_;
    int frequency_;
    public:
    unit_sphere(int frequency);
    
    const std::vector<GLfloat>& get_vertices() const{
        return vertices_;
    }
    const std::vector<GLuint>& get_indices() const {
        return triangles_;
    }
    const std::vector<GLfloat>& get_colors() const{
        return colors_;
    }
    const std::vector<GLfloat>& get_normals() const{
        return normals_;
    }
    void set_color_solid(GLfloat red, GLfloat green, GLfloat blue){
        for(unsigned int k = 0;k<vertices_.size();k+=3){
            colors_.push_back(red);
            colors_.push_back(green);
            colors_.push_back(blue);
        }
    }
};

unit_sphere::unit_sphere(int frequency){
    frequency_ = frequency;
    vertices_.push_back(0.0f);
    vertices_.push_back(1.0f);
    vertices_.push_back(0.0f);
    normals_.push_back(0.0f);
    normals_.push_back(1.0f);
    normals_.push_back(0.0f);

    GLfloat theta = glm::atan(0.5f);
    for(int k = 0;k < 5;k++){
        vertices_.push_back(glm::cos(theta) * glm::cos(M_PI*2*k/5));
        vertices_.push_back(glm::sin(theta));
        vertices_.push_back(glm::cos(theta) * glm::sin(M_PI*2*k/5));
        
        normals_.push_back(glm::cos(theta) * glm::cos(M_PI*2*k/5));
        normals_.push_back(glm::sin(theta));
        normals_.push_back(glm::cos(theta) * glm::sin(M_PI*2*k/5));        
        
        triangles_.push_back(0);
        triangles_.push_back(k+1);
        triangles_.push_back((k+1)%5+1);
    }
    theta = -1.0f * glm::atan(0.5f);
    for(int k = 0;k < 5;k++){
        vertices_.push_back(glm::cos(theta) * glm::cos(M_PI*(2*k+1)/5));
        vertices_.push_back(glm::sin(theta));
        vertices_.push_back(glm::cos(theta) * glm::sin(M_PI*(2*k+1)/5));

        normals_.push_back(glm::cos(theta) * glm::cos(M_PI*(2*k+1)/5));
        normals_.push_back(glm::sin(theta));
        normals_.push_back(glm::cos(theta) * glm::sin(M_PI*(2*k+1)/5));
        
        triangles_.push_back(11);
        triangles_.push_back(k+6);
        triangles_.push_back((k+1)%5+6);

        triangles_.push_back(k+1);
        triangles_.push_back((k+1)%5+1);
        triangles_.push_back(k+6);
        
        triangles_.push_back(k+6);
        triangles_.push_back((k+1)%5+6);
        triangles_.push_back((k+1)%5+1);
    }
    vertices_.push_back(0.0f);
    vertices_.push_back(-1.0f);
    vertices_.push_back(0.0f);

    normals_.push_back(0.0f);
    normals_.push_back(-1.0f);
    normals_.push_back(0.0f);
    
    for(int k=0;k<frequency;k++){
        std::vector<GLuint> old_triangles(std::move(triangles_));
        triangles_.clear();
        std::map<std::pair<int, int>, int> edges;
        for(int j=0; j < old_triangles.size(); j=j+3){
            std::vector<GLuint> vertex_group({old_triangles[j], old_triangles[j+1], old_triangles[j+2]});
            for(int i=0;i < 3;i++){
                auto iter_edge = edges.find({vertex_group[i],vertex_group[(i+1)%3]});
                if(iter_edge == edges.end()){
                    glm::vec3 vertex0(vertices_[3 * vertex_group[i]],vertices_[3 * vertex_group[i]+1],
                                      vertices_[3 * vertex_group[i]+2]);
                    glm::vec3 vertex1(vertices_[3 * vertex_group[(i+1)%3]],vertices_[3 * vertex_group[(i+1)%3]+1],
                                      vertices_[3 * vertex_group[(i+1)%3]+2]);
                    glm::vec3 new_vertex = glm::normalize(vertex0+vertex1);
                    vertices_.insert(vertices_.end(), {new_vertex.x, new_vertex.y, new_vertex.z});
                    normals_.insert(normals_.end(), {new_vertex.x, new_vertex.y, new_vertex.z});
                    vertex_group.push_back(vertices_.size()/3-1);
                    
                }
                else{
                    vertex_group.push_back(iter_edge->second);
                }
            }
            triangles_.push_back(vertex_group[0]);
            triangles_.push_back(vertex_group[3]);
            triangles_.push_back(vertex_group[5]);

            triangles_.push_back(vertex_group[1]);
            triangles_.push_back(vertex_group[4]);
            triangles_.push_back(vertex_group[3]);

            triangles_.push_back(vertex_group[2]);
            triangles_.push_back(vertex_group[5]);
            triangles_.push_back(vertex_group[4]);

            triangles_.push_back(vertex_group[3]);
            triangles_.push_back(vertex_group[4]);
            triangles_.push_back(vertex_group[5]);
        }
    }
}


#endif /* Tentacle_h */
