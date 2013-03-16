#ifndef BlueSquare_H
#define BlueSquare_H

#include "wWaveVisual.h"

class BlueSquare : 
	public wWaveVisual
{
public:
	BlueSquare(float length);
	~BlueSquare(void);

	void draw(void);
};
#endif


