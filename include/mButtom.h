//
//  mButtom.h
//  TouchTable
//
//  Created by bijaBook2 on 10/26/12.
//
//


#include <iostream>
#include "cinder/app/AppBasic.h"
#include "cinder/Cinder.h"
#include "cinder/Timeline.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#include "mTriangle.h"


#pragma once

using namespace std;
using namespace ci;
using namespace ci::app;



class mButton{
    
public:
    mButton();
    mButton(Vec2f pos_ );
    mButton(Vec2f pos_, Vec2f size_);
    mButton(mTriangle t);
    
    bool onTop(Vec2f &pt);
    void update();
    void draw();
    
    void setPos(Vec2f pt);
    void setPos(Vec2f pt,float t, ci::Timeline &timeline);
    void resetPos(float t, ci::Timeline &timeline);
    
    void setSize(Vec2f pt);
    void setSize(Vec2f pt, ci::Timeline &timeline);
    void setSize(ci::Timeline &timeline);
    void resetSize(ci::Timeline &timeline);
    
    void setName(string &n);
    
    void printInfo();
    
    gl::Texture mTex;
    bool isOnTop;
    Vec2f growSize;
    
    int selection_index;
    
protected:
    
    void set();
    void callbackDie();
    void callbackBorn();

    
    Vec2f initialPos;
    Anim<Vec2f> pos;
    
    Vec2f initialSize;
    Anim<Vec2f> size;
    
    Vec2f textPos;
    
    Area area;
    
    string name;
    bool isDead;
    
    mTriangle mTri;
    
};
