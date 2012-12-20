//
//  mButtom.cpp
//  TouchTable
//
//  Created by bijaBook2 on 10/26/12.
//
//

#include "mButtom.h"



mButton::mButton(){
    isDead = false;
    
    pos = Vec2f(500, 600);
    size = Vec2f(30, 40);
    
    mTri  = mTriangle(pos, size().x, true);
}

mButton::mButton(Vec2f pos_){
    
    isDead = false;
    
    pos = pos_;
    initialPos = pos_;

    size = Vec2f(50, 50);
    growSize = Vec2f(70, 70);
    name = "button";
    
    mTri  = mTriangle(pos, size().x, true);
}

mButton::mButton(Vec2f pos_, Vec2f size_){
    
    isDead = false;
    
    pos = Vec2f(pos_.x, pos_.y);
    initialPos = pos_;

    size = size_;
    initialSize = size_;
    growSize = size_ + Vec2f(30, 10);
    
    name = "button";
    
    setPos(initialPos, -0.4f ,timeline());
    setSize(initialSize);
    
    
   mTri  = mTriangle(pos, size().x, true);

}

mButton::mButton(mTriangle t){
    
    mTri = t;
    
    
    isDead = false;
    
    pos = Vec2f(t.pos.x , t.pos.y);
    initialPos = pos;
    
    size = Vec2f(t.size, 0);
    initialSize = size;
    growSize = size() + Vec2f(30, 10);
    
    name = "button";
    
    setPos(initialPos, -0.4f ,timeline());
    setSize(initialSize);
    
}


void mButton::setName(string &n){
    
    name = n;
    
}


void mButton::callbackBorn(){
    
    console() << "Im live!!! ---------" << endl;
}

void mButton::callbackDie(){
    
    console() << "Im deAD ---------" << endl;
}

bool mButton::onTop(Vec2f &pt){
    
    if(mTri.contains(pt)){
        
        console() << "onTop";
        isOnTop = true;
        return true;
        
    }

    else{
//        isOnTop = false;
        return false;
    }
}

// Functions to modify position ----
void mButton::setPos(Vec2f pt){
    
    pos = pt;
}
void mButton::setPos(Vec2f pt,float t, ci::Timeline &timeline){
    
    timeline.apply(&pos, pt, 0.7f, EaseOutAtan(10)).timelineEnd(t);
}
void mButton::resetPos(float t, ci::Timeline &timeline){
    
    timeline.apply(&pos, initialPos, 0.7f, EaseOutAtan(t));
}

// ---- End position functions ---


// Functions to modify size ---- 
void mButton::setSize(Vec2f pt){
    
    size = pt;
}
void mButton::setSize(Vec2f pt, ci::Timeline &timeline){
    
    timeline.apply(&size, pt, 0.1f, EaseOutAtan(3));
}

void mButton::setSize(ci::Timeline &timeline){
    
    timeline.apply(&size, growSize, 0.1f, EaseOutAtan(3));
}

void mButton::resetSize(ci::Timeline &timeline){
    
    timeline.apply(&size, initialSize, 0.1f, EaseOutAtan(3));
}

// ---- End size functions -----


void mButton::printInfo(){
    
    console() << "Size: " << size << endl;
    console() << "Pos: " << pos << endl;
    
}

void mButton::update(){

    mTri.changeSize( size().x);
}


void mButton::draw(){
    
    gl::enableAlphaBlending();

    if(mTex){
        gl::color(1.0f, 1.0f, 1.0f, 0.75f);
        mTex.enableAndBind();
        mTri.draw();
        mTex.unbind();
    }
        

    else{
        

        mTri.draw();
        gl::color(1.0f, 0.0f, 0.0f, 0.5f);
        gl::drawString(name, Vec2f(area.x1, area.y1+10));
        
        
    }
    
}