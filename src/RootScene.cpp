//
//  RootScene.cpp
//  TouchTable
//
//  Created by bijaBook2 on 11/29/12.
//
//

#include "RootScene.h"



bool isCameraAnimationFinished = true;


void Camera_callbackFinish(){
    
    isCameraAnimationFinished = true;
    
}

void Camera_callbackStart(){
    
    isCameraAnimationFinished = false;
    
}


RootScene::RootScene(){
    
    
    
}

RootScene::RootScene(XmlTree &mXml, map<uint32_t, touchPoint> *mTouches_){
    
    
    scenes.resize(50);
    currentScene = 0;
    
    mTouches = mTouches_;
    
    mTriangle tr = mTriangle(Vec2f(100, 200), 50, true);
    grid = mGrid( tr );

    XmlTree arvore = mXml.getChild("svg");
    sortXml(arvore);
    
    mCam = ci::CameraOrtho(0, getWindowWidth(), getWindowHeight(), 0, -100, 100);
    mCam.setCenterOfInterestPoint( -Vec3f::zAxis() );
    centerPointAcc = Vec3f::zero();
    
    //---- Manage Scenes.
    

    for(int i = scenes.size()-1; i >= 0;){
        
       if(scenes[i].pos != Vec3f::zero())
           break;
       else{
           scenes.pop_back();
           i--;
       }
        

        
    }

    console() << "========" << endl;
    for(int i = 0; i <= scenes.size(); i++){
        
        console() << "Scene pos: " << scenes[i].pos << endl;
        
    }
    
    
    uTiles = 1;
    vTiles = 1;
    
    yOffset = 0;
    xOffset = 0;
    
    loadTextures();
     //   console() << "---" << endl <<"Scene pos: " << scenes[5].pos << endl;
}


void RootScene::loadTextures(){
    
    
    gl::Texture::Format fmt;
    fmt.setWrap(GL_REPEAT, GL_REPEAT);
    gridTexture =  gl::Texture( loadImage(loadAsset("grid_sprite.jpg")), fmt );

    
    gridFadeTex =  gl::Texture( loadImage( loadAsset("grid_fade.png")));
    
}

void RootScene::sortXml(XmlTree &xml){
    

    
    // button increment, and scene increment;
    int incc = 0;
    int i = 0;

    for(XmlTree::Iter child = xml.begin(); child != xml.end(); ++child){
        string first_tag = child->getTag();
        console() << "svg tag: " << first_tag << endl;
        
        
        if(first_tag == "g"){
            
            for(XmlTree::Iter groupChild = child->begin(); groupChild != child->end(); ++groupChild){

                
                string tag = groupChild->getTag();
                console() << "g tag: " << tag << endl;
                
                
                if(tag == "circle"){
                    
                    string name = groupChild->getAttributeValue<string>("id");
                    
                    if(name[0] == 'C'){
                        
                        
                        float x = groupChild->getAttributeValue<float>("cx");
                        float y = groupChild->getAttributeValue<float>("cy");
                        
                        console() << "X: " << x << ", Y: " << y << endl;
                        
                        vector<string> order = split(name, '.');
                        i = atoi( order[1].c_str() );
                        
                    
                        scenes[i].pos = Vec3f(x,y,0);
                        
                        
                        console() <<" added a scene " << endl;
                        console() <<" scene " << endl;
                
                        
                    }
                }
                
                
                else if(tag == "rect"){
                    console() << "BT start" << endl;
                    
                    if(groupChild->hasAttribute("id")){
                        vector<string> names = split(groupChild->getAttributeValue<string>("id"), '.');
                        
                        if(names[0] == "bt"){
                            
                            
                            float x = groupChild->getAttributeValue<float>("x");
                            float y = groupChild->getAttributeValue<float>("y");
                            
                            Vec2f p(x, y);
                            
                            
                            
                            float width =  groupChild->getAttributeValue<float>("width");
                            float height = groupChild->getAttributeValue<float>("height");
                            
                            mButton b = mButton( grid.getTriangle( p ));
                            b.setSize(Vec2f(width, height), timeline());
                            b.setName(names[1]);
                            b.update();
                            
                            projetos.push_back(b);
                            scenes[i].indexes.push_back(incc);
                            incc++;
                            
                            console() << "BT " << incc << " added in scene " << i << endl;
        
                            }
                        
                       }
                    }
                
                
                
                
                else{
                    
                    console() << "Could load seccondary tag of name: " << tag << endl;
                }

            }
            
        }
        
        
        
        
        if(first_tag == "line"){
            
            float x1 = child->getAttributeValue<float>("x1");
            float y1 = child->getAttributeValue<float>("y1");
            float x2 = child->getAttributeValue<float>("x2");
            float y2 = child->getAttributeValue<float>("y2");
            
            mLines.push_back(Line(x1, y1, x2, y2 ));
            
        }


        else{
            
            console() << "Could load tag of name: " << first_tag << endl;
        }
        
        
        
    }
    


}


void RootScene::mouseMove(Vec2f pos, Vec3f &acc){
 
    centerPointAcc = acc/5.0f;
    
    mousePos = pos;

    
    for(int i = 0; i < scenes[currentScene].indexes.size(); i++){
        
        int p  = scenes[currentScene].indexes[i];
        mButton *b = &projetos[p];
        
        if(b->onTop( finalPos )){
            
            b->setSize(timeline());
        }
        
        else if(b->isOnTop && !b->onTop( finalPos )){
            
            b->resetSize(timeline());
            b->isOnTop = false;
        }
        
    }

}

void RootScene::mouseMove(Vec2f &pos){

    mousePos = pos;
    
    
    for(int i = 0; i < scenes[currentScene].indexes.size(); i++){
        
        int p  = scenes[currentScene].indexes[i];
        mButton *b = &projetos[p];
        
        if(b->onTop( finalPos )){
            
            b->setSize(timeline());
        }
        
        else if(b->isOnTop && !b->onTop( finalPos )){
            
            b->resetSize(timeline());
            b->isOnTop = false;
        }
        
    }

    
    
}


void RootScene::setBgTexture(gl::Texture &t){
    
    gridTexture = t;
    
}

// Changes the scene to the next one
void RootScene::changeScene(ci::Timeline &t){
    
    
    if(isCameraAnimationFinished){
        
        tempIndexes.clear();
        for(int i = 0; i < scenes[currentScene].indexes.size(); i++){
            
            int p  = scenes[currentScene].indexes[i];
            
            tempIndexes.push_back(p);

        }
        
        currentScene += 1;
        if(currentScene > scenes.size() - 1 ){
            currentScene = 0;
        }
        

        t.appendTo(&centerPoint, scenes[currentScene].pos, 1.0f, EaseInOutAtan(30)).startFn(Camera_callbackStart).finishFn(Camera_callbackFinish);
    }
}

// Changes the scene to a specific one ( i )
void RootScene::changeScene(int i, ci::Timeline &t){
    if(isCameraAnimationFinished){
        
        tempIndexes.clear();
        for(int i = 0; i < scenes[currentScene].indexes.size(); i++){
            
            int p  = scenes[currentScene].indexes[i];
            
            tempIndexes.push_back(p);
            
        }
        
        
        currentScene  = i;
        
        if(currentScene > scenes.size() - 1 ){
            currentScene = scenes.size() - 1;
        }
        
        
        t.appendTo(&centerPoint, scenes[currentScene].pos, 1.3f, EaseInOutAtan(30)).startFn(Camera_callbackStart).finishFn(Camera_callbackFinish);
    }
    
}

void RootScene::checkScene(){
    
    
    // Checks wich scene is closer to the camera center point
    
    if(centerPointAcc != Vec3f::zero() && centerPointAcc.length() < 0.1f ){

        float currentDist = fabs(scenes[currentScene].pos.distance( centerPoint() ) );
        
        
        int changeToIndex = currentScene;
        for(int i = 0; i < scenes.size(); i++){

            float dst1 = abs(scenes[i].pos.distance( centerPoint() )) ;

            if(dst1 < currentDist && currentScene != i){
                
                changeToIndex = i;
            }
            

        }
        
        
        if(changeToIndex != currentScene){
            
            changeScene(changeToIndex, timeline());
        }
        
        else{
            changeScene(currentScene, timeline());
        }
        
        
    
    }
}

void RootScene::update(){
    
    for(int i = 0; i < scenes[currentScene].indexes.size(); i++){
        
        int p  = scenes[currentScene].indexes[i];
        mButton *b = &projetos[p];
        b->update();
    }
    
    if(centerPointAcc.length() > .01){
        centerPoint() += centerPointAcc;
        centerPointAcc *= .95f;
   }
    
    else{
        centerPointAcc = Vec3f::zero();
    }

    finalPos = mousePos + centerPoint().xy();
    
    
    
    checkScene();

}



void RootScene::draw(){
    
    
    
    mCam.setEyePoint(centerPoint);
    gl::setMatrices(mCam);

    
    if(gridTexture){
        
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        gl::color(Color(0.85f, 0.85f, 1));
        

        
        gridTexture.enableAndBind();
        glBegin(GL_QUAD_STRIP);
        

        //1
        gl::texCoord(0, 0);
        gl::vertex(0 + xOffset, -720 + yOffset);
        
        //2
        gl::texCoord(0, vTiles);
        gl::vertex(0+ xOffset,1440 + yOffset);
        
        //4
        gl::texCoord(uTiles, 0);
        gl::vertex(3840+ xOffset, -720 + yOffset);
        
        //3
        gl::texCoord(uTiles, vTiles);
        gl::vertex(3840+ xOffset, 1440 + yOffset);
        

        glEnd();
        gridTexture.unbind();
        
        gl::pushModelView();
        gl::translate(centerPoint);
        gl::draw(gridFadeTex);
        gl::popModelView();
    }



    gl::enableAlphaBlending();
    gl::color(ColorA(1,1,1, .5f ));
    
    

    
    
    // draw lines
    gl::pushModelView();
//    gl::translate(centerPoint());
    glLineWidth(5);
    glEnable(GL_LINE_SMOOTH);
    
    for(vector<Line>::iterator it = mLines.begin(); it != mLines.end(); ++it)
    {
        
        gl::drawLine(it->p1, it->p2);
        
    }

    // current draw buttons
    for(int i = 0; i < scenes[currentScene].indexes.size(); i++){
        
        int p  = scenes[currentScene].indexes[i];
        
        mButton *b = &projetos[p];
        
        b->draw();
        

    }
    gl::popModelView();
    
for( map<uint32_t, touchPoint>::const_iterator activeIt = mTouches->begin(); activeIt != mTouches->end(); ++activeIt )
{
    Vec2f pos = activeIt->second.touchPosition + centerPoint().xy();
    
    mTriangle t = grid.getTriangle( pos );
    t.changeSize(grid.getSize());
    t.draw();
    
}
    
    // draw last scene indexes
    for(int i = 0; i < tempIndexes.size(); i++){
        
        int v = tempIndexes[i];
        projetos[v].draw();
        
    }
    
    gl::drawSolidCircle(finalPos, 10);
    
}



