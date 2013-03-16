/**
 * Class:	Rectangles
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Rectangles_H
#define Rectangles_H

#include "Model.h"
#include "Visuals/Visual.h"

namespace WavesVisuals
{
	class Rectangles :
		public Visual
	{
	public:
		typedef struct
		{
			unsigned int axis;
			unsigned int createdTime;
			float depth;
			bool enabled;
			float height;
			Point3f offset;
			float width;
		} RectData;

		Rectangles(unsigned int maxRects = DEFAULT_MAX_RECTS);
		~Rectangles(void);

		void draw(void);
		Visual *getInstance(void) const;
		void update(void);

	private:
		static const std::string	AXIS_NAME;
		static const std::string	CENTRE_OFFSET_NAME;
		static const unsigned int	DEFAULT_MAX_RECTS = 25;
		static const std::string	INTENSITY_NAME;
		static const unsigned int	LIFETIME_MS = 5000;
		static const float			MAX_RECT_DEPTH;
		static const float			MAX_RECT_HEIGHT;
		static const float			MAX_RECT_WIDTH;
		static const float			MAX_ROTATION_INCREMENT;
		static const float			MAX_X_OFFSET;
		static const float			MAX_Y_OFFSET;
		static const float			MAX_Z_OFFSET;
		static const float			MAX_ZOOM;
		static const std::string	MODEL_PATH;
		static const float			NEW_THRESHOLD;
		static const std::string	RECT_DEPTH_NAME;
		static const std::string	RECT_WIDTH_NAME;
		static const std::string	RECT_HEIGHT_NAME;
		static const std::string	ROTATION_SPEED_NAME;
		static const unsigned int	UPDATE_RATE_MS = 5;
		static const std::string	ZOOM_SPEED_NAME;

		unsigned int last;
		unsigned int maxRects;
		Model *model;
		RectData *rects;
		unsigned int rectPtr;
		float rotationVal;
		GLsizei vertexBufferSizeInBytes;
		std::string vertexBufferKey;
		float zoom;
		float zoomSign;

		void addRect(void);
		unsigned int initVbo(void);
		void newRect(void);
	};
}
#endif

