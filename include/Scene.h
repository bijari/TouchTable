//
//  Scene.h
//  TouchTable
//
//  Created by bijaBook2 on 11/6/12.
//
//

#ifndef __TouchTable__Scene__
#define __TouchTable__Scene__
#endif /* defined(__TouchTable__Scene__) */

#include <iostream>

#include "cinder/app/AppBasic.h"
#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"

#include "cinder/Xml.h"

#include "cinder/qtime/QuickTime.h"
#include "cinder/Thread.h"

#include "cinder/Rand.h"

#include "mButtom.h"
#include "mGrid.h"

#pragma once

using namespace ci;
using namespace ci::app;
using namespace std;


class Content{
    
public:
    
    virtual void draw()=0;
    virtual void play()=0;
    virtual void stop()=0;
    
};

class ContentImage : public Content{
private:
    gl::Texture mTex;

    
public:
    ContentImage() { };
    void setImage(Surface &surf){ mTex = gl::Texture(surf); }
    void draw();
    void play(){ };
    void stop(){ };

};

class ContentMovie : public Content{

private:
    qtime::MovieGl mMovie;
    bool movieLoaded;
    
    void loadMovie(fs::path path);
    
public:
    ContentMovie(){ };
    ContentMovie(fs::path p_) { loadMovie(p_); };
    void setMovie(fs::path p_){ loadMovie(p_); }
    void draw();
    void play(){ };
    void stop(){ };
};


typedef boost::shared_ptr<Content> cont_Ptr;

class Scene{
    
public:
    
    Scene(){  }
    Scene( mGrid *grid_){ grid = grid_; load(); }
    
    void load();
    void draw();
    void update();
    
    void mouseMove(Vec2f &pt);
    void mouseClick(Vec2f &pt);
    

    mGrid *grid;
    
    
    // contents;
    cont_Ptr activeContent;
    vector<cont_Ptr> mContents;
    
    vector<mButton> buttonList;
    
    
    
    
};






