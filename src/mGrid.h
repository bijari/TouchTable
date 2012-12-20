//
//  mGrid.h
//  TouchTable
//
//  Created by bijaBook2 on 12/12/12.
//
//

#include <iostream>
#include "mTriangle.h"
#include "cinder/Cinder.h"


#pragma once



using namespace ci;
using namespace std;

class mGrid{
    
public:
    mGrid();
    mGrid(mTriangle &tri_);
    

    mTriangle mTri;
    mTriangle getTriangle(Vec2f &pos);
    float getSize();
    
    float size;
    
};