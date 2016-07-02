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
//  Tentacle.h
//  Glitter
//
//  Created by alexxx on 31/03/2016.
//
//

#ifndef Utentacle_h
#define Utentacle_h
#pragma once

#include <glm/glm.hpp>
//#include <cstdio>
#include <cstdlib>


class unit_tentacle{
    std::vector<GLfloat> vertices_, colors_, normals_;
    std::vector<GLuint> triangles_;
    GLfloat length_, theta_max_, R_min_, R_spiral_, spiral_growth_rate_;
    int frequency_;
    
    public:

    unit_tentacle(GLfloat theta_max,GLfloat length,GLfloat R_min, int frequency, GLfloat fuzz);
    
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
    

    GLfloat get_length() const{
        return length_;
    }
    GLfloat get_theta_max() const{
        return theta_max_;
    }
    GLfloat get_spiral_growth_rate() const{
        return spiral_growth_rate_;
    }
    
    void set_color_solid(GLfloat red, GLfloat green, GLfloat blue){
        colors_.clear();
        for(unsigned int k = 0;k<vertices_.size();k+=3){
            colors_.push_back(red);
            colors_.push_back(green);
            colors_.push_back(blue);
        }
    }
    void bend_tentacle(GLfloat spiral_growth_rate);
    void set_color_gradient(glm::vec3 start_color, glm::vec3 end_color){
        glm::vec3 color_step = (end_color-start_color)/length_;
        colors_.clear();
        for(unsigned int k = 0;k<vertices_.size();k+=3){
            glm::vec3 color = start_color + color_step * vertices_[k+1];
            colors_.push_back(color.r);
            colors_.push_back(color.g);
            colors_.push_back(color.b);
        }
        
    }
};

unit_tentacle::unit_tentacle(GLfloat theta_max,GLfloat length,GLfloat R_min, int frequency, GLfloat fuzz){
    GLfloat b0 = glm::log(length)/(2*M_PI);
    GLfloat b = b0;
    GLfloat err = 1.0f-glm::exp(-b*theta_max)-length*glm::cos(atan(1.0f/b))*2.0f*(1.0f-glm::exp(-2.0f*M_PI*b))/(glm::exp(-2.0f*M_PI*b)+1.0f);
    int k=0;
    while(glm::abs(err) > 0.000001f){
        b = b/(1.0f-b0*glm::atan(err)/(M_PI/2.0f));
        err = 1.0f-glm::exp(-b*theta_max)-length*glm::cos(atan(1.0f/b))*2.0f*(1.0f-glm::exp(-2.0f*M_PI*b))/(glm::exp(-2.0f*M_PI*b)+1.0f);
        k++;
        if(k>1000){
            throw std::runtime_error("No converge, bad, bad!!");
        }
    }
    b = -b;
    spiral_growth_rate_ = b;
    GLfloat R_outer = 1.0f/(1.0f-glm::exp(2*M_PI*b));
    R_spiral_ = R_outer - 0.5;
    
    length_ = length;
    theta_max_ = theta_max;
    R_min_ = R_min;
    
    frequency_ = frequency;
    
    GLfloat y_start = 0.0f;
    GLfloat y_end = length_+y_start;
    GLfloat y = y_start;
    GLfloat y_prev = y_start;
    GLfloat y_next = y_start;
    int M = frequency;
    GLfloat R = 0.5f;
    GLfloat R_prev = R;
    GLfloat R_next = R;
    GLfloat arc_length = (2.0*M_PI*R)/M;
    GLfloat theta_scale = theta_max/y_end;
    
    
    int OddEven = 0;
    while(y_end-y > 0.0*arc_length){
        y_next = y + arc_length*0.866f;
        R_next = 0.5f*glm::exp(b*y_next*theta_scale);
        
        for(int k=0;k<M;k++){
            GLfloat angle = (k*2.0*M_PI)/M+(OddEven*1.0*M_PI)/M;
            vertices_.push_back(R * glm::cos(angle));
            vertices_.push_back(y-y_start+0.0*fuzz*((1.0f*rand())/RAND_MAX-1.0f)/arc_length);
            vertices_.push_back(R * glm::sin(angle));
            
            GLfloat theta = angle;
            GLfloat phi = glm::atan((R_prev-R_next)/(y_next-y_prev));
            
            normals_.push_back(glm::cos(theta)*glm::cos(phi));
            normals_.push_back(glm::sin(phi));
            normals_.push_back(glm::sin(theta)*glm::cos(phi));
            
        }
        OddEven = (OddEven+1);
        if(vertices_.size() > M){
            const int Nvertices = static_cast<int>(vertices_.size()/3);
            for(int k=0;k<M*2;k++){
                if(k<M){
                    triangles_.push_back(Nvertices-M-M+k);
                    triangles_.push_back(Nvertices-M+k%M);
                    triangles_.push_back(Nvertices-M-M+(k+1)%M);
                    
                }
                else{
                    triangles_.push_back(Nvertices-M+k-M);
                    triangles_.push_back(Nvertices-M+(k-M+1)%M);
                    triangles_.push_back(Nvertices-M+(k-M+1)%M-M);
                    
                }
            }
        }
        y_prev = y;
        y = y_next;
        R_prev = R;
        R = R_next;
        arc_length = 2*M_PI*R/M;
    }
    vertices_.push_back(0.0f);
    vertices_.push_back(y-arc_length*0.25f+0.0*fuzz*((1.0f*rand())/RAND_MAX-1.0f)/arc_length);
    vertices_.push_back(0.0f);
    for(int k=0;k<M;k++){
        const int Nvertices = static_cast<int>(vertices_.size()/3);
        triangles_.push_back(Nvertices-1-M+k);
        triangles_.push_back(Nvertices-1);
        triangles_.push_back(Nvertices-1-M+(k+1)%M);
    }
    
    
}



#endif /* Tentacle_h */
