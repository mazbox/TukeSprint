/**
 * ColorScheme.h
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */

class ColorScheme {
public:
	bool enabled;
	string name;
	ColorScheme(string name) {
		enabled = false;
		this->name = name;
	}
};