#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);
    ofSetFrameRate(100);
    
    thickness = 1.0; //線の太さの最小値
    thickMax = 9.0; //線の太さの最大値
    
    backImage.load("back.png");
    
    setSVG();
    
    //ハートを何周描くか
    rotNum = point.size() * 34 + point.size()/2; //34周半
    vc = 0;
    vc2 = 0;
    radius = 0.01; //１周目のハートの半径（SVGの幅、高さに掛ける数値）
    ps = 50; //１周目のハートの取得する配列の間隔（５０個とばしで配列を取得する）
    lap = 0; //いま何周目か
    radiusNoise = ofRandom(10);
    addNoise = 1;
    
    //ofxNanoVG
    canvas.allocate(ofGetWidth(), ofGetHeight());
    
    //font
    italic.load("Mellow_Morning_Small_Caps_Italic.ttf", 40, true, true, true);
    script.load("Mellow_Morning_Script_Alt.ttf", 80, true, true ,true);
     
}
//--------------------------------------------------------------

void ofApp::setSVG(){
    svg.load("heart.svg");
    for (ofPath p: svg.getPaths()){
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        const vector<ofPolyline>& lines = p.getOutline();
        for(const ofPolyline & line: lines){
            outlines.push_back(line.getResampledBySpacing(1));
        }
    }
    
    //svgの頂点を配列に代入
    for (ofPolyline & line: outlines){
        for (int j = 0; j < line.size(); j++){
            point.push_back(line[j]);
        }
    }
    
    //配列に入れた頂点のradius,angleを計算
    glm::vec2 center= glm::vec2(svg.getWidth()/2, svg.getHeight()/2);
    for (int i = 0; i < point.size(); i++) {
        float a = atan2(point[i].y - center.y, point[i].x - center.x);
        ang.push_back(a);
        float r = (point[i].y - center.y) / sin(ang[i]);
        rad.push_back(r);
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofxNanoVG::Canvas& ca = canvas;
    ca.background(0.878, 0.878, 0.878); //FloatColor
    //ca.background(1, 0.965, 0.929); //FloatColor
    ca.begin();
    ca.strokeColor(ofColor::fromHex(0x0185be));
    //ca.strokeColor(ofColor::fromHex(0xc3d6e7));
    
    if(svg.getWidth()/2*radius < 500){
        vc = vc%point.size();
        float thisRadius = (rad[vc]*radius) + (ofMap(ofNoise(radiusNoise), 0, 1, -8, 8)) ;
        float hx = ofGetWidth()/2 + (thisRadius * cos(ang[vc]));
        float hy = ofGetHeight()/2 + (thisRadius * sin(ang[vc]));
        
        //画像の色を取得
        ofColor c = readBackground(backImage, hx, hy);
        //暗い色を太い線に、明るい色を細い線にする
        float brightness = c.getBrightness();
        float b = ofMap(c.getBrightness(), 0, 255, thickMax, thickness);
        lw.push_back(b); //線の太さを配列に代入
        glm::vec2 pos(hx, hy);
        myPos.push_back(pos); //点の位置を配列に代入
        
        int f = floor(vc2/point.size()); //いま何周目にいるか計算する
        float addRadius = 0.00003;
        //1周するごとに取得する配列の感覚を小さくする（*0.95）
        if(lap != f){
            ps = ofClamp(ps*0.95, 10, ps);
            addNoise -= 0.00001;
            addRadius = ofRandom(0.00001, 0.0001);
        }
         vc += 1*ps;
         vc2 += 1*ps;
         radius += addRadius*ps;
         lap = f;
         radiusNoise += (0.1*addNoise);
    }
    
    ca.lineCap(ofxNanoVG::LineCap::ROUND);
    ca.lineJoin(ofxNanoVG::LineCap::ROUND);
    
    //線を描く
    for(int i = 1; i < myPos.size(); i++){
        ca.beginPath();
        ca.lineWidth(lw[i]);
        ca.moveTo(myPos[i-1].x, myPos[i-1].y);
        ca.lineTo(myPos[i-1].x, myPos[i-1].y);
        ca.lineTo(myPos[i].x, myPos[i].y);
        ca.strokePath();
        
    }
    ca.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    //ハート
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(255, 255, 255);
    canvas.draw(0, 0);
    
    //文字
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ofSetColor(ofColor::fromHex(0x4e5f67));
    //ofSetColor(ofColor::fromHex(0xff8e4c));
    ofTranslate(322, 583);
    ofRotateDeg(-10);
    italic.setLetterSpacing(0.9);
    italic.drawStringAsShapes("Encouragement of", 0, 0);
    ofRectangle rect = italic.getStringBoundingBox("Encouragement of", 0,0);
    ofRectangle rect2 = script.getStringBoundingBox("Climb", 0, 0);
    script.drawStringAsShapes("Climb", rect.width/2-rect2.width/2, rect.height*2.2);

}

//--------------------------------------------------------------
ofColor ofApp::readBackground(ofImage img, float x, float y){
    ofPixels colorPixels = img.getPixels();
    ofColor c = colorPixels.getColor(x, y);
    return c;
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'S' || key == 's') {
        screenshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        screenshot.save(ofGetTimestampString("%Y%m%d%H%M%S")+"##.jpg");
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
