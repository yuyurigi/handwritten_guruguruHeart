#pragma once

#include "ofMain.h"
#include "ofxSvg.h"
#include "ofxNanoVG.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    ofColor readBackground(ofImage img, float x, float y);
    void setSVG();
    
    ofxSVG svg;
    vector<ofPolyline> outlines;
    vector<glm::vec2> point;
    vector<glm::vec2> myPos;
    vector<float> lw;
    ofImage screenshot, backImage;
    vector<float> rad;
    vector<float> ang;
    int rotNum, vc, vc2, ps, lap;
    float radius, thickMax, thickness, radiusNoise, addNoise;
    ofTrueTypeFont  italic, script;
    
    ofxNanoVG::Canvas canvas;
};
