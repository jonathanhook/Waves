/**
 * Class:	SplinePreset
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef SplinePreset_H
#define SplinePreset_H

#include <map>
#include <vector>
#include "JDHUtility/Point2f.h"

using namespace JDHUtility;

namespace Waves
{
	class SplinePreset
	{
	public:
		typedef enum
		{
			FFT,
			TEMPORAL
		} PresetType;

		static SplinePreset *load(std::string file);
		static std::map<unsigned int, SplinePreset *> loadAll(void);
		static std::map<unsigned int, SplinePreset *> loadDefaults(void);
		static unsigned int generateId(void);

		SplinePreset(unsigned int id, std::vector<const Point2f> points, float width, PresetType presetType);
		~SplinePreset(void);

		unsigned int getId(void) const;
		PresetType getPresetType(void) const;
		const std::vector<const Point2f> &getPoints(void) const;
		float getWidth(void) const;
		void save(void) const;
		void setPoints(std::vector<const Point2f> points);

	private:
		static const char *DEFAULT_PATH;
		static const char *PATH;
		static const char *XML_CONTROL_POINT;
		static const char *XML_ID;
		static const char *XML_PRESET_TYPE;
		static const char *XML_SPLINE_PRESET;
		static const char *XML_WIDTH;
		static const char *XML_X;
		static const char *XML_Y;

		unsigned int id;
		PresetType presetType;
		std::vector<const Point2f> points;
		float width;

	};
}
#endif

