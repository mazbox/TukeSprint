/**
 * ColorScheme.h
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */
#include "GuiControl.h"

class ColorScheme {
public:
	GuiControl *control;
	bool enabled;
	string name;
	string thumbnailUrl;
	int color1;
	int color2;
	int color3;
	ofImage thumbnail;
	ColorScheme(string name, string thumbnailUrl, int color1, int color2, int color3) {
		enabled = false;
		this->name = name;
		this->thumbnailUrl = thumbnailUrl;
		thumbnail.loadImage(thumbnailUrl);
		this->color1 = color1;
		this->color2 = color2;
		this->color3 = color3;
	}
};