//
//  mTriangle.cpp
//  TouchTable
//
//  Created by bijaBook2 on 12/11/12.
//
//

#include "mTriangle.h"



void mTriangle::makeIsoTri(Vec2f &p, float &s, bool &up){
    
    
    pos = p;
    size = s;
    
    
    height = sin(300)*s;
    
    tx1 = Vec2f(0, .35);
    tx2 = Vec2f(.35, .3);
    tx3 = Vec2f(0, 0.015f);

    p1 = Vec2f(-0.5*s +p.x, p.y);
    p2 = Vec2f( 0.5*s +p.x, p.y);
    
    
    if(up){
        p3 = Vec2f( p.x, height + p.y);
        
        
        isUp = true;
    }
    
    else{
        p3 = Vec2f( p.x, -height + p.y);
        
        isUp = false;
    }
    
    center = (p1 + p2 + p3)/3.0f;
    
}


float mTriangle::sign(Vec2f &a, Vec2f &b, Vec2f &c){
    
    return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
}

bool mTriangle::contains(Vec2f &pt){
    
    bool b1, b2, b3;
    
    b1 = sign(pt, p1, p2) < 0.0f;
    b2 = sign(pt, p2, p3) < 0.0f;
    b3 = sign(pt, p3, p1) < 0.0f;
    
    
    return ((b1 == b2) && (b2 == b3));
}

void mTriangle::changeSize(float s){
    
    
    makeIsoTri(pos, s, isUp);
    
}


void mTriangle::draw(){
    
    

        glBegin(GL_TRIANGLE_STRIP);
    
        gl::texCoord(tx1);
        gl::vertex(p1);
    
        gl::texCoord(tx2);
        gl::vertex(p2);
    
        gl::texCoord(tx3);
        gl::vertex(p3);
    
        glEnd();
        
    
}