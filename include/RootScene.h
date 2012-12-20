//
//  RootScene.h
//  TouchTable
//
//  Created by bijaBook2 on 11/29/12.
//
//

#include <iostream>

#include "cinder/app/AppBasic.h"
#include "cinder/Cinder.h"
#include "cinder/Xml.h"
#include "cinder/Timeline.h"
#include "cinder/Camera.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"

#include "mButtom.h"
#include "mGrid.h"


using namespace std;
using namespace ci;
using namespace ci::app;



// Struct to store scenes and the buttons indexes of it
struct middleScene{
  
    middleScene(){};
    
        
    Vec3f pos;
    vector<int> indexes;
    
};


struct Line{
    
    Line();
    Line(float x1, float y1, float x2, float y2){p1 = Vec2f(x1, y1); p2 = Vec2f(x2, y2);   }
    Line(Vec2f p1_, Vec2f p2_){p1 = p1_; p2 = p2_; }
    
    
    Vec2f p1;
    Vec2f p2;
    
};


struct touchPoint{
    
    touchPoint(){}
    touchPoint(const Vec2f &p) : t(0)
    {
        touchPosition = p;
        p_distance = 0;
    }
    
    int t;
    float p_distance;
    Vec2f p_Position;
    Vec2f touchPosition;
    
    void update() {
        
        t = t+1;
    }
    
    
};

class RootScene{
    
public:
    RootScene();
    RootScene(XmlTree &mXml_, map<uint32_t, touchPoint> *mTouches_);

    
    void update();
    void draw();
    void changeScene(ci::Timeline &t);
    void changeScene(int i, ci::Timeline &t);
    
    void mouseMove(Vec2f pos, Vec3f &acc);
    void mouseMove(Vec2f &pos);
    
    void setBgTexture(gl::Texture &t);
    
    int currentScene;
    Vec2f mousePos;
    
    float uTiles;
    float vTiles;
    
    float yOffset;
    float xOffset;
    
protected:
    
    void sortXml(XmlTree &xml);
    void loadTextures();
    
    void checkScene();
    
    vector <mButton> galerias;
    vector <mButton> projetos;

    vector<Line> mLines;
    vector<middleScene> scenes;
    vector<int> tempIndexes;

    
    map<uint32_t, touchPoint> *mTouches;

    gl::Texture gridTexture;
    gl::Texture gridFadeTex;
    
    
    // Camera stuff
    Anim<Vec3f> centerPoint;
    Vec3f centerPointAcc;
    Vec2f finalPos;  //mouse position added with the camera position
    vector<Vec2f*> finalTouchPos;
    float lineDamping;
    
    ci::CameraOrtho mCam;
    
    
    mGrid grid;
    
    
    
};