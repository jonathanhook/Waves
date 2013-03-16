/**
 * Class:	SplinePreset
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include <sstream>
#include "JDHUtility/CrossPlatformTime.h"
#include "JDHUtility/FileAccess.h"
#include "SplinePreset.h"
#include "xmlParser\xmlParser.h"

namespace Waves
{
	/* PRIVATE CONSTANTS */
	const char *SplinePreset::DEFAULT_PATH = "..\\SavedState\\SplinePresets\\Default\\";
	const char *SplinePreset::PATH = "..\\SavedState\\SplinePresets\\";
	const char *SplinePreset::XML_CONTROL_POINT = "ControlPoint";
	const char *SplinePreset::XML_ID = "Id";
	const char *SplinePreset::XML_PRESET_TYPE = "PresetType";
	const char *SplinePreset::XML_SPLINE_PRESET = "SplinePreset";
	const char *SplinePreset::XML_WIDTH = "Width";
	const char *SplinePreset::XML_X = "X";
	const char *SplinePreset::XML_Y = "Y";

	/* PUBLIC STATIC FUNCTIONS */
	SplinePreset *SplinePreset::load(std::string file)
	{
		XMLNode doc = XMLNode::openFileHelper(file.c_str(), XML_SPLINE_PRESET);

		unsigned int id = atoi(doc.getAttribute(XML_ID));
		float width = (float)atof(doc.getAttribute(XML_WIDTH));
		PresetType presetType = (PresetType)atoi(doc.getAttribute(XML_PRESET_TYPE));

		std::vector<const Point2f> points;	
		int numPoints = doc.nChildNode(XML_CONTROL_POINT);
		for(int i = 0; i < numPoints; i++)
		{
			XMLNode node = doc.getChildNode(XML_CONTROL_POINT, i);
			float x = (float)atof(node.getAttribute(XML_X));
			float y = (float)atof(node.getAttribute(XML_Y));
			points.push_back(Point2f(x, y));
		}

		return new SplinePreset(id, points, width, presetType);
	}

	std::map<unsigned int, SplinePreset *> SplinePreset::loadAll(void)
	{
		std::map<unsigned int, SplinePreset *> presets;

		char c[1024];

		sprintf(c, "%s*.xml", PATH);
		std::vector<std::string> files = FileAccess::getFiles(c);

		for(unsigned int i = 0; i < files.size(); i++)
		{
			sprintf(c, "%s%s", PATH, files[i].c_str());
			SplinePreset *p = load(c);
			assert(p != NULL);

			presets[p->getId()] = p;
		}

		return presets;
	}

	std::map<unsigned int, SplinePreset *> SplinePreset::loadDefaults(void)
	{
		std::map<unsigned int, SplinePreset *> presets;

		char c[1024];

		sprintf(c, "%s*.xml", DEFAULT_PATH);
		std::vector<std::string> files = FileAccess::getFiles(c);

		for(unsigned int i = 0; i < files.size(); i++)
		{
			sprintf(c, "%s%s", DEFAULT_PATH, files[i].c_str());
			SplinePreset *p = load(c);
			assert(p != NULL);

			presets[p->getId()] = p;
		}

		return presets;
	}

	unsigned int SplinePreset::generateId(void)
	{
		static unsigned int seed = 0;
		unsigned int now = CrossPlatformTime::getTimeMillis();
		return (now + seed++);
	}

	/* CONSTRUCTORS */
	SplinePreset::SplinePreset(unsigned int id, std::vector<const Point2f> points, float width, PresetType presetType)
	{
		this->points = points;
		this->width = width;
		this->presetType = presetType;
		this->id = id;
	}

	SplinePreset::~SplinePreset(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	unsigned int SplinePreset::getId(void) const
	{
		return id;
	}

	SplinePreset::PresetType SplinePreset::getPresetType(void) const
	{
		return presetType;
	}

	const std::vector<const Point2f> &SplinePreset::getPoints(void) const
	{
		return points;
	}
	
	float SplinePreset::getWidth(void) const
	{
		return width;
	}

	void SplinePreset::save(void) const
	{
		char c[1024];

		XMLNode doc = XMLNode::createXMLTopNode(XML_SPLINE_PRESET);
		sprintf(c, "%d", id);
		doc.addAttribute(XML_ID, c);
		
		sprintf(c, "%f", width);
		doc.addAttribute(XML_WIDTH, c);

		sprintf(c, "%d", presetType);
		doc.addAttribute(XML_PRESET_TYPE, c);

		for(unsigned int i = 0; i < points.size(); i++)
		{
			XMLNode node = XMLNode::createXMLTopNode(XML_CONTROL_POINT);
			
			sprintf(c, "%f", points[i].getX());
			node.addAttribute(XML_X, c);

			sprintf(c, "%f", points[i].getY());
			node.addAttribute(XML_Y, c);

			doc.addChild(node);
		}

		sprintf(c, "%s\\%d.xml", PATH, id);
		XMLError e = doc.writeToFile(c);
		
	}

	void SplinePreset::setPoints(std::vector<const Point2f> points)
	{
		this->points = points;
	}
}
