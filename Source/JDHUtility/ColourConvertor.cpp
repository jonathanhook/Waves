/**
 * Class:	ColourConvertor
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/ColourConvertor.h"
#include "Interpolator.h"

namespace JDHUtility
{
	/* PUBLIC STATIC FUNCTIONS */
	Colour3f ColourConvertor::hslToRgb(const Vector3f &hsl)
	{
		Colour3f result;
		float h = hsl.getX();
		float s = hsl.getY();
		float l = hsl.getZ();
		
		//If S=0, define R, G, and B all to L
		if(s == 0.0f) 
		{
			result.setR(hsl.getZ());
			result.setG(hsl.getZ());
			result.setB(hsl.getZ());
		}
		//Otherwise, test L.
		else
		{
			float temp2 = 0.0f;

			//If L < 0.5, temp2=L*(1.0+S)
			if(l < 0.5f)
			{
				temp2= l * (1.0f + s);
			}
			//If L >= 0.5, temp2=L+S - L*S
			else
			{
				temp2 = (l + s) - (l * s);
			}
						
			//temp1 = 2.0*L - temp2
			float temp1 = 2.0f * l - temp2;

			//Convert H to the range 0-1
			//For each of R, G, B, compute another temporary value, temp3, as follows:
			//for R, temp3=H+1.0/3.0
			float rTemp3 = h + 1.0f / 3.0f;
			rTemp3 = hslClamp(rTemp3);

			//for G, temp3=H
			float gTemp3 = h;
			gTemp3 = hslClamp(gTemp3);

			//for B, temp3=H-1.0/3.0
			float bTemp3 = h - 1.0f / 3.0f;
			bTemp3 = hslClamp(bTemp3);

			//For each of R, G, B, do the following test:
			// R **************************************************
			//If 6.0*temp3 < 1, color=temp1+(temp2-temp1)*6.0*temp3
			float rColor = 0.0f;
			if(6.0f * rTemp3 < 1.0f) 
			{
				rColor = temp1 + (temp2 - temp1) * 6.0f * rTemp3;
			}	
			//Else if 2.0*temp3 < 1, color=temp2
			else if(2.0f * rTemp3 < 1.0f)
			{
				rColor = temp2;
			}
			//Else if 3.0*temp3 < 2, color=temp1+(temp2-temp1)*((2.0/3.0)-temp3)*6.0
			else if(3.0f* rTemp3 < 2.0f)
			{
				rColor = temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - rTemp3) * 6.0f;
			}
			//Else color=temp1
			else rColor = temp1;

			//For each of R, G, B, do the following test:
			// G **************************************************
			//If 6.0*temp3 < 1, color=temp1+(temp2-temp1)*6.0*temp3
			float gColor = 0.0f;
			if(6.0f * gTemp3 < 1.0f) 
			{
				gColor = temp1 + (temp2 - temp1) * 6.0f * gTemp3;
			}	
			//Else if 2.0*temp3 < 1, color=temp2
			else if(2.0f * gTemp3 < 1.0f)
			{
				gColor = temp2;
			}
			//Else if 3.0*temp3 < 2, color=temp1+(temp2-temp1)*((2.0/3.0)-temp3)*6.0
			else if(3.0f* gTemp3 < 2.0f)
			{
				gColor = temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - gTemp3) * 6.0f;
			}
			//Else color=temp1
			else gColor = temp1;

			//For each of R, G, B, do the following test:
			// B **************************************************
			//If 6.0*temp3 < 1, color=temp1+(temp2-temp1)*6.0*temp3
			float bColor = 0.0f;
			if(6.0f * bTemp3 < 1.0f) 
			{
				bColor = temp1 + (temp2 - temp1) * 6.0f * bTemp3;
			}	
			//Else if 2.0*temp3 < 1, color=temp2
			else if(2.0f * bTemp3 < 1.0f)
			{
				bColor = temp2;
			}
			//Else if 3.0*temp3 < 2, color=temp1+(temp2-temp1)*((2.0/3.0)-temp3)*6.0
			else if(3.0f* bTemp3 < 2.0f)
			{
				bColor = temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - bTemp3) * 6.0f;
			}
			//Else color=temp1
			else bColor = temp1;


			//Scale back to the range 0-100 to use the scaling shown in the video color page
			//For the example, R=14, G=90, B=14
			result.setR(rColor);
			result.setG(gColor);
			result.setB(bColor);
		}

		return result;
	}

	Vector3f ColourConvertor::rgbToHsl(const Colour3f &rgb)
	{
		float max = 0.0f;
		float min = 0.0f;

		// clamp values -- remove for speedup
		float r = clamp(rgb.getR());
		float g = clamp(rgb.getG());
		float b = clamp(rgb.getB());

		// find max/min
		findMaxMin(r, g, b, max, min);

		float h = 0.0f;
		float s = 0.0f;
		float l = 0.0f;

		// L = (maxcolor + mincolor)/2 
		l = (max + min) / 2.0f;

		if(max == min)
		{
			h = 0.0f;
			s = 0.0f;
		}
		else
		{
			// If L < 0.5, S=(maxcolor-mincolor)/(maxcolor+mincolor)
			if(l <0.5f) s = (max - min) / (max + min);
			// If L >=0.5, S=(maxcolor-mincolor)/(2.0-maxcolor-mincolor)
			else if(l >= 0.5f) s = (max - min) / (2.0f - max - min);
		
			// If R=maxcolor, H = (G-B)/(maxcolor-mincolor)
			if(r == max) h = (g - b) / (max - min);
			// If G=maxcolor, H = 2.0 + (B-R)/(maxcolor-mincolor)
			else if(g == max) h = 2.0f + (b - r) / (max - min);
			// If B=maxcolor, H = 4.0 + (R-G)/(maxcolor-mincolor)
			else if(b == max) h = 4.0f + (r - g) / (max - min);
		}

		return Vector3f(h, s, l);
	}

	/* PRIVATE MEMBER FUNCTIONS */
	float ColourConvertor::clamp(float f)
	{
		if(f < 0.0f) f = 0.0f;
		else if(f > 1.0f) f = 1.0f;
		return f;
	}

	void ColourConvertor::findMaxMin(float r, float g, float b, float &max, float &min)
	{
		max = r;
		if(g > max) max = g;
		if(b > max) max = b;

		min = r;
		if(g < min) min = g;
		if(b < min) min = b;
	}

	float ColourConvertor::hslClamp(float f)
	{
		//if temp3 < 0, temp3 = temp3 + 1.0
		if(f < 0) return f + 1.0f;
		//if temp3 > 1, temp3 = temp3 - 1.0
		else if(f > 1.0f) return f - 1.0f;
		else return f;
	}
}
