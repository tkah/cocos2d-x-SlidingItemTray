//
//  TraySlider.h
//
//  Created by Torran Kahleck on 2/24/15.
//  Copyright (c) 2015 Torran Kahleck
//

#ifndef BunkerBusting_TraySlider_h
#define BunkerBusting_TraySlider_h

#include "cocos2d.h"

class TraySlider : public cocos2d::Node {
    
private:
    virtual bool initWithSpritesRectAndColors(cocos2d::Vector<cocos2d::Sprite*> pSprites, cocos2d::Rect pRect, cocos2d::Color4F pBtnColor, cocos2d::Color4F pBtnBrdrColor, cocos2d::Color4F pTrayColor, cocos2d::Color4F pTrayBrdrColor, cocos2d::Color4F pTrayBtnColor, cocos2d::Color4F pTrayBtnBrdrColor);
    bool isToggled;
    bool isMoving;
    void scaleSprite(cocos2d::Sprite*);
    
    float stretchBy;
    float moveDist;
    int selectedSprite;
    int lastSprite;
    
    cocos2d::Color4F btnColor;
    cocos2d::Color4F btnBrdrColor;
    cocos2d::Color4F trayColor;
    cocos2d::Color4F trayBrdrColor;
    cocos2d::Color4F trayBtnColor;
    cocos2d::Color4F trayBtnBrdrColor;
    
    cocos2d::Vector <cocos2d::Sprite*> sprites;
    cocos2d::Rect trayRect;
    cocos2d::Rect curBounds;
    cocos2d::Vec2 openArrow[3];
    cocos2d::Vec2 closeArrow[3];
    
    cocos2d::DrawNode *trayButton;
    cocos2d::DrawNode *toggleButton;
    cocos2d::DrawNode *traySlide;
    
    cocos2d::Sprite *toggleArrow;
    cocos2d::Sprite *trayItemSelected;
    
public:
    static TraySlider *create();
    
    static TraySlider *createWithSpritesRectAndColors(cocos2d::Vector<cocos2d::Sprite*> pSprite, cocos2d::Rect pRect, cocos2d::Color4F pBtnColor, cocos2d::Color4F pBtnBrdrColor, cocos2d::Color4F pTrayColor, cocos2d::Color4F pTrayBrdrColor, cocos2d::Color4F pTrayBtnColor, cocos2d::Color4F pTrayBtnBrdrColor);
    void open();
    void close();
    bool isOpen();
    bool moving();
    int getSelected();
    bool inBounds(cocos2d::Point pt);
};

#endif
