/*
 *  GuiTitle.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 17/01/2011.
 *
 */

class GuiTitle: public GuiControl {
public:
	
	
	void setup() {
		height = 24;
		width = 110;
		font = resources->getDefaultFont();
		color = 0xFFFFFF;
	}
	
	int color;
	
	void draw() {
		ofSetHexColor(color);
		font->drawString(name, x+3, y+14);
		//ofLine(x, y+height-5, x+width, y+height-5);
	}
	ofTrueTypeFont *font;
};