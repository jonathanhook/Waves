/**
 * Class:	VisualWavesControl
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <map>
#include <string>
#include <vector>
#include "FastDelegate.h"
#include "JDHUtility/FileAccess.h"
#include "InterfaceManager.h"
#include "MainControlPanel.h"
#include "RadarSegment.h"
#include "TextScrollMenu.h"
#include "ToggleButton.h"
#include "Track.h"
#include "VisualWavesControl.h"
#include "WavesGlobalState.h"
#include "xmlParser\xmlParser.h"

using namespace fastdelegate;
using namespace JDHUtility;
using namespace WavesVisuals;

namespace Waves
{
	/* PRIVATE CONSTANTS */
	const float VisualWavesControl::ITEM_HEIGHT = 0.1f;
	const char *VisualWavesControl::PATH = "..\\SavedState\\WavesControls\\";
	const char *VisualWavesControl::XML_ID = "Id";
	const char *VisualWavesControl::XML_FFT_PRESET = "FFTPreset";
	const char *VisualWavesControl::XML_FOR_OUTPUT = "ForOutput";
	const char *VisualWavesControl::XML_HEIGHT = "Height";
	const char *VisualWavesControl::XML_MODE = "Mode";
	const char *VisualWavesControl::XML_NICKNAME = "Nickname";
	const char *VisualWavesControl::XML_PARAMETER = "Parameter";
	const char *VisualWavesControl::XML_POSITION = "Position";
	const char *VisualWavesControl::XML_TEMPORAL_PRESET = "TemporalPreset";
	const char *VisualWavesControl::XML_TRACK = "Track";
	const char *VisualWavesControl::XML_VISUAL = "Visual";
	const char *VisualWavesControl::XML_WAVES_CONTROL = "WavesControl";
	const char *VisualWavesControl::XML_WIDTH = "Width";
	const char *VisualWavesControl::XML_X = "X";
	const char *VisualWavesControl::XML_Y = "Y";

	/* PUBLIC STATIC FUNCTIONS */
	VisualWavesControl *VisualWavesControl::load(std::string file)
	{
		XMLNode doc = XMLNode::openFileHelper(file.c_str(), XML_WAVES_CONTROL);

		float width = atof(doc.getAttribute(XML_WIDTH));
		std::string visualName = doc.getAttribute(XML_VISUAL);
		
		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs != NULL);

		const std::vector<Visual *> factoryVisuals = wgs->getFactoryVisuals();
		
		Visual *visual = NULL;
		for(unsigned int i = 0; i < factoryVisuals.size(); i++)
		{
			Visual *cmp = factoryVisuals[i];
			assert(cmp != NULL);

			if(cmp->getName() == visualName)
			{
				visual = cmp->getInstance();
				break;
			}
		}

		if(visual == NULL)
		{
			return NULL;
		}
		else
		{
			assert(visual != NULL);

			XMLNode positionNode = doc.getChildNode(XML_POSITION);
			float x = atof(positionNode.getAttribute(XML_X));
			float y = atof(positionNode.getAttribute(XML_Y));
			Point2f position(x, y);

			std::string nickname = doc.getAttribute(XML_NICKNAME);

			bool forOutput = (bool)atoi(doc.getAttribute(XML_FOR_OUTPUT));

			// load tracks
			std::vector<Track *> tracks;
			unsigned int trackCount = doc.nChildNode(XML_TRACK);
			for(unsigned int i = 0; i < trackCount; i++)
			{
				XMLNode trackNode = doc.getChildNode(XML_TRACK, i);

				XMLNode trackPositionNode = trackNode.getChildNode(XML_POSITION);
				float tx = atof(trackPositionNode.getAttribute(XML_X));
				float ty = atof(trackPositionNode.getAttribute(XML_Y));
				Point2f position(tx, ty);

				float width = atof(trackNode.getAttribute(XML_WIDTH));
				float height = atof(trackNode.getAttribute(XML_HEIGHT));

				std::string parameterName = trackNode.getAttribute(XML_PARAMETER);
				Parameter *parameter = visual->getParameter(parameterName);

				Track *track = new Track(position, width, height, parameter);	
				std::map<unsigned int, SplinePreset *> &splinePresets = wgs->getSplinePresets();

				Track::TrackMode mode = (Track::TrackMode)atoi(trackNode.getAttribute(XML_MODE));
				track->setMode(mode);

				// load presets for menu
				unsigned int numFFTPresets = trackNode.nChildNode(XML_FFT_PRESET);
				for(unsigned int j = 0; j < numFFTPresets; j++)
				{
					XMLNode fftPresetNode = trackNode.getChildNode(XML_FFT_PRESET, j);
					unsigned int pId = atoi(fftPresetNode.getAttribute(XML_ID));
					track->addFFTPreset(pId);
				}

				unsigned int numtemporalPresets = trackNode.nChildNode(XML_TEMPORAL_PRESET);
				for(unsigned int j = 0; j < numtemporalPresets; j++)
				{
					XMLNode temporalPresetNode = trackNode.getChildNode(XML_TEMPORAL_PRESET, j);
					unsigned int pId = atoi(temporalPresetNode.getAttribute(XML_ID));
					track->addTemporalPreset(pId);
				}

				// load current presets (if any)
				std::map<unsigned int, SplinePreset *> &loadedPresets = wgs->getSplinePresets();
				std::map<unsigned int, SplinePreset *> &defaultPresets = wgs->getDefaultPresets();

				bool hasTemporalCurrentPreset = (bool)trackNode.isAttributeSet(XML_TEMPORAL_PRESET);
				if(hasTemporalCurrentPreset)
				{
					unsigned int temporalCurrentPreset = atoi(trackNode.getAttribute(XML_TEMPORAL_PRESET));
					
					SplinePreset *sp = NULL;
					
					if(loadedPresets.count(temporalCurrentPreset) > 0)
					{
						sp = loadedPresets[temporalCurrentPreset];
					}
					else if(defaultPresets.count(temporalCurrentPreset) > 0)
					{
						sp = defaultPresets[temporalCurrentPreset];
					}

					if(sp != NULL)
					{
						TemporalSplineControl &temporalSplineControl = track->getTemporalSplineControl();
						temporalSplineControl.loadPreset(sp);
					}
				}

				bool hasFFTCurrentPreset = (bool)trackNode.isAttributeSet(XML_FFT_PRESET);
				if(hasFFTCurrentPreset)
				{
					unsigned int FFTCurrentPreset = atoi(trackNode.getAttribute(XML_FFT_PRESET));
					
					SplinePreset *sp = NULL;
					
					if(loadedPresets.count(FFTCurrentPreset) > 0)
					{
						sp = loadedPresets[FFTCurrentPreset];
					}
					else if(defaultPresets.count(FFTCurrentPreset) > 0)
					{
						sp = defaultPresets[FFTCurrentPreset];
					}

					if(sp != NULL)
					{
						FFTSplineControl &FFTSplineControl = track->getFFTSplineControl();
						FFTSplineControl.loadPreset(sp);
					}
				}

				tracks.push_back(track);
			}

			unsigned int loadedId = atoi(doc.getAttribute(XML_ID));

			VisualWavesControl *v = new VisualWavesControl(position, width, visual, tracks, nickname, forOutput);
			v->setEnabled(false);
			v->setId(loadedId);
			return v; 
		}
	}

	std::map<unsigned int, VisualWavesControl *> VisualWavesControl::loadAll(void)
	{
		std::map<unsigned int, VisualWavesControl *> loadedControls;
		
		char c[1024];
		sprintf(c, "%s*.xml", PATH);
		std::vector<std::string> files = FileAccess::getFiles(c);

		for(unsigned int i = 0; i < files.size(); i++)
		{
			sprintf(c, "%s%s", PATH, files[i].c_str());
			VisualWavesControl *v = load(c);
			assert(v != NULL);

			unsigned int vId = v->getId(); 
			loadedControls[vId] = v;
		}

		return loadedControls;
	}

	/* CONSTRUCTORS */
	VisualWavesControl::VisualWavesControl(const Point2f &position, float width, Visual *visual, std::vector<Track *> tracks, std::string nickname, bool forOutput) :
		WavesControl(position, width, visual->getName(), tracks)
	{
		this->visual = visual;
		this->nickname = nickname;
		this->forOutput = forOutput;

		initInterfaceElements();
	}

	VisualWavesControl::~VisualWavesControl(void)
	{
		if(visual != NULL) delete visual;
		visual = NULL;
	}
	
	/* PUBLIC MEMBER FUNCTIONS */
	const std::string &VisualWavesControl::getNickname(void) const
	{
		return nickname;
	}

	Visual *VisualWavesControl::getVisual(void) const
	{
		return visual;
	}

	bool VisualWavesControl::isForOutput(void) const
	{
		return forOutput;
	}

	void VisualWavesControl::renderVisual(void)
	{
		assert(visual != NULL);

		visual->render();
	}

	void VisualWavesControl::save(void)
	{
		assert(visual != NULL);

		char c[1024];

		XMLNode doc = XMLNode::createXMLTopNode(XML_WAVES_CONTROL);
		
		sprintf(c, "%d", id);
		doc.addAttribute(XML_ID, c);

		std::string visualName = visual->getName();
		sprintf(c, "%s", visualName.c_str());
		doc.addAttribute(XML_VISUAL, c);	
		
		sprintf(c, "%f", width);
		doc.addAttribute(XML_WIDTH, c);

		sprintf(c, "%s", nickname.c_str());
		doc.addAttribute(XML_NICKNAME, c);

		sprintf(c, "%d", forOutput);
		doc.addAttribute(XML_FOR_OUTPUT, c);

		// position
		XMLNode positionNode = XMLNode::createXMLTopNode(XML_POSITION);	
		
		float px = position.getX();
		sprintf(c, "%f", px);
		positionNode.addAttribute(XML_X, c);
		
		float py = position.getY();
		sprintf(c, "%f", py);
		positionNode.addAttribute(XML_Y, c);
		doc.addChild(positionNode);

		// tracks
		for(unsigned int i = 0; i < tracks.size(); i++)
		{
			XMLNode trackNode = XMLNode::createXMLTopNode(XML_TRACK);

			Track *track = tracks[i];
			assert(track != NULL);
			
			// position
			XMLNode trackPositionNode = XMLNode::createXMLTopNode(XML_POSITION);
			
			const Point2f &trackPosition = track->getPosition();
			float tx = trackPosition.getX();
			sprintf(c, "%f", tx);
			trackPositionNode.addAttribute(XML_X, c);
			
			float ty = trackPosition.getY();
			sprintf(c, "%f", ty);
			trackPositionNode.addAttribute(XML_Y, c);
			trackNode.addChild(trackPositionNode);

			Track::TrackMode mode = track->getMode();
			sprintf(c, "%d", mode);
			trackNode.addAttribute(XML_MODE, c);

			float trackWidth = track->getWidth();
			sprintf(c, "%f", trackWidth);
			trackNode.addAttribute(XML_WIDTH, c);

			float trackHeight = track->getHeight();
			sprintf(c, "%f", trackHeight);
			trackNode.addAttribute(XML_HEIGHT, c);
			
			const  Parameter &parameter = track->getParameter();
			std::string parameterName = parameter.getName();
			sprintf(c, "%s", parameterName.c_str());
			trackNode.addAttribute(XML_PARAMETER, c);

			// current presets
			FFTSplineControl &fftSplineControl = track->getFFTSplineControl();
			if(fftSplineControl.isPresetSaved())
			{
				unsigned int currentFFTPresetId = fftSplineControl.getPresetId();
				sprintf(c, "%d", currentFFTPresetId);
				trackNode.addAttribute(XML_FFT_PRESET, c);
			}

			TemporalSplineControl &temporalSplineControl = track->getTemporalSplineControl();
			if(temporalSplineControl.isPresetSaved())
			{
				unsigned int currenttemporalPresetId = temporalSplineControl.getPresetId();
				sprintf(c, "%d", currenttemporalPresetId);
				trackNode.addAttribute(XML_TEMPORAL_PRESET, c);
			}

			std::vector<unsigned int> fftPresets = track->getFFTPresets();
			for(unsigned int j = 0; j < fftPresets.size(); j++)
			{
				XMLNode fftPresetNode = XMLNode::createXMLTopNode(XML_FFT_PRESET);

				unsigned int fftPId = fftPresets[j];
				sprintf(c, "%d", fftPId);
				fftPresetNode.addAttribute(XML_ID, c);

				trackNode.addChild(fftPresetNode);
			}

			std::vector<unsigned int> temporalPresets = track->getTemporalPresets();
			for(unsigned int j = 0; j < temporalPresets.size(); j++)
			{
				XMLNode temporalPresetNode = XMLNode::createXMLTopNode(XML_TEMPORAL_PRESET);

				unsigned int temporalPId = temporalPresets[j];
				sprintf(c, "%d", temporalPId);
				temporalPresetNode.addAttribute(XML_ID, c);

				trackNode.addChild(temporalPresetNode);
			}

			doc.addChild(trackNode);
		}

		// TODO : better id generation for all interface elements to prevent clashes
		sprintf(c, "%s\\%d.xml", PATH, id);
		XMLError e = doc.writeToFile(c);

		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs != NULL);

		wgs->addSavedControl(this);
	}

	void VisualWavesControl::setClosedCallback(ClosedCallback closed)
	{
		this->closed = closed;
	}

	void VisualWavesControl::setSavedCallback(SavedCallback saved)
	{
		this->saved = saved;
	}

	void VisualWavesControl::update(void)
	{
		assert(visual != NULL);
		
		WavesControl::update();
		
		if(onRadar)
		{
			visual->update();
		}
	}

	/* PROTECTED MEMBER FUNCTIONS */
	void VisualWavesControl::addToggleButton_ToggleChanged(bool toggled)
	{
		assert(visual != NULL);

		if(toggled)
		{
			std::vector<std::string> menuItems;
			const std::map<std::string, Parameter *> parameters = visual->getParameters();
		
			for(std::map<std::string, Parameter *>::const_iterator it = parameters.cbegin(); it != parameters.cend(); it++)
			{
				Parameter *p = (*it).second;
				assert(p != NULL);

				if(!p->isDisabled())
				{
					menuItems.push_back(p->getName());
				}
			}

			showMenu(menuItems);
		}
		else
		{
			hideMenu();
		}
	}

	void VisualWavesControl::controlPanel_Closed(void)
	{
		if(closed != NULL)
		{
			closed(this);
		}
	}

	void VisualWavesControl::controlPanel_CueChanged(bool selected)
	{
		forOutput = selected;
	}

	void VisualWavesControl::controlPanel_Saved(void)
	{
		// save all of the tracks' presets
		for(unsigned int i = 0; i < tracks.size(); i++)
		{
			Track *t = tracks[i];
			assert(t != NULL);
			
			FFTSplineControl &fftS = t->getFFTSplineControl();
			fftS.savePreset();
			unsigned int pId = fftS.getPresetId();
			t->addFFTPreset(pId);

			TemporalSplineControl &tempS = t->getTemporalSplineControl();
			tempS.savePreset();		
			pId = tempS.getPresetId();
			t->addTemporalPreset(pId);
		}	

		save();

		if(saved != NULL)
		{
			saved(this);
		}
	}
	
	void VisualWavesControl::initInterfaceElements(void)
	{
		WavesControl::initInterfaceElements();

		// main control panel
		MainControlPanel *controlPanel = new MainControlPanel(Point2f((BAR_DIM * 2.0f) + BORDER_DIM, 0.0f), width + (InterfaceManager::getTrackHeight() * 0.75f) + (BORDER_DIM * 2.0f) + BAR_DIM, InterfaceManager::getTrackHeight(), visual, forOutput);
		controlPanel->setClosedCallback(MakeDelegate(this, &VisualWavesControl::controlPanel_Closed));
		controlPanel->setCueChangedCallback(MakeDelegate(this, &VisualWavesControl::controlPanel_CueChanged));
		controlPanel->setSaveCallback(MakeDelegate(this, &VisualWavesControl::controlPanel_Saved));
		registerInterfaceElement(controlPanel);
	}

	void VisualWavesControl::menu_ItemSelected(std::string item)
	{
		assert(visual != NULL);

		hideMenu();
		addTrack(visual->getParameter(item));
	}

	void VisualWavesControl::updateParameterValues(void)
	{
		assert(visual != NULL);

		// for each track set all parameters to their defaults (hacky!)
		std::map<std::string, Parameter *> vParams = visual->getParameters();
		std::map<std::string, Parameter *>::iterator pIt;
		for(pIt = vParams.begin(); pIt != vParams.end(); pIt++)
		{
			std::string name = (*pIt).first;
			Parameter *p = (*pIt).second;
			assert(p != NULL);

			float defaultVal = p->getDefaultValue();
			visual->setParameterValue(name, defaultVal);
		}

		// for each track, get the new value and update the visual
		for(std::vector<Track *>::iterator it = tracks.begin(); it != tracks.end(); it++)
		{
			Track *t = (*it);
			assert(t != NULL);

			const Parameter &p = t->getParameter();
			std::string name = p.getName();
			float value = t->getValue();
			
			// BUG COULD BE THAT SOME CODE IS RETURNING NAN (note, this only happens for luma? / only crashes for luma)
			assert(value >= 0.0f && value <= 1.0f);

			visual->setParameterValue(name, value);
		}
	}
}
