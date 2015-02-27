//
//  TraySlider.cpp
//
//  Created by Torran Kahleck on 2/24/15.
//  Copyright (c) 2015 Torran Kahleck
//

#include "TraySlider.h"

#define toggleButtonWidth 18

TraySlider *TraySlider::create() {
    TraySlider *pRet = new TraySlider();
    if (pRet && pRet->init()) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

TraySlider *TraySlider::createWithSpritesRectAndColors(cocos2d::Vector<cocos2d::Sprite*> pSprites, cocos2d::Rect pRect, cocos2d::Color4F pBtnColor, cocos2d::Color4F pBtnBrdrColor, cocos2d::Color4F pTrayColor, cocos2d::Color4F pTrayBrdrColor, cocos2d::Color4F pTrayBtnColor, cocos2d::Color4F pTrayBtnBrdrColor) {
    TraySlider *pRet = new TraySlider();
    if (pRet && pRet->initWithSpritesRectAndColors(pSprites, pRect, pBtnColor, pBtnBrdrColor, pTrayColor, pTrayBrdrColor, pTrayBtnColor, pTrayBtnBrdrColor)) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool TraySlider::initWithSpritesRectAndColors(cocos2d::Vector<cocos2d::Sprite*> pSprites, cocos2d::Rect pRect, cocos2d::Color4F pBtnColor, cocos2d::Color4F pBtnBrdrColor, cocos2d::Color4F pTrayColor, cocos2d::Color4F pTrayBrdrColor, cocos2d::Color4F pTrayBtnColor, cocos2d::Color4F pTrayBtnBrdrColor) {
    if ( !Node::init() ) {
        return false;
    }
    
    for (int i = 0; i < pSprites.size(); i++) {
        sprites.pushBack(pSprites.at(i));
    }
    
    trayRect = pRect;
    stretchBy = sprites.size();
    isToggled = false;
    isMoving = false;
    selectedSprite = 0;
    lastSprite = 0;
    moveDist = trayRect.getMaxX() - stretchBy * trayRect.origin.x;
    
    curBounds = pRect;
    curBounds.size.width += toggleButtonWidth;
    
    btnColor = pBtnColor;                       // Main button color
    btnBrdrColor = pBtnBrdrColor;               // Main button's border color
    trayColor = pTrayColor;                     // Sliding tray color
    trayBrdrColor = pTrayBrdrColor;             // Sliding tray's border color
    trayBtnColor = pTrayBtnColor;               // Toggle button color
    trayBtnBrdrColor = pTrayBtnBrdrColor;       // Toggle button's border color
    
    this->setAnchorPoint(cocos2d::Vec2(0,0));
    
    // Create Vec2 array from passed in rectangle
    cocos2d::Vec2 mainRect[4];
    mainRect[0] = cocos2d::Vec2(trayRect.origin.x , trayRect.origin.y);
    mainRect[1] = cocos2d::Vec2(trayRect.origin.x + trayRect.size.width, trayRect.origin.y);
    mainRect[2] = cocos2d::Vec2(trayRect.origin.x + trayRect.size.width, trayRect.origin.y + trayRect.size.height);
    mainRect[3] = cocos2d::Vec2(trayRect.origin.x, trayRect.origin.y + trayRect.size.height);

    // Create main button programmatically using passed-in rectangle
    trayButton = cocos2d::DrawNode::create();
    trayButton->drawPolygon(mainRect, 4, btnColor, .5, btnBrdrColor);
    this->addChild(trayButton, 21);
    
    // Create sliding tray programmatically using passed-in rectangle
    traySlide = cocos2d::DrawNode::create();
    traySlide->drawPolygon(mainRect, 4, trayColor, 0, trayBrdrColor);
    this->addChild(traySlide, 20);
    
    // Create Vec2 array for toggle button
    cocos2d::Vec2 toggleRect[4];
    toggleRect[0] = cocos2d::Vec2(trayRect.origin.x + trayRect.size.width, trayRect.origin.y);
    toggleRect[1] = cocos2d::Vec2(trayRect.origin.x + trayRect.size.width + toggleButtonWidth, trayRect.origin.y);
    toggleRect[2] = cocos2d::Vec2(trayRect.origin.x + trayRect.size.width + toggleButtonWidth, trayRect.origin.y + trayRect.size.height);
    toggleRect[3] = cocos2d::Vec2(trayRect.origin.x + trayRect.size.width, trayRect.origin.y + trayRect.size.height);
    
    // Create toggle button programmatically using above array
    toggleButton = cocos2d::DrawNode::create();
    toggleButton->drawPolygon(toggleRect, 4, trayBtnColor, .5, trayBtnBrdrColor);
    this->addChild(toggleButton, 22);
    
    // Create toggle button's arrow using "toggleArrow.png"
    toggleArrow = cocos2d::Sprite::create("toggleArrow.png");
    toggleArrow->setPosition(cocos2d::Point(trayRect.origin.x + trayRect.size.width + toggleButtonWidth/2,
                                   trayRect.origin.y + trayRect.size.height/2));
    toggleButton->addChild(toggleArrow);
    
    // Create selected item sprite using texture of first sprite in passed-in Vector
    trayItemSelected = cocos2d::Sprite::createWithSpriteFrame(sprites.at(selectedSprite)->getSpriteFrame());
    trayItemSelected->setPosition(cocos2d::Point(trayRect.origin.x + trayRect.size.width/2,
                                                 trayRect.origin.y + trayRect.size.height/2));
    scaleSprite(trayItemSelected);
    trayButton->addChild(trayItemSelected);
    
    // Create tray items from passed-in Vector sprites, set to be invisible when tray is closed
    for (int i = 0; i < sprites.size(); i++) {
        cocos2d::Sprite *trayItem = sprites.at(i);
        trayItem->setPosition(cocos2d::Point((((i + 1) * trayRect.size.width) + trayItemSelected->getPositionX()),
                                             trayItemSelected->getPositionY()));
        scaleSprite(trayItem);
        trayItem->setVisible(false);
        this->addChild(trayItem, 22 + i);
    }
    
    return true;
}

/*
 * Private method, scales passed-in sprite to fit user defined button size
 * @param: sp - sprite to scale
 */
void TraySlider::scaleSprite(cocos2d::Sprite* sp) {
    float scaleBy = 1;
    if (sp->getBoundingBox().size.width > sp->getBoundingBox().size.height) {
        if (sp->getBoundingBox().size.width > trayRect.size.width) {
            scaleBy = trayRect.size.width / sp->getBoundingBox().size.width - .1;
        }
    } else {
        if (sp->getBoundingBox().size.height > trayRect.size.height) {
            scaleBy = trayRect.size.height / sp->getBoundingBox().size.height - .1;
        }
    }
    sp->setScale(scaleBy);
}

/*
 * Public method, opens tray
 */
void TraySlider::open() {
    isMoving = true; // Prevents multiple touches during tray movement
    traySlide->setAnchorPoint(cocos2d::Vec2(0,.5));
    
    // Spawn actions to scale and move tray into place
    cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
    actions.pushBack(cocos2d::ScaleBy::create(.2, stretchBy, 1));
    actions.pushBack(cocos2d::MoveTo::create(.2, cocos2d::Vec2(moveDist, 0)));
    auto parallel = cocos2d::Spawn::create(actions);
    
    // Sprite bouncing actions
    auto spriteBounce = cocos2d::CallFunc::create([=]()->void{
        for (int i = 0; i < sprites.size(); i++) {
            cocos2d::Vector<cocos2d::FiniteTimeAction*> itemActions;
            sprites.at(i)->setVisible(true);
            auto fo = cocos2d::ScaleBy::create(.15, 1.25);
            auto rev = fo->cocos2d::ScaleBy::reverse();
            itemActions.pushBack(fo);
            itemActions.pushBack(rev);
            auto itemOpenSequence = cocos2d::Sequence::create(itemActions);
            sprites.at(i)->runAction(itemOpenSequence);
        }
    });
    
    // Set sequence of above actions
    auto fullSeq = cocos2d::Sequence::create(parallel, spriteBounce,
                                             cocos2d::CallFunc::create([=]()->void{ isMoving=false; }),
                                             nullptr);
    traySlide->runAction(fullSeq);
    
    // Move toggle button and flip arrow
    auto moveToggle = cocos2d::MoveTo::create(.25, cocos2d::Vec2(trayRect.size.width * stretchBy, 0));
    toggleButton->runAction(moveToggle);
    toggleArrow->setFlippedX(true);
    
    isToggled = true;
    curBounds = trayRect;
    curBounds.size.width += curBounds.size.width * stretchBy + toggleButtonWidth;
}

/*
 * Public method, closes tray
 */
void TraySlider::close() {
    isMoving = true; // Prevents multiple touches during tray movement
    
    // Sprite select bounce actions
    auto spriteBounce = cocos2d::CallFunc::create([=]()->void{
        if (selectedSprite >= 1 && selectedSprite <= sprites.size()) {
            trayItemSelected->setSpriteFrame(sprites.at(selectedSprite - 1)->getSpriteFrame());
            cocos2d::Vector<cocos2d::FiniteTimeAction*> itemActions;
            auto foSelect = cocos2d::ScaleBy::create(.15, 1.25);
            auto revSelect = foSelect->cocos2d::ScaleBy::reverse();
            itemActions.pushBack(foSelect);
            itemActions.pushBack(revSelect);
            auto itemSelectedSequence = cocos2d::Sequence::create(itemActions);
            trayItemSelected->runAction(itemSelectedSequence);
            
            cocos2d::Vector<cocos2d::FiniteTimeAction*> itemPressedAction;
            auto foPress = cocos2d::ScaleBy::create(.15, .75);
            auto revPress = foPress->cocos2d::ScaleBy::reverse();
            itemPressedAction.pushBack(foPress);
            itemPressedAction.pushBack(revPress);
            auto itemPressOpenSequence = cocos2d::Sequence::create(itemPressedAction);
            sprites.at(selectedSprite - 1)->runAction(itemPressOpenSequence);
        } else {
            selectedSprite = lastSprite;
        }
    });
    
    // Scale and move tray actions, calls function to set sprites invisible just before close
    //  and to move toggle button
    cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
    actions.pushBack(cocos2d::ScaleTo::create(.25, 1, 1));
    actions.pushBack(cocos2d::MoveTo::create(.25, cocos2d::Vec2(0,0)));
    actions.pushBack(cocos2d::CallFunc::create([=]()->void{
        for (int i = 0; i < sprites.size(); i++) {
            sprites.at(i)->setVisible(false);
        }
        auto moveToggle = cocos2d::MoveTo::create(.25, cocos2d::Vec2(0, 0));
        toggleButton->runAction(moveToggle);
        toggleArrow->setFlippedX(false);
    }));
    auto parallel = cocos2d::Spawn::create(actions);
    
    // Set sequence of above actions
    auto fullSeq = cocos2d::Sequence::create(spriteBounce, cocos2d::DelayTime::create(.3), parallel, cocos2d::CallFunc::create([=]()->void{ isMoving=false; }), nullptr);
    traySlide->runAction(fullSeq);
    
    isToggled = false;
    curBounds = trayRect;
    curBounds.size.width += toggleButtonWidth;
}

/*
 * Public method, checks to see if touched point is in tray bounds
 * @param:  pt - touched point
 * @return: true = in bounds, false = not in bounds
 */
bool TraySlider::inBounds(cocos2d::Point pt) {
    bool inBounds = curBounds.containsPoint(pt);
    if (inBounds) {
        lastSprite = selectedSprite;
        selectedSprite = floor((pt.x - trayRect.origin.x) / trayRect.size.width);
    }
    return inBounds;
}

/*
 * Public method, checks if tray is open
 * @return: true = open, false = closed
 */
bool TraySlider::isOpen() {
    return isToggled;
}

/*
 * Public method, checks if tray is moving
 * @return: true = moving, false = not moving
 */
bool TraySlider::moving(){
    return isMoving;
}

/*
 * Public method, gets the selected/active item from the tray
 * @return: int corresponding to selected item's index in user-defined passed-in Vector
 */
int TraySlider::getSelected() {
    return selectedSprite - 1;
}