/**
 * Class:	Matrixf
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 *
 * Matrix in the form:
 * m1 m4 m7 
 * m2 m5 m8
 * m3 m6 m9
 */
#include <exception>
#include "JDHUtility/Matrixf.h"

namespace JDHUtility
{
	/* CONSTRUCTORS */
	Matrixf::Matrixf(unsigned int width, unsigned int height)
	{
		this->width = width;
		this->height = height;

		unsigned int size = width * height;
		matrix = new float[size];
	}

	Matrixf::~Matrixf(void)
	{
		delete[] matrix;
	}

	/* PUBLIC MEMBER FUNCTIONS */
	Matrixf *Matrixf::add(Matrixf &m) const
	{
		if(getWidth() != m.getWidth() || getHeight() != m.getHeight()) throw std::exception("Exception: incorrect matrix dimensions for add operation");

		Matrixf *result = new Matrixf(width, height);
		
		for(unsigned int i = 0; i < (width * height); i++)
		{
			result->getMatrix()[i] = (matrix[i] + m.getMatrix()[i]);
		}

		return result;
	}

	unsigned int Matrixf::getHeight(void) const
	{
		return height;
	}

	float *Matrixf::getMatrix(void)
	{
		return matrix;
	}

	float Matrixf::getValue(unsigned int x, unsigned int y) const
	{
		if(x >= width || y >= height) throw std::exception("Exception: matrix index out of range");

		return matrix[(height * x) + y];
	}

	unsigned int Matrixf::getWidth(void) const
	{
		return width;
	}

	Matrixf *Matrixf::multiply(Matrixf &m) const
	{
		if(getWidth() != m.getHeight()) throw std::exception("Exception: incorrect matrix dimensions for multiply operation");

		Matrixf *result = new Matrixf(m.getWidth(), getHeight());
		
		float val;
		for(unsigned int i = 0; i < result->getWidth(); i++)
		{
			for(unsigned int j = 0; j < height; j++)
			{
				val = 0;
				for(unsigned int k = 0; k < width; k++)
				{
					val += (getValue(k, j) * m.getValue(i, k));
				}

				result->setValue(val, i, j);
			}
		}

		return result;
	}

	void Matrixf::setValue(float value, unsigned int x, unsigned int y)
	{
		if(x >= width || y >= height) throw std::exception("Exception: matrix index out of range");

		matrix[(height * x) + y] = value;
	}

}