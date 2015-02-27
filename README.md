TraySlider
=======
TraySlider is a cocos2d-x object that offers a fairly plain, tappable button featuring the selected item from a user-defined Vector of Sprites. This button, when clicked, extends a tray of additional items from the same Vector. These items can be tapped themselves, which will cause the item in question to be made the active/selected item featured on the button. The tray's selected item can be accessed through the `getSelected()` method.

Screenshot
----------
![](https://cloud.githubusercontent.com/assets/8550028/6420211/5c504db2-be81-11e4-9ac9-3e9ebdecbba6.gif)

Including in your project
-------------------------
Copy `TraySlider.h` and `TraySlider.cpp` into your `Classes` folder. Copy `toggleArrow.png` into your `Resources` folder. Add `#include "TraySlider.h"` into the `*.h` file of the class which will be creating the tray object.  

Usage
-----
###Creating the TraySlider object

- Create a new tray object in your header file:

```cpp
    TraySlider *tray;
```

- Next, in your `*.cpp` file, create a Vector of Sprites, `cocos2d::Vector<cocos2d::Sprites*>`, containing the objects you'd like displayed in the tray. The item at index 0 will be selected by default.
- After that, create a `Rect` with the origin, width, and height you'd like to be used for the Tray's button, minus the width of the toggle button (defined in `TraySlider.cpp` - 18px by default).
- Then, create the colors you'd like to use for various portions of the buttons and tray as `cocos2d::Color4F`s.
- Lastly, create the TraySlider object with `TraySlider::createWithSpritesRectAndColors(cocos2d::Vector<cocos2d::Sprite*> pSprite, cocos2d::Rect pRect, cocos2d::Color4F pBtnColor, cocos2d::Color4F pBtnBrdrColor, cocos2d::Color4F pTrayColor, cocos2d::Color4F pTrayBrdrColor, cocos2d::Color4F pTrayBtnColor, cocos2d::Color4F pTrayBtnBrdrColor)` and add it to your scene.

- Example below of adding a list of bomb SpriteFrames to the tray with a button starting at 50,50 and a width/height of 60.

```cpp
    cocos2d::Vector<cocos2d::Sprite*> bombFrames;
    char str[100] = {0};
    for (int i = 1; i < 5; i++) {
        sprintf(str, "bomb_%d.png", i);
        Sprite *sp = Sprite::createWithSpriteFrameName(str);
        bombFrames.pushBack(sp);
    }

    cocos2d::Color4F white(1,1,1,.5); // Using a transparent white for the backgrounds of the tray, tray button, and toggle button
    cocos2d::Color4F black(0,0,0,1);  // Using black as the border color for the tray button and toggle button.

    Rect trayRect = Rect(50,50,60,60);
    tray = TraySlider::createWithSpritesRectAndColors(bombFrames, trayRect, white, black, white, white, white, black);
    this->addChild(tray, 20);
```

- Note: The Sprites passed-in will be be scaled down to fit into the Rect will an additional .1 for padding.
- Note: Though the Rect passed-in on creation can be any size, there is no check to make sure the button or tray stays on the screen.
- Note: The toggle arrow is not created programmatically as the diagonal lines didn't look right when rendered. Smaller Rects will result in the this png sticking out of its box. If a small Rect is needed, a new toggle arrow can be easily replaced.

###Reacting to touches

- Use something similar to the following in your scene's `onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)` method.

```cpp
    Point location = touch->getLocationInView();
    location = Director::getInstance()->convertToGL(location);
    if (tray->inBounds(location)) {
        if (tray->isOpen() && !tray->moving()) {
            tray->close();
        }
        else if (!tray->moving()) {
            tray->open();
        }
    }
```

- `inBounds()` will check to see if the touch event was within the bounds of the tray.
- `isOpen()` returns a bool corresponding to the open state of the tray.
- `moving()` returns a bool corresponding to the moving state of the tray and is necessary to prevent multiple touches firing the open/close functions.
- `open()` & `close()` are fairly self-explanatory.

###Getting selected item

- Using `getSelected()` will return the index of the currently selected Sprite from the passed-in Vector.

Developed By
-------
[Torran Kahleck](http://www.torrankaleke.com/ "Personal Site") (tkah)

License
-------
    The MIT License (MIT)

    Copyright (c) 2015 Torran Kahleck

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
