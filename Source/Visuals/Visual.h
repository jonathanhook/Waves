/**
 * Class:	Visual
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Visual_H
#define Visual_H

#include <map>
#include <string>
#include <vector>
#include "JDHUtility/Colour3f.h"
#include "JDHUtility/Point3f.h"
#include "JDHUtility/Vector3f.h"
#include "Visuals/Parameter.h"

using namespace JDHUtility;

namespace WavesVisuals
{
	class Visual
	{
	public:
		Visual(std::string name, bool combinedScale = true);
		~Visual(void);

		virtual void draw(void) = 0;
		Colour3f getColour(void);
		const std::string &getName(void) const;
		virtual Visual *getInstance(void) const = 0;
		float getOpacity(void);
		Parameter *getParameter(std::string key);
		const std::map<std::string, Parameter *> getParameters(void) const;
		Point3f getPosition(void);
		Vector3f getRotation(void);
		Vector3f getScale(void);
		virtual void render(void);
		unsigned int renderThumbnail(bool renderWithDefaultValues);
		void setColour(const Colour3f &colour);
		void setOpacity(float opacity);
		void setParameterToDefault(std::string key);
		void setParameterValue(std::string key, float value);
		void setPosition(const Point3f &position);
		void setRotation(const Vector3f &rotation);
		void setScale(const Vector3f &scale);
		virtual void update(void) = 0;

	protected:
		std::string name;
		std::map<std::string, Parameter *> parameters;

		static const std::string	COMBINED_SCALE_NAME;
		static const float			COMBINED_SCALE_DEFAULT;
		static const float			GLOBAL_OPACITY_DEFAULT;
		static const std::string	GLOBAL_OPACITY_NAME;
		static const std::string	HUE_NAME;
		static const float			HUE_DEFAULT;
		static const std::string	LUMINANCE_NAME;
		static const float			LUMINANCE_DEFAULT;
		static const std::string	OPACITY_NAME;
		static const float			OPACITY_DEFAULT;
		static const float			POSITION_DEFAULT;
		static const std::string	POSITION_X_NAME;
		static const std::string	POSITION_Y_NAME;
		static const std::string	POSITION_Z_NAME;
		static const std::string	ROTATION_X_NAME;
		static const std::string	ROTATION_Y_NAME;
		static const std::string	ROTATION_Z_NAME;
		static const float			ROTATION_DEFAULT;
		static const std::string	SATURATION_NAME;
		static const float			SATURATION_DEFAULT;
		static const std::string	SCALE_X_NAME;
		static const std::string	SCALE_Y_NAME;
		static const std::string	SCALE_Z_NAME;
		static const float			SCALE_DEFAULT;
	
	protected:
		float getRandom(void) const;

	private:
		static const unsigned int TEXTURE_WIDTH = 256;
		static const unsigned int TEXTURE_HEIGHT = 128;

		unsigned int depthBuffer;
		unsigned int frameBufferObject;
		unsigned int texture;
		bool thumbnailRendered;

		virtual void initParameters(bool combinedScale);
	};
}
#endif

