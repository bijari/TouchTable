//
//  mTriangle.h
//  TouchTable
//
//  Created by bijaBook2 on 12/11/12.
//
//

#include <iostream>
#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/AppBasic.h"

#pragma once

using namespace ci;
using namespace ci::app;
using namespace std;

class mTriangle{
  
public:
    mTriangle() { console() << "default tri" << endl; }
    mTriangle(Vec2f a, Vec2f b, Vec2f c) : p1(a), p2(b), p3(c){ center = (p1 + p2 + p3)/3.0f; }
    mTriangle(Vec2f pos, float size, bool up){ makeIsoTri(pos, size, up); }
    

    
    void changeSize(float s);
    void draw();
    
    bool contains(Vec2f &pt);
    
    Vec2f p1, p2, p3;    // geometry coords
    Vec2f tx1, tx2, tx3; // texture coords
    Vec2f center, pos;
    float height, size;
    bool isUp;
    
    gl::Texture mTex;
    
private:
    void makeIsoTri(Vec2f &p, float &s, bool &up);
    float sign(Vec2f &a, Vec2f &b, Vec2f &c);
    

};