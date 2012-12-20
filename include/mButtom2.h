//
//  mButtom.h
//  TouchTable
//
//  Created by bijaBook2 on 10/26/12.
//
//

#ifndef __TouchTable__mButtom__
#define __TouchTable__mButtom__

#include <iostream>
#include "cinder/app/AppBasic.h"
#include "cinder/Cinder.h"
#include "cinder/Surface.h"
#include "cinder/ImageIo.h"


using namespace std;
using namespace ci;
using namespace ci::app;


class mButton{
    
public:
    
    
    mButton();
    mButton(Vec2f initialPos);
    
    void draw();
    void update();
    void onTop();
    void selected();
    
    bool isPressed;
    
    Vec2f mPos;
    float rotation;
    float rotAcc;
    float size;
    float sizeAcc;
    
    Color mDefaultColor;
    Color mSelectedColor;
    
    Surface mSurface;
    
    
    
};





#endif /* defined(__TouchTable__mButtom__) */
