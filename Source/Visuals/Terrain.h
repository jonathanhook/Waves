/**
 * Class:	Terrain
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Terrain_H
#define Terrain_H

#include "Visuals/Visual.h"

namespace WavesVisuals
{
	class Model;

	class Terrain :
		public Visual
	{
	public:
		Terrain(void);
		~Terrain(void);

		void draw(void);
		Visual *getInstance(void) const;
		void update(void);

	private:
		static const std::string HEIGHT_OFFSET_NAME;
		static const std::string MODEL_PATH;
		static const std::string ROTATION_SPEED_NAME;
		static const float MAX_ROTATION_INCREMENT;
		static const unsigned int UPDATE_RATE_MS = 16;

		CGcontext cg_Context;
		CGparameter cgp_heightOffset;
		CGparameter cgp_modelViewProj;
		CGparameter cgp_opacity;
		CGprogram cg_Program;	
		CGprofile cg_VertexProfile;
		unsigned int last;
		Model *model;
		float rotation;
		std::string vertexBufferKey;
		GLsizei vertexBufferSizeInBytes;

		void initShaders(void);
		unsigned int initVbo(void);
		void setShaderConstants(void);
	};
}
#endif