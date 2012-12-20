#include "cinder/Cinder.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/System.h"
#include "cinder/Rand.h"
#include "cinder/CinderMath.h"
#include "TuioClient.h"
#include "cinder/Utilities.h"

#include "cinder/params/Params.h"

#include "cinder/Thread.h"

//my includes

#include "mButtom.h"
#include "mTriangle.h"

#include "Scene.h"
#include "RootScene.h"



#define TOUCH_IDLE_DISTANCE 15
#define TOUCH_IDLE_TIME 125


using namespace ci;
using namespace ci::app;
using namespace std;

//----  Structs... ----



// ----  END of Structs... ----
class TouchTableApp : public AppBasic {
public:
	
    void prepareSettings(Settings *settings);    
    
    void mouseDown( MouseEvent event );
    void myMouseDown(); // called from the touch event
    void mouseDrag( MouseEvent event );
    void myMouseDrag();
    void mouseMove( MouseEvent event );
    
    void touchesBegan( TouchEvent event );
	void touchesMoved( TouchEvent event );
	void touchesEnded( TouchEvent event );
    
    void setup();
	void update();
	void draw();
    

    // Touch Stuff ----
	tuio::Client mTuio;
    map<uint32_t, touchPoint> mTouches;

    int dragTime;    // amount of dragging;
    float touchTime; // records the start time of the touch event;
    float p_touchDistance; //previous avarage distance from touches
    float p_touchRotation; // previous rotation
    
    float rot_time; // testvar
    float scale_time;

    bool touchScale();    // execute SCALING for selected button, returns true if action is beeing performed
    bool touchRotation(); // same, but for rotation
    bool touchTranslation();
    
    Vec2f mousePos;
    Vec2f p_MousePos;

    
    Vec2f touchCenterPoint; // test only
    // end Touch Stuff ---

    RootScene mainScene;
    
    params::InterfaceGl mParams;
    Vec3f camEyePoint;
    int currentScene;
    
    mGrid grid;
    
    
    //Debug stuff
    float spacing;
    mTriangle tr;
    
    gl::Texture gridTexture;
    
    
    float u_tile;
    float v_tile;
    
    float x_offset;
    float y_offset;
    
    Scene mScene;
};




void TouchTableApp::prepareSettings(Settings *settings){
    
    settings->enableMultiTouch();
    settings->setWindowSize(1280, 720);


}

void TouchTableApp::setup()
{
    
    mTuio.registerTouches( this );
	mTuio.connect();

    XmlTree svgFile;
    try{
        svgFile = XmlTree(loadAsset("Cenas.svg"));
        mainScene = RootScene(svgFile, &mTouches);
    }


    catch(Exception &e){
        console() << "Could not load svg file: " << e.what() << endl;
    }
    
    
    currentScene = 1;
    
    u_tile = 76.75f;
    v_tile = 21.99f;
    
    x_offset = -1;
    y_offset = 31;
    
    mParams = params::InterfaceGl("CameraSettings", Vec2i(255, 200));
    mParams.addParam("Scene Position", &camEyePoint);
    mParams.addParam("Current scene", &currentScene);
    mParams.addParam("u_tile", &u_tile);
    mParams.addParam("v_tile", &v_tile);

    mParams.addParam("x_offset", &x_offset);
    mParams.addParam("y_offset", &y_offset);
    
    rot_time = 0.02f;
    dragTime = 0;
    
    
    gl::clear(Color(0,0,0));

    
}




void TouchTableApp::touchesBegan( TouchEvent event ){
    //console() << "touch Began!"  << endl;
    
    touchTime = app::getElapsedSeconds();
    console() << " Touch Began Size: " << event.getTouches().size() << endl;
    
    
    for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end();++touchIt ) {
        console() << touchIt->getId()<< " <-id /pos -> " << touchIt->getPos() <<  endl;
        
        if(touchIt->getId() == 1 )
            mousePos = touchIt->getPos();
        
        
        mTouches.insert(make_pair( touchIt->getId(), touchPoint(touchIt->getPos())  ));
        
	}
    
    console() << " mTouches size: " << mTouches.size() << endl;
    
    
    //if(mousePos.distance(p_MousePos) > 10)
        p_MousePos = mousePos;
}



void TouchTableApp::touchesMoved( TouchEvent event ){


    mousePos = mTouches.begin()->second.touchPosition; // set the mouse position to the first touch element
    mainScene.mousePos = mousePos;

    
    for(vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); touchIt++){
        
        
        Vec2f touchPos = touchIt->getPos();
        Vec2f lastTouchPos = mTouches[touchIt->getId()].touchPosition;
        mTouches[touchIt->getId()].p_Position = lastTouchPos;
        

        if(touchPos.distance(lastTouchPos) > 100)
        mTouches[touchIt->getId()].touchPosition += 0.02f * (mTouches[touchIt->getId()].touchPosition - touchPos); // perform smoothing

        
        mTouches[touchIt->getId()].touchPosition = touchPos;

    }
    
    float time = app::getElapsedSeconds() - touchTime;
    
    
    if(event.getTouches().size() <= 1 && time > 1.0f ){
        
            mainScene.mouseMove(mousePos);

    }
    
    else if(event.getTouches().size() > 1){
        

//        if(!touchRotation(&b))     // perform scaling, if it returns false, pass to rotation
          //  touchScale();
        
        
        
        Vec2f mAcc2d = mousePos - p_MousePos;
        Vec3f mAcc3d = Vec3f(mAcc2d.x, mAcc2d.y, 0);
        
        console() << "accRootscene ->  " << mAcc3d.length() << endl;
        mainScene.mouseMove(mousePos, mAcc3d);
    
    }

    myMouseDrag();
    dragTime += 1;
    
    console() << "Current Scene: " << currentScene << endl;
    
}


void TouchTableApp::touchesEnded( TouchEvent event ){
    console() << "-----> touch Ended!  <-----"  << endl;
    
    float time = app::getElapsedSeconds() - touchTime; //event time
    
    rot_time = 0.02f;

    for(vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt){
        //console() << "ID: " <<touchIt->getId() << endl;
        mTouches.erase(touchIt->getId());
    
    }
    
    

    if(mTouches[0].touchPosition == Vec2f::zero());
        mTouches.erase(mTouches.begin());
        
    if( time  <  0.5)
        myMouseDown();


    
    
    console() << "pMouse: " << p_MousePos.distance(mousePos) << endl;
    if(p_MousePos.distance(mousePos) > 15){

        console() << "---!!!Scene changing!!! ---" << endl;
        //mainScene.changeScene(timeline());
    }
    
    
    p_MousePos = mousePos;
    dragTime = 0;
    
}


// ------- Touch Transforms :  Scale, Rotate, Move

bool TouchTableApp::touchScale(){
    
    bool returnVar = true;
    float acc_distance; // accumulated distance
    
    
    scale_time = 1.0f;
    
    for(map<uint32_t, touchPoint>::iterator it = mTouches.begin(); it != mTouches.end(); it++){
        
        Vec2f touchPos(it->second.touchPosition);
        
        acc_distance += mousePos.distance(touchPos); // calculates the accumulated distance from the mouse position
        
    }
    
    acc_distance = acc_distance/mTouches.size(); // gets the avarage distance

    
    console() << " Touch Distance: " << abs(acc_distance - p_touchDistance) << endl;
    
    
    if(abs(acc_distance - p_touchDistance) > 5){
        
        if(acc_distance < p_touchDistance){
            // perform scale -
            
            
        }
        
        else{
            
            // perform scale +
        }
        
    }
    
    else{
        returnVar = false;
    }
    
    p_touchDistance = acc_distance;
    
    return returnVar;
}



bool TouchTableApp::touchRotation(){
    
    
    bool returnVar = true;

    
    Vec2f tempVec = Vec2f::zero();

    
    for(map<uint32_t, touchPoint>::iterator it = mTouches.begin(); it != mTouches.end(); it++){
        tempVec += it->second.touchPosition;
    }
    touchCenterPoint = tempVec/mTouches.size();
    
    tempVec = Vec2f(mousePos.x - touchCenterPoint.x , mousePos.y - touchCenterPoint.y);
    tempVec.normalize();
    
    float angle = atan2( tempVec.y, tempVec.x);
    
    
    if( abs(angle - p_touchRotation) > .01f){

        if(angle < p_touchRotation){
                // Perform rotation -
        }

            else{
                //Perform rotation +
            }

    }
    
    else{
        returnVar = false;
    }
    
    p_touchRotation = angle;
    
    return returnVar;
}

bool TouchTableApp::touchTranslation(){
    
    
    bool returnVar = true;
    
    if(mTouches.size() == 1){
        // perform translation
        
//        if(mousePos.x - p_MousePos.x  > 10)
            
        
        
    }
        
    else
        returnVar = false;
    
    return returnVar;
}


// -------  End of Touch transforms


// -------  Custons events:
void TouchTableApp::myMouseDown()
{
    
    
}

void TouchTableApp::myMouseDrag()
{
    

}


// --- End of custom Events


void TouchTableApp::mouseDown( MouseEvent event )
{
    
    //mainScene.changeScene(timeline());
    //console() << "mouseDown!" << endl;
    p_MousePos = mousePos;
    

    
    Vec2f p = event.getPos();
    mScene.mouseClick(p);
    
}

void TouchTableApp::mouseDrag( MouseEvent event )
{
    
    
}

void TouchTableApp::mouseMove( MouseEvent event )
{
    Vec2f p = event.getPos();
    mScene.mouseMove(p);
    
}

void TouchTableApp::update()
{
    
    //loop the touchPoints to see if anything is idle
    for( map<uint32_t, touchPoint>::const_iterator activeIt = mTouches.begin(); activeIt != mTouches.end(); ++activeIt ) {
    
        // Calculates position previus distance
        Vec2f tempPos = activeIt->second.touchPosition;
        Vec2f p_tempPos = activeIt->second.p_Position;
        float distance = p_tempPos.distance(tempPos);

        if( distance < TOUCH_IDLE_DISTANCE || distance == activeIt->second.p_distance )
                mTouches[activeIt->first].update();
        
        else if(activeIt->second.t > 0 ){
                mTouches[activeIt->first].t = 0;
        }
        
        
        
        // if idle time is big, erase.
        if(activeIt->second.t > TOUCH_IDLE_TIME){
            
            mTouches.erase(activeIt->first);
            break;
        }
     
        
       mTouches[activeIt->first].p_distance = distance;
    }
    
    mainScene.uTiles = u_tile;
    mainScene.vTiles = v_tile;
    
    mainScene.yOffset = y_offset;
    mainScene.xOffset = x_offset;
    
    mainScene.update();
    mScene.update();

}

void TouchTableApp::draw()
{
	// clear out the window with black

    
    
    
    gl::clear( Color( 0, 0, 0 ) );
    gl::color(Color::white());
    gl::enableAlphaBlending();
    
    
    
    //gl::setMatricesWindow(getWindowWidth(), getWindowHeight());


    
    

    
    
    if(p_MousePos.distance(mousePos) > 30)
        gl::drawLine(p_MousePos, mousePos);

    p_MousePos = mousePos;

    
    // ---
    gl::color( Color::white());
    
    mainScene.draw();
    mParams.draw();
    
    for( map<uint32_t, touchPoint>::const_iterator activeIt = mTouches.begin(); activeIt != mTouches.end(); ++activeIt ) {

        
    }
    

    
    
    //-- Debug---
    gl::setMatricesWindow(getWindowWidth(), getWindowHeight());
    gl::drawString(toString(getAverageFps()), Vec2f::zero());
    


}

CINDER_APP_BASIC( TouchTableApp, RendererGl )
