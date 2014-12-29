#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
		ofSetFullscreen(true);
    ofEnableSmoothing();
    ofBackground(ofColor::black);
    //    ofSetBackgroundAuto(false);
    
    //image1とimage2は、同じサイズであることを前提とする
    image1.loadImage("monalisa_debug.jpg");
    image2.loadImage("monalisa_debug.jpg");
    originalImage1 = image1;
    originalImage2 = image2;
    
    drawImageSwitch = false;
    counter = -1;
    for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
        exchangeOrderVector.push_back(i);
    }
		usedOrNotVector.assign((int)(image1.getWidth()*image1.getHeight()), false);
		
		// 重複がないピクセル数分のインデックスの乱数をつくる
    random_shuffle(exchangeOrderVector.begin(), exchangeOrderVector.end());
    enableDuplicate = false;
    enableExchange = true;
    enableRandomExchange = true;
    enableMultipixelDrawing = true;
    
//    ofSetFullscreen(true);
    
    saveToFile = false;
		dataReaded = false;
		animationToOriginal = false;
		animationToExchange = false;

		
		currentPointVector.assign((int)(image1.getWidth()*image1.getHeight()), 0);// image1のピクセル数分を0で初期化
		currentRectRightPointVector.assign((int)(image1.getWidth()*image1.getHeight()), 0);
		currentRectLeftPointVector.assign((int)(image1.getWidth()*image1.getHeight()), 0);
		// 左右が入れ替わるアニメーションのため
		marginBetweenImages = 160;
		currentLeftImageOrigin = ofPoint(ofGetWidth()/2.0-image1.getWidth()-marginBetweenImages/2.0, (ofGetHeight()-image1.getHeight())/2.0);
		
		
		enableInteractive = true;
		enableFixFrameExchange = true;

		if (enableInteractive) {
				//カメラ
				//				ofSetLogLevel(OF_LOG_VERBOSE);
				//				vidGrabber.listDevices();
				// ADJUSTME: カメラを確認して、使いたいカメラのIDを入力すること
				//		videoGrabber.setDeviceID(1); // logicool 920を挿した時は、 0 - FaceTime HD Camera (Built-in) / 1 - HD Pro Webcam C920
				videoGrabber.initGrabber(image2.getWidth(), image2.getHeight());
		}

		float pixelHeight = ofGetHeight()/image1.getHeight();
		pixelSize.set(pixelHeight, pixelHeight);
		
		//自動化
		actionTime = 0;
		didAnimationToExchange = false;
		didAnimationToOrigin = false;
		didTakePicture = false;
		didReadData = false;
		
		calucurateFileName = "calculatedData/left-otaniOniji_right-monalisa_320x480.dat";
}

//--------------------------------------------------------------
void testApp::update(){
    if (!saveToFile) {
        if (counter <= image1.getWidth()*image1.getHeight()) {
            counter++;
        }
    }
		
		//カメラ
		if(enableInteractive){
				videoGrabber.update();
				if (videoGrabber.isFrameNew()) {
				}
				float timeDifference = ofGetElapsedTimef()-actionTime;
				if (!didTakePicture && timeDifference > 10) {
						ofLogNotice("a-"+ofToString(didTakePicture)+ofToString(didReadData)+ofToString(didAnimationToExchange)+ofToString(didAnimationToOrigin));
						// t
						image1.clear();
						usedOrNotVector.assign((int)(image1.getWidth()*image1.getHeight()), false);
						image1.setFromPixels(videoGrabber.getPixelsRef());
						image1.mirror(false, true);
						calucurateAndWrite();
						didTakePicture = true;
				}else if (!didReadData && timeDifference > 20){
						ofLogNotice("b-"+ofToString(didTakePicture)+ofToString(didReadData)+ofToString(didAnimationToExchange)+ofToString(didAnimationToOrigin));
						readPointPairVector.clear();
						importFileForStruct(&readPointPairVector, calucurateFileName);
						didReadData = true;
				}else if(!didAnimationToExchange && timeDifference > 25){
						ofLogNotice("c-"+ofToString(didTakePicture)+ofToString(didReadData)+ofToString(didAnimationToExchange)+ofToString(didAnimationToOrigin));
						animationToExchange = animationToExchange ? false : true;
						animationToOriginal = animationToExchange ? false : true;
						setupCurrentPointVector(readPointPairVector);
						didAnimationToExchange = true;
				}else if(!didAnimationToOrigin && timeDifference > 50){
						ofLogNotice("d-"+ofToString(didTakePicture)+ofToString(didReadData)+ofToString(didAnimationToExchange)+ofToString(didAnimationToOrigin));
						animationToExchange = animationToExchange ? false : true;
						animationToOriginal = animationToExchange ? false : true;
						setupCurrentPointVector(readPointPairVector);
						didAnimationToOrigin = true;
				}else if (timeDifference > 55){
						ofLogNotice("e-"+ofToString(didTakePicture)+ofToString(didReadData)+ofToString(didAnimationToExchange)+ofToString(didAnimationToOrigin));
						didTakePicture = false;
						didReadData = false;
						didAnimationToOrigin = false;
						didAnimationToExchange = false;
						actionTime = ofGetElapsedTimef();
				}
		}
}

//--------------------------------------------------------------
void testApp::draw(){
		ofPushMatrix();
		if (enableFixFrameExchange) {
				ofTranslate((ofGetWidth()-image1.getWidth()*pixelSize.x)/2.0, (ofGetHeight()-image1.getHeight()*pixelSize.y)/2.0);
		}else{
				ofTranslate((ofGetWidth()-marginBetweenImages)/2.0-image1.getWidth(), (ofGetHeight()-image1.getHeight())/2.0);
		}
		
				//    ofLog(OF_LOG_NOTICE, "draw"+ofToString(ofGetFrameNum()));
				//    if (!saveToFile) {
				//        if (counter <= image1.getWidth()*image1.getHeight()) {
				//            // 左の画像を右の画像に入れ替える
				//            ofPoint referencePoint;
				//            if (enableRandomExchange) {
				//                //ランダムで交換
				//                referencePoint = ofPoint(exchangeOrderVector[counter]%(int)image1.getWidth(), exchangeOrderVector[counter]/(int)image1.getWidth());
				//
				//            }else{
				//                //上から順に交換
				//                referencePoint = ofPoint(counter%(int)image1.getWidth(), counter/(int)image1.getWidth());
				//            }
				//            ofColor referenceColor = image1.getColor(referencePoint.x, referencePoint.y);
				//            double minimumDistance = image1.getWidth()+image2.getHeight();
				//            ofPoint minimumDistancePoint;
				//            unsigned char * checkImagePixels = image2.getPixels();
				//            // 最小の距離の色を調べる
				//            for (int i=0; i<image1.getHeight(); i++) {
				//                for (int j=0; j<image1.getWidth(); j++) {
				//                    if (!enableDuplicate) {
				//                        // 重複を許さない
				//                        if (!usedOrNotVector[i*(int)image2.getWidth()+j]) {
				//                            ofColor checkColor = ofColor(checkImagePixels[(i*(int)image2.getWidth()+j)*3],
				//                                                         checkImagePixels[(i*(int)image2.getWidth()+j)*3+1],
				//                                                         checkImagePixels[(i*(int)image2.getWidth()+j)*3+2]);
				//                            double checkDistance = getColorDistance(referenceColor, checkColor);
				//                            if (checkDistance < minimumDistance) {
				//                                if (!usedOrNotVector[i*(int)image2.getWidth()+j]) {
				//                                    minimumDistance = checkDistance;
				//                                    minimumDistancePoint = ofPoint(j, i);
				//                                }
				//                                // 同じ色の重複を許す
				//                                minimumDistance = checkDistance;
				//                                minimumDistancePoint = ofPoint(j, i);
				//                            }
				//                        }
				//                    } else{
				//                        ofColor checkColor = ofColor(checkImagePixels[(i*(int)image2.getWidth()+j)*3],
				//                                                     checkImagePixels[(i*(int)image2.getWidth()+j)*3+1],
				//                                                     checkImagePixels[(i*(int)image2.getWidth()+j)*3+2]);
				//                        double checkDistance = getColorDistance(referenceColor, checkColor);
				//                        if (checkDistance < minimumDistance) {
				//                            if (!usedOrNotVector[i*(int)image2.getWidth()+j]) {
				//                                minimumDistance = checkDistance;
				//                                minimumDistancePoint = ofPoint(j, i);
				//                            }
				//                            // 同じ色の重複を許す
				//                            minimumDistance = checkDistance;
				//                            minimumDistancePoint = ofPoint(j, i);
				//                        }
				//                    }
				//                }
				//            }
				//            //最も近い距離の色の組み合わせをベクターに保存
				//            pointPair tempPointPair;
				//            tempPointPair.point1 = referencePoint;
				//            tempPointPair.point2 = minimumDistancePoint;
				//
				//            // 重複を許さない
				//            if (!enableDuplicate) {
				//                //再利用しないために、使用したPointを保存
				//                usedOrNotVector[minimumDistancePoint.y*(int)image2.getWidth()+minimumDistancePoint.x] = true;
				//            }
				//            pointPairVector.push_back(tempPointPair);
				//            //    ofLog(OF_LOG_NOTICE, "X:"+ofToString(pointPairVector.back().point2.x)+" Y:"+ofToString(pointPairVector.back().point2.y));
				//
				//            // 左の画像の色を入れ替える
				//            image1.setColor(pointPairVector.back().point1.x, pointPairVector.back().point1.y, image2.getColor(pointPairVector.back().point2.x, pointPairVector.back().point2.y));
				//            image1.update();
				//
				//            // 右の画像の色を入れ替える
				//            if (enableExchange) {
				//                image2.setColor(pointPairVector.back().point2.x, pointPairVector.back().point2.y, originalImage1.getColor(pointPairVector.back().point1.x, pointPairVector.back().point1.y));
				//                image2.update();
				//            }
				//        }
				//
				//        // draw image
				//        image1.draw(ofPoint(ofGetWidth()/4.-image1.getWidth()/2.+100, (ofGetHeight()-image1.getHeight())/2.), image1.getWidth(), image1.getHeight());
				//        image2.draw(ofPoint(ofGetWidth()/4.*3-image2.getWidth()/2.-100 , (ofGetHeight()-image2.getHeight())/2.), image2.getWidth(), image2.getHeight());
				//
				//        //hightLihgting
				//        if (counter <= image1.getWidth()*image1.getHeight()) {
				//            ofPushStyle();
				//            ofSetColor(ofColor::white);
				//            ofSetLineWidth(1);
				//            ofLine(pointPairVector.back().point1.x+(ofGetWidth()/4.-image1.getWidth()/2.+100), pointPairVector.back().point1.y+((ofGetHeight()-image1.getHeight())/2.),
				//                   pointPairVector.back().point2.x+(ofGetWidth()/4.*3-image2.getWidth()/2.-100), pointPairVector.back().point2.y+((ofGetHeight()-image2.getHeight())/2.));
				//            ofPopStyle();
				//        }
				//
				//    }
				if (animationToOriginal) {
						// debug - アニメーションせずに入れ替える。(動いてる模様)
						//				for (int i=0; i<image1.getHeight(); i++) {
						//						for (int j=0; j<image1.getWidth(); j++) {
						//								image1.setColor(readPointPairVector[i*(int)image2.getWidth()+j].point1.x, readPointPairVector[i*(int)image2.getWidth()+j].point1.y,
						//																image2.getColor(readPointPairVector[i*(int)image2.getWidth()+j].point2.x, readPointPairVector[i*(int)image2.getWidth()+j].point2.y));
						//						}
						//				}
						
						// x軸,y軸それぞれ1個ずつ動くやりかた。(動いてる模様)
						//				// リセット
						//				image1.setColor(ofColor::black);
						//				for (int i=0; i<image1.getHeight(); i++) {
						//						for (int j=0; j<image1.getWidth(); j++) {
						//								//x軸
						//								if (readPointPairVector[i*(int)image2.getWidth()+j].point2.x-currentPointVector[i*(int)image2.getWidth()+j].x > 0) {
						//										currentPointVector[i*(int)image2.getWidth()+j].x++;
						//								}else if(readPointPairVector[i*(int)image2.getWidth()+j].point2.x-currentPointVector[i*(int)image2.getWidth()+j].x < 0){
						//										currentPointVector[i*(int)image2.getWidth()+j].x--;
						//								}
						//								//y軸
						//								if (readPointPairVector[i*(int)image2.getWidth()+j].point2.y-currentPointVector[i*(int)image2.getWidth()+j].y > 0) {
						//										currentPointVector[i*(int)image2.getWidth()+j].y++;
						//								}else if(readPointPairVector[i*(int)image2.getWidth()+j].point2.y-currentPointVector[i*(int)image2.getWidth()+j].y < 0){
						//										currentPointVector[i*(int)image2.getWidth()+j].y--;
						//								}
						//
						//								// 色をぬる
						//								image1.setColor(currentPointVector[i*(int)image2.getWidth()+j].x, currentPointVector[i*(int)image2.getWidth()+j].y,
						//																image2.getColor(readPointPairVector[i*(int)image2.getWidth()+j].point2.x, readPointPairVector[i*(int)image2.getWidth()+j].point2.y));
						//
						//						}
						//				}
						
						
						// 左右もしくは上下にしか動かないやりかた(画像データを書き換えるやり方)
						//				image1.setColor(ofColor::black);
						//				for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
						//								if (abs(readPointPairVector[i].point2.x-readPointPairVector[i].point1.x) <
						//										abs(readPointPairVector[i].point2.y-readPointPairVector[i].point1.y)) {
						//										// x方向が先に移動
						//										if(readPointPairVector[i].point2.x-currentPointVector[i].x < 0){
						//												//1ピクセル負の方向に移動
						//												currentPointVector[i].x = currentPointVector[i].x-1;
						//										}
						//										else if (readPointPairVector[i].point2.x-currentPointVector[i].x > 0){
						//												//1ピクセル正の方向に移動
						//												currentPointVector[i].x = currentPointVector[i].x+1;
						//										}else{
						//												//y方向に移動
						//												if (readPointPairVector[i].point2.y-currentPointVector[i].y < 0) {
						//														//1ピクセル負の方向に移動
						//														currentPointVector[i].y = currentPointVector[i].y-1;
						//												}else if (readPointPairVector[i].point2.y-currentPointVector[i].y > 0){
						//														//1ピクセル正の方向に移動
						//														currentPointVector[i].y = currentPointVector[i].y+1;
						//												}else{
						//														//ゴールに到着しているので、なにもしない
						//												}
						//										}
						//								}else{
						//										// y方向が先に移動
						//										if (readPointPairVector[i].point2.y-currentPointVector[i].y < 0) {
						//												//1ピクセル負の方向に移動
						//												currentPointVector[i].y = currentPointVector[i].y-1;
						//										}else if (readPointPairVector[i].point2.y-currentPointVector[i].y > 0){
						//												//1ピクセル正の方向に移動
						//												currentPointVector[i].y = currentPointVector[i].y+1;
						//										}else{
						//												//x方向に移動
						//												if(readPointPairVector[i].point2.x-currentPointVector[i].x < 0){
						//														//1ピクセル負の方向に移動
						//														currentPointVector[i].x = currentPointVector[i].x-1;
						//												}
						//												else if (readPointPairVector[i].point2.x-currentPointVector[i].x > 0){
						//														//1ピクセル正の方向に移動
						//														currentPointVector[i].x = currentPointVector[i].x+1;
						//												}else{
						//														//ゴールに到着しているので何もしない
						//												}
						//										}
						//								}
						//								//ずらしたpoint1の位置に、point2の色を設定
						//								image1.setColor(currentPointVector[i].x, currentPointVector[i].y,
						//																image2.getColor(readPointPairVector[i].point2.x, readPointPairVector[i].point2.y));
						//				}
						//				image1.update();
						
						//				//絵全体を左から右に移動する
						//				// 目標地点が: ofGetWidth()/2.0+marginBetweenImages/2.0
						//				if (currentLeftImageOrigin.x < ofGetWidth()/2.0+marginBetweenImages/2.0) {
						//						currentLeftImageOrigin.x = currentLeftImageOrigin.x+1;
						//				}
						//				image1.draw(currentLeftImageOrigin.x, currentLeftImageOrigin.y, image1.getWidth(), image2.getHeight());
						
						// 1ピクセルの四角形での描画
						// x軸,y軸それぞれ1個ずつ動くやりかた
						// 左から右(交換版からオリジナルへの移行)
						//				ofPushMatrix();
						//				ofTranslate((ofGetWidth()-marginBetweenImages)/2.0-image1.getWidth(), (ofGetHeight()-image1.getHeight())/2.0);

						for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
								// ofRect(0, 0, 30, 30);
								//モナリザのとき
								// 到達点
								//FIXME: ここ？に条件を書く
								if(didTakePicture && didReadData && !didAnimationToExchange &&!didAnimationToOrigin){
										// オリジナルを描くだけ
										ofPoint goalPoint(readPointPairVector[i].point2.x, readPointPairVector[i].point2.y);
										ofPushStyle();
										ofSetColor(image2.getColor(readPointPairVector[i].point2.x, readPointPairVector[i].point2.y));
										// 色を塗る
										ofRect(goalPoint.x*pixelSize.x, goalPoint.y*pixelSize.x, pixelSize.x, pixelSize.y);
										//ofRect(goalPoint.x, goalPoint.y, pixelSize.x, pixelSize.y); // n3
										ofPopStyle();
								}else{
										ofPoint goalPoint;
										if (enableFixFrameExchange) {
												goalPoint = ofPoint(readPointPairVector[i].point2.x, readPointPairVector[i].point2.y);
										}else{
												goalPoint = ofPoint(readPointPairVector[i].point2.x+image1.getWidth()+marginBetweenImages, readPointPairVector[i].point2.y);
										}
										// x軸
										if (goalPoint.x-currentRectRightPointVector[i].x > 0) {
												currentRectRightPointVector[i].x++;
										}else if(goalPoint.x-currentRectRightPointVector[i].x < 0){
												currentRectRightPointVector[i].x--;
										}
										// y軸
										if (goalPoint.y-currentRectRightPointVector[i].y > 0) {
												currentRectRightPointVector[i].y++;
										}else if(goalPoint.y-currentRectRightPointVector[i].y < 0){
												currentRectRightPointVector[i].y--;
										}
										
										ofPushStyle();
										ofSetColor(image2.getColor(readPointPairVector[i].point2.x, readPointPairVector[i].point2.y));
										// 色を塗る
										// 左から右
										ofRect(currentRectRightPointVector[i].x*pixelSize.x, currentRectRightPointVector[i].y*pixelSize.x, pixelSize.x, pixelSize.y);
										//ofRect(currentRectRightPointVector[i].x, currentRectRightPointVector[i].y, pixelSize.x, pixelSize.y); //n3
										ofPopStyle();
								}
						}

						if (!enableFixFrameExchange) {
								// 右から左(交換版からオリジナルへの移行)
								for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
										// 到達点
										ofPoint goalPoint(readPointPairVector[i].point1.x, readPointPairVector[i].point1.y);
										// x軸
										if (goalPoint.x-currentRectLeftPointVector[i].x > 0) {
												currentRectLeftPointVector[i].x++;
										}else if(goalPoint.x-currentRectLeftPointVector[i].x < 0){
												currentRectLeftPointVector[i].x--;
										}
										// y軸
										if (goalPoint.y-currentRectLeftPointVector[i].y > 0) {
												currentRectLeftPointVector[i].y++;
										}else if(goalPoint.y-currentRectLeftPointVector[i].y < 0){
												currentRectLeftPointVector[i].y--;
										}
										
										ofPushStyle();
										ofSetColor(image1.getColor(readPointPairVector[i].point1.x, readPointPairVector[i].point1.y));
										// 色を塗る
										ofRect(currentRectLeftPointVector[i].x, currentRectLeftPointVector[i].y, 1, 1);
										ofPopStyle();
								}
						}
						//				ofPopMatrix();
						
						// 左右もしくは上下にしか動かないやりかた
						//				for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
						//						// 到達点
						//						ofPoint goalPoint(readPointPairVector[i].point2.x+image1.getWidth()+marginBetweenImages, readPointPairVector[i].point2.y);
						//						if (abs(readPointPairVector[i].point2.x-readPointPairVector[i].point1.x) <
						//								abs(readPointPairVector[i].point2.y-readPointPairVector[i].point1.y)) {
						//								// x方向が先に移動
						//								if (goalPoint.x-currentRectRightPointVector[i].x > 0) {
						//										currentRectRightPointVector[i].x++;
						//								}else if(goalPoint.x-currentRectRightPointVector[i].x < 0){
						//										currentRectRightPointVector[i].x--;
						//								}else{
						//										//y方向に移動
						//										if (goalPoint.y-currentRectRightPointVector[i].y > 0) {
						//												currentRectRightPointVector[i].y++;
						//										}else if(goalPoint.y-currentRectRightPointVector[i].y < 0){
						//												currentRectRightPointVector[i].y--;
						//										}else{
						//												//ゴールに到着しているので、なにもしない
						//										}
						//								}
						//						}else{
						//								// y方向が先に移動
						//								// y軸
						//								if (goalPoint.y-currentRectRightPointVector[i].y > 0) {
						//										currentRectRightPointVector[i].y++;
						//								}else if(goalPoint.y-currentRectRightPointVector[i].y < 0){
						//										currentRectRightPointVector[i].y--;
						//								}else{
						//										//x方向に移動
						//										if (goalPoint.x-currentRectRightPointVector[i].x > 0) {
						//												currentRectRightPointVector[i].x++;
						//										}else if(goalPoint.x-currentRectRightPointVector[i].x < 0){
						//												currentRectRightPointVector[i].x--;
						//										}else{
						//												//ゴールに到着しているので何もしない
						//										}
						//								}
						//						}
						//						ofPushStyle();
						//						ofSetColor(image2.getColor(readPointPairVector[i].point2.x, readPointPairVector[i].point2.y));
						//						// 色を塗る
						//						ofRect(currentRectRightPointVector[i].x, currentRectRightPointVector[i].y, 1, 1);
						//						ofPopStyle();
						//				}
						
				}
				if (animationToExchange) {
						// ofCircle(0, 0, 10);
						// 1ピクセルの四角形での描画
						// x軸,y軸それぞれ1個ずつ動くやりかた
						// 左から右(オリジナルから交換版への移行)
						//				ofPushMatrix();
						//				ofTranslate((ofGetWidth()-marginBetweenImages)/2.0-image1.getWidth(), (ofGetHeight()-image1.getHeight())/2.0);
						if (!enableFixFrameExchange) {
								for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
										// 到達点
										ofPoint goalPoint(readPointPairVector[i].point2.x+image1.getWidth()+marginBetweenImages, readPointPairVector[i].point2.y);
										// x軸
										if (goalPoint.x-currentRectRightPointVector[i].x > 0) {
												currentRectRightPointVector[i].x++;
										}else if(goalPoint.x-currentRectRightPointVector[i].x < 0){
												currentRectRightPointVector[i].x--;
										}
										// y軸
										if (goalPoint.y-currentRectRightPointVector[i].y > 0) {
												currentRectRightPointVector[i].y++;
										}else if(goalPoint.y-currentRectRightPointVector[i].y < 0){
												currentRectRightPointVector[i].y--;
										}
										
										ofPushStyle();
										ofSetColor(image1.getColor(readPointPairVector[i].point1.x, readPointPairVector[i].point1.y));
										// 色を塗る
										// 左から右
										ofRect(currentRectRightPointVector[i].x, currentRectRightPointVector[i].y, 1, 1);
										ofPopStyle();
								}
						}
						// 右から左(オリジナルから交換版への移行)
						for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
								// 到達点
								ofPoint goalPoint(readPointPairVector[i].point1.x, readPointPairVector[i].point1.y);
								// x軸
								if (goalPoint.x-currentRectLeftPointVector[i].x > 0) {
										currentRectLeftPointVector[i].x++;
								}else if(goalPoint.x-currentRectLeftPointVector[i].x < 0){
										currentRectLeftPointVector[i].x--;
								}
								// y軸
								if (goalPoint.y-currentRectLeftPointVector[i].y > 0) {
										currentRectLeftPointVector[i].y++;
								}else if(goalPoint.y-currentRectLeftPointVector[i].y < 0){
										currentRectLeftPointVector[i].y--;
								}
								
								ofPushStyle();
								ofSetColor(image2.getColor(readPointPairVector[i].point2.x, readPointPairVector[i].point2.y));
								// 色を塗る
								//pixel sizeを使用する
								ofRect(currentRectLeftPointVector[i].x*pixelSize.x, currentRectLeftPointVector[i].y*pixelSize.y, pixelSize.x, pixelSize.y);
								// ofRect(currentRectLeftPointVector[i].x, currentRectLeftPointVector[i].y, pixelSize.x, pixelSize.y);
								ofPopStyle();
						}
						//				ofPopMatrix();
				}
				ofPopMatrix();

		
		
		//DELEME: debug
//		ofPushStyle();
//		ofSetColor(ofColor::lightBlue);
//		ofLine(ofGetWidth()/2.0, 0, ofGetWidth()/2.0, ofGetHeight());
//		ofLine(0, ofGetHeight()/2.0, ofGetWidth(), ofGetHeight()/2.0);
//		ofPopStyle();
}

double testApp::getColorDistance(ofColor color1, ofColor color2){
    return sqrt(double((color1.r-color2.r)*(color1.r-color2.r)+(color1.g-color2.g)*(color1.g-color2.g)+(color1.b-color2.b)*(color1.b-color2.b)));
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    drawImageSwitch = true;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case '0':
            ofEnableBlendMode(OF_BLENDMODE_DISABLED);
            break;
        case '1':
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            break;
        case '2':
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            break;
        case '3':
            ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
            break;
        case '4':
            ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
            break;
        case '5':
            ofEnableBlendMode(OF_BLENDMODE_SCREEN);
            break;
        case 'a':
            ofEnableAntiAliasing();
            break;
        case 'b':
            ofBackground(0, 0, 0, 255);
            break;
				case 'c':
						animationToExchange = true;
						setupCurrentPointVector(readPointPairVector);
						break;
        case 'd':
            ofDisableAlphaBlending();
            break;
        case 'e':
            ofEnableAlphaBlending();
            break;
				case 'g':
						animationToExchange = animationToExchange ? false : true;
						animationToOriginal = animationToExchange ? false : true;
						setupCurrentPointVector(readPointPairVector);
						break;
				case 'o':
						animationToOriginal = true;
						setupCurrentPointVector(readPointPairVector);
						break;
        case 'r':
            //read file
						readPointPairVector.clear();
            importFileForStruct(&readPointPairVector, calucurateFileName);
//            setupCurrentPointVector(readPointPairVector);
//            for (int i=0; i<image1.getHeight(); i++) {
//                for (int j=0; j<image1.getWidth(); j++) {
//                    image1.setColor(readPointPairVector[i*(int)image1.getWidth()+j].point1.x,
//                                    readPointPairVector[i*(int)image1.getWidth()+j].point1.y,
//                                    image2.getColor(readPointPairVector[i*(int)image1.getWidth()+j].point2.x,
//                                                    readPointPairVector[i*(int)image1.getWidth()+j].point2.y));
//                }
//            }
//						dataReaded = true;
            break;
        case 's':
            ofDisableAntiAliasing();
            break;
				case 't':
						image1.clear();
						usedOrNotVector.assign((int)(image1.getWidth()*image1.getHeight()), false);
						image1.setFromPixels(videoGrabber.getPixelsRef());
						image1.mirror(false, true);
						calucurateAndWrite();
						break;
				case 'u':
						importFileForStruct(&readPointPairVector, calucurateFileName);// ファイル名を書けるようにする
            setupCurrentPointVector(readPointPairVector);
						break;
        case 'w':
            ofBackground(255, 255, 255, 255);
            break;
        case ' ':
						calucurateAndWrite();
            break;
        default:
            break;
    }
}

void testApp::calucurateAndWrite(){
		saveToFile = true;
		ofLog(OF_LOG_NOTICE, ofGetTimestampString()+": calculating...");
		//ペアのデータを作成 -->
		vector<pointPair> pointPairVectorForWriting;
		for (int k=0; k<image1.getWidth()*image1.getHeight(); k++) {
				if (k <= image1.getWidth()*image1.getHeight()) {
						// 左の画像を右の画像に入れ替える
						ofPoint referencePoint;
						if (enableRandomExchange) {
								//ランダムで交換
								referencePoint = ofPoint(exchangeOrderVector[k]%(int)image1.getWidth(), exchangeOrderVector[k]/(int)image1.getWidth());
						}else{
								//上から順に交換
								referencePoint = ofPoint(k%(int)image1.getWidth(), k/(int)image1.getWidth());
						}
						// 距離を調べる元になる色
						ofColor referenceColor = image1.getColor(referencePoint.x, referencePoint.y);
						// for文の中で見つかった今のところの最小距離を保存しておく
						double minimumDistance = getColorDistance(ofColor(0, 0, 0), ofColor(255, 255, 255)); // 初期化はgetColorDistance関数で、最大の距離になるもの
																																																 // for文の中で見つかった今のところの最小点を保存しておく
						ofPoint minimumDistancePoint(1000, 1000);
						unsigned char * checkImagePixels = image2.getPixels();
						// 最小の距離の色を調べる
						for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
								if (!enableDuplicate) {
										// 重複を許さない
										if (!usedOrNotVector[i]) {
												ofColor checkColor = ofColor(checkImagePixels[i*3],
																										 checkImagePixels[i*3+1],
																										 checkImagePixels[i*3+2]);
												double checkDistance = getColorDistance(referenceColor, checkColor);
												if (checkDistance < minimumDistance) {
														minimumDistance = checkDistance;
														minimumDistancePoint = ofPoint(i%(int)image1.getWidth(), i/(int)image1.getWidth());
												}
										}
								} else{
										ofColor checkColor = ofColor(checkImagePixels[i*3],
																								 checkImagePixels[i*3+1],
																								 checkImagePixels[i*3+2]);
										double checkDistance = getColorDistance(referenceColor, checkColor);
										if (checkDistance < minimumDistance) {
												if (!usedOrNotVector[i]) {
														minimumDistance = checkDistance;
														minimumDistancePoint = ofPoint(i%(int)image1.getWidth(), i/(int)image1.getWidth());
												}
												// 同じ色の重複を許す
												minimumDistance = checkDistance;
												minimumDistancePoint = ofPoint(i%(int)image1.getWidth(), i/(int)image1.getWidth());
										}
								}
						}
						
						//最も近い距離の色の組み合わせをベクターに保存
						pointPair tempPointPair;
						tempPointPair.point1 = referencePoint;
						tempPointPair.point2 = minimumDistancePoint;
						
						// 重複を許さない
						if (!enableDuplicate) {
								//再利用しないために、使用したPointを保存
								usedOrNotVector[minimumDistancePoint.y*(int)image2.getWidth()+minimumDistancePoint.x] = true;
						}
						// 最短距離の色のペアを保存
						pointPairVectorForWriting.push_back(tempPointPair);
				}
		}
		ofLog(OF_LOG_NOTICE, "pointPairVectorForWriting done!");
		//<-- ペアのデータを作成
		calucurateFileName = "deleteme/foo_"+ofGetTimestampString()+".dat";
		exportFileForStruct(pointPairVectorForWriting, calucurateFileName);
		ofLog(OF_LOG_NOTICE, ofToString("writing file..."));
		saveToFile = false;
		ofLog(OF_LOG_NOTICE, ofGetTimestampString()+": done");

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

void testApp::exportFile(){
    vector<unsigned int> testvector;
    testvector.push_back(4);
    testvector.push_back(2);
    testvector.push_back(7);
    testvector.push_back(30);
    testvector.push_back(31);
    
    int count = testvector.size();
    ofstream out(ofToDataPath("deleteme/foo_"+ofToString(ofGetYear())+"-"+ofToString(ofGetMonth())+"-"+ofToString(ofGetDay())+"_"+ofToString(ofGetHours())+"-"+ofToString(ofGetMinutes())+"-"+ofToString(ofGetSeconds())+".dat").c_str(),ios::out | ios::binary);
    if (!out) return 1;
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));// out.write((char *) &count, sizeof(count)); // 最初にサイズを保存
    out.write(reinterpret_cast<const char*>(&testvector[0]), testvector.size() * sizeof(int));
    out.close();
}

void testApp::exportFileForStruct(vector<pointPair> ppv, string fileName){
		// fileName should be like foo.dat
    int count = ppv.size();
    ofstream out(ofToDataPath(fileName).c_str(),ios::out | ios::binary);
    if (!out) return 1;
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    out.write(reinterpret_cast<const char*>(&ppv[0]), ppv.size() * sizeof(pointPair));
    out.close();
}

void testApp::importFile(){
    vector<int> testvector;
    int count = 0;
    ifstream in(ofToDataPath("deleteme/foo_2014-05-21-00-25-47-197.dat").c_str(), ios::in | ios::binary);
    if ( !in ) return 1;
    in.read(reinterpret_cast<char*>(&count), sizeof(count)); //countのサイズ分をcountに入れる(一個目のデータがサイズであることが前提)
    testvector.assign(count, 0); //testvectorをcountの数個だけ、0で初期設定する
    in.read(reinterpret_cast<char*>(&testvector[0]), testvector.size()*sizeof(int)); //ベクタの最初の位置を指定, intのサイズ分*ベクタの大きさ
    in.close();
    for ( int i = 0; i < testvector.size(); ++i ){
        cout << testvector[i] << endl;
    }
}

void testApp::importFileForStruct(vector<pointPair> *ppv, string fileName){
		// fileName should be like foo.dat
    ppv->clear();
    int count = 0;
    ifstream in(ofToDataPath(fileName).c_str(), ios::in | ios::binary);
    if ( !in ) return ppv;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    pointPair pp;
    pp.point1 = ofPoint(0, 0);
    pp.point2 = ofPoint(0, 0);
    ppv->assign(count, pp);
    in.read(reinterpret_cast<char*>(&ppv->front()), ppv->size()*sizeof(pointPair));
    ofLog(OF_LOG_NOTICE, "reading done!"+ofToString(ppv->size()));
    in.close();
}

void testApp::setupCurrentPointVector(vector<pointPair> ppv){
		currentPointVector.clear();
		currentRectRightPointVector.clear();
		currentRectLeftPointVector.clear();
		for (int i=0; i<image1.getWidth()*image1.getHeight(); i++) {
				currentPointVector[i] = ppv[i].point1;
				currentRectRightPointVector[i] = ppv[i].point1;
				if (enableFixFrameExchange) {
						currentRectLeftPointVector[i].x = ppv[i].point2.x;
						currentRectLeftPointVector[i].y = ppv[i].point2.y;						
				}else{
						currentRectLeftPointVector[i].x = ppv[i].point2.x+image1.getWidth()+marginBetweenImages;
						currentRectLeftPointVector[i].y = ppv[i].point2.y;
				}
		}
}

void testApp::moveUpDownLeftRight(){
		
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}
