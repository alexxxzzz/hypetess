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

#ifndef Tentacle_h
#define Tentacle_h
#pragma once

#include <glm/glm.hpp>
//#include <cstdio>
#include <cstdlib>


class Tentacle{
    //Rmean = (Rmax-Rmin)/(log(Rmax)-log(Rmin))
    //length(vertices) = 3*height*Rmean/(Rmin^2)
    //length(curve) = length(vertices)/5
    //length(indices) = 2*length(vertices)
    std::vector<GLfloat> vertices_, colors_, normals_, curve_, radii_;
    std::vector<GLuint> triangles_;
    GLfloat length_, Rmin_, Rmax_;
    public:
    Tentacle(unsigned int N, GLfloat length_in,GLfloat Rmin_in,GLfloat Rmax_in,int Mmax, int Mmin, GLfloat fuzz);
    
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
    
    void set_color_solid(GLfloat red, GLfloat green, GLfloat blue){
        for(unsigned int k = 0;k<vertices_.size();k+=3){
            colors_.push_back(red);
            colors_.push_back(green);
            colors_.push_back(blue);
        }
    }
    //void bend_tentacle(double b)
};

Tentacle::Tentacle(unsigned int N, GLfloat length_in,GLfloat Rmin_in,GLfloat Rmax_in,int Mmax, int Mmin, GLfloat fuzz){
    
    length_ = length_in;
    Rmin_ = Rmin_in;
    Rmax_ = Rmax_in;
    
    GLfloat b = -glm::log(Rmax_/Rmin_)/length_;
    GLfloat y_start = 0.0f;
    GLfloat y_end = length_+y_start;
    GLfloat y = y_start;
    GLfloat y_prev = y_start;
    GLfloat y_next = y_start;
    int M = Mmax;
    int Mprev = M;
    GLfloat arc_length_min = (2.0*M_PI*Rmax_)/Mmax;
    GLfloat R = Rmax_;
    GLfloat R_prev = Rmax_;
    GLfloat R_next = Rmax_;
    GLfloat arc_length = (2.0*M_PI*R)/M;
    
    
    int OddEven = 0;
    while(y_end-y > 0.0*arc_length){
        y_next = y + arc_length*0.866f;
        R_next = glm::exp(b*(y_next-glm::log(Rmax_)/b));
        
        if(N<vertices_.size()+3*M+1){
            throw std::runtime_error("too many vertices generated!! OMG!!! Quit, quit, quit!!!");
        }
        
        for(int k=0;k<M;k++){
            GLfloat angle =(k*2.0*M_PI)/M+(OddEven*1.0*M_PI)/(2*M)+fuzz*((1.0f*rand())/RAND_MAX-1.0f)*(2.0*M_PI)/M;
            GLfloat RR = R+((1.0f*rand())/RAND_MAX-1.0f)*(R*fuzz);
            vertices_.push_back(R * glm::cos(angle));
            vertices_.push_back(y-y_start+0.0*fuzz*((1.0f*rand())/RAND_MAX-1.0f)/arc_length);
            vertices_.push_back(R * glm::sin(angle));
            
            GLfloat theta = (k*2.0*M_PI)/M+(OddEven*1.0*M_PI)/(2*M);
            GLfloat phi = glm::atan((R_prev-R_next)/(y_next-y_prev));
            normals_.push_back(glm::cos(theta)*glm::cos(phi));
            normals_.push_back(glm::sin(phi));
            normals_.push_back(glm::sin(theta)*glm::cos(phi));
            
        }
        OddEven = (OddEven+1);
        if(curve_.size() > 0){
            const int Nvertices = static_cast<int>(vertices_.size()/3);
            for(int k=0;k<M+Mprev;k++){
                if(k<Mprev){
                    triangles_.push_back(Nvertices-M-Mprev+k);
                    triangles_.push_back(Nvertices-M+k%M);
                    triangles_.push_back(Nvertices-M-Mprev+(k+1)%Mprev);
                    
                }
                else{
                    triangles_.push_back(Nvertices-M+k-Mprev);
                    triangles_.push_back(Nvertices-M+(k-Mprev+1)%M);
                    triangles_.push_back(Nvertices-M+(k-Mprev+1)%Mprev-Mprev);
                    
                }
            }
        }
        curve_.push_back(0);
        curve_.push_back(0);
        curve_.push_back(y-y_start);
        radii_.push_back(R);
        Mprev = M;
        y_prev = y;
        y = y_next;
        R_prev = R;
        R = R_next;
        arc_length = 2*M_PI*R/M;
        if(arc_length < arc_length_min&&M>Mmin){
            M--;
            arc_length = (2.0*M_PI*R)/M;
            //y = y + glm::sqrt(arc_length*arc_length*3.0f/4.0f);
        }
    }
    vertices_.push_back(0.0f);
    vertices_.push_back(y-arc_length*0.25f+0.0*fuzz*((1.0f*rand())/RAND_MAX-1.0f)/arc_length);
    vertices_.push_back(0.0f);
    for(int k=0;k<Mprev;k++){
        const int Nvertices = static_cast<int>(vertices_.size()/3);
        triangles_.push_back(Nvertices-1-Mprev+k);
        triangles_.push_back(Nvertices-1);
        triangles_.push_back(Nvertices-1-Mprev+(k+1)%Mprev);
    }
    
    
}

#endif /* Tentacle_h */
