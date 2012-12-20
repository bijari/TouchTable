//
//  mGrid.cpp
//  TouchTable
//
//  Created by bijaBook2 on 12/12/12.
//
//

#include "mGrid.h"





mGrid::mGrid(){
    
    

    
}


mGrid::mGrid(mTriangle &tri_){
    
    
  mTri = tri_;   
}

float mGrid::getSize(){
    if(mTri.size != 0){
    float s = mTri.size;
    return s;
        
    }
    
    else{
        
        return 0;
    }
}


mTriangle mGrid::getTriangle(Vec2f &pos){

    mTriangle returnTri;
    
    int h = mTri.height;
    
    
    try{
    console() << "mTri.height: " << h << endl <<  "height" << mTri.height <<endl;

    int mY = int(pos.y / h) * h ;
    int half_s = mTri.size/2;
    
    int mX = int(pos.x / half_s) * half_s + half_s;
    
    
    float  debug = ( mY/h ) % 2;
    console() << "Tri debug: " << debug << endl;
    

   if ( (mX %  (int)mTri.size) == 0) {
        if(( mY/ h ) % 2 == -1){

            returnTri = mTriangle(Vec2f(mX + half_s, mY), mTri.size, false);
        }
       
        else{

            returnTri = mTriangle(Vec2f(mX, mY), mTri.size, false);
        }

   }
    else {
        
        if(( mY/h ) % 2 == -1){

            returnTri = mTriangle(Vec2f(mX+half_s, mY-mTri.height), mTri.size, true);
        }
        
        else{

            returnTri = mTriangle(Vec2f(mX, mY-mTri.height), mTri.size, true);
        }
    }
    
    }
    
    catch(Exception &e){
        
        console() << e.what() << endl;
    }
    
    return returnTri;
}