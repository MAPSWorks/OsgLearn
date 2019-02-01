#pragma once
#include <osgEarth/MapNode>
#include <osgEarth/ElevationQuery>
#include <osgViewer/viewer>   
#include <osgEarthUtil/Controls>

//��ѯ�̵߳��¼��ص�
class QueryElevationHandler : public osgGA::GUIEventHandler
{
public:
	QueryElevationHandler(osgEarth::MapNode* mapNode, osg::ref_ptr<osg::Group> pRoot);
protected:
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	void displayPosLabel(osg::ref_ptr<osg::Group> pRoot);
private:
	osgEarth::MapNode* _mapNode;
	//���ζ���
	const osgEarth::Terrain*   _terrain;
	bool             _mouseDown;
	//��ѯ�߳�ʹ�õĶ���
	osgEarth::ElevationQuery   _query;
	osg::NodePath    _path;
	osgEarth::Util::Controls::LabelControl*  m_mouseCoords;
};