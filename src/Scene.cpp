//
//  Scene.cpp
//  TouchTable
//
//  Created by bijaBook2 on 11/6/12.
//
//

#include "Scene.h"


void ContentMovie::loadMovie(fs::path path){
    ci::ThreadSetup mThreadSetup;
    
    
    if(path.extension().string() == ".mov"){
        //mMovie = qtime::MovieGl(path);
        //mMovie.setLoop();
        //mMovie.play();
        
    }
    
    movieLoaded = true;
}


void ContentMovie::draw(){
    
//    gl::drawSolidCircle(Vec2f(500, 500), 50);
    console() << "Movie beeing draw! " << endl;
    
    if(movieLoaded){
        if( mMovie ){
        
            gl::draw(mMovie.getTexture());
        }
        
        
    }
    

    
}

void ContentImage::draw(){
    
//    gl::drawSolidRect(Rectf(250, 250, 500, 500));
    
    console() << "Image beeing draw! " << endl;

    
    
}


/// -----  END CONTENTS CLASS -----

void Scene::load(){
    
    string file = "Bikini";


    fs::path movFile;
    fs::path dir = app::getAssetPath("Projetos/" + file );
    fs::directory_iterator dir_iterator;

    float x = 0;
    float y = 100;
    int scl_index = 0;
    
    
    for (fs::directory_iterator it(dir); it!= dir_iterator; ++it) {
        
        console() << it->path() << endl;
        console() << "I-->>:::" << scl_index << endl;
        
        
        
        if(x > 200){
            x = 50;
            y += 55 + Rand::randInt(0, 30);
        }
        


        if(it->path().extension() == ".mov"){
            
            Vec2f pos = Vec2f(x, y);
            
            mTriangle t = grid->getTriangle(pos);
            t.changeSize(grid->getSize() *.9f);
            mButton b = mButton(t);
            b.selection_index = scl_index;
            b.update();
            buttonList.push_back(b);
            
            movFile = it->path();
            
            ContentMovie *m = new ContentMovie(movFile);
            
            mContents.push_back(cont_Ptr( m ));
            ++scl_index;
            
        }
        
        else if(it->path().extension() == ".jpg"){
            
            Vec2f pos = Vec2f(x, y);

            mTriangle t = grid->getTriangle(pos);
            t.changeSize(grid->getSize() *.9f);
            mButton b = mButton(t);
            
            
            b.selection_index = scl_index;
            b.mTex = gl::Texture(loadImage(it->path()));
            b.update();
            buttonList.push_back(b);
            
            mContents.push_back(cont_Ptr(new ContentImage() ));
            ++scl_index;
            
        }
        
        x += Rand::randInt(25, 70);
    }
    
    activeContent = mContents[0];
    
}


void Scene::mouseClick(Vec2f &pt){
    

    int index  = 0;
    
    for(vector<mButton>::iterator it = buttonList.begin(); it != buttonList.end(); it++){
        
        if(it->onTop( pt )){
            index = it->selection_index;
           console() << "mouse Click!!!!: " << index << endl;
            activeContent = mContents[index];
            console() << " index: " << index << endl;
        }
    }
    



    
}


void Scene::mouseMove(Vec2f &pt){
    
    for(vector<mButton>::iterator it = buttonList.begin(); it != buttonList.end(); it++){
        
        if(it->onTop( pt )){
            
            it->setSize(timeline());
            
        }
        
        else if(it->isOnTop && ! it->onTop( pt )){
            
            it->resetSize(timeline());
            it->isOnTop = false;
        }
        
    }
    
}

void Scene::update(){
    
    for(vector<mButton>::iterator it = buttonList.begin(); it != buttonList.end(); it++){
    
        it->update();
        
    }
    
    console() << " ------------ " << endl;
    
    
 
}


void Scene::draw(){
    
    
    for(vector<mButton>::iterator it = buttonList.begin(); it != buttonList.end(); it++){
        
        it->draw();
        
    }
    
       activeContent->draw();
    
}