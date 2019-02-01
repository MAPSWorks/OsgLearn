#include "QEarthManager.h"

#include <osgEarth/ElevationQuery>
#include <osgEarthUtil/Controls>
#include <osg/Multisample>

#include "QEarthMath.h"
#include "QueryElevationHandler.h"

using namespace osgEarth;
using namespace osgEarth::Util;
using namespace osgEarth::Util::Controls;

QEarthManager::QEarthManager():
m_mapNode(NULL),
m_pRoot(NULL)
{
}


QEarthManager::~QEarthManager()
{
	m_manipulator.release();
	m_skyNode.release();
	m_mapNode.release();
	m_pRoot.release();
}

QEarthManager* QEarthManager::instance()
{
	static QEarthManager e;
	return &e;
}

void QEarthManager::convertObjMatrix(const double dLon, const double dLat,
	const double dAlt, const double dAzimuth,
	const double dPitch, const double dRoll, osg::MatrixTransform* matrixNode)
{
	if (!m_mapNode)
	{
		OE_FATAL << "earth map node is invalid." << std::endl;
		return ;
	}
	//�߶ȸ�ֵ��̨վ��б
	osg::Matrixd geoToWorldMat;
	/**1.��������ϵ�任����������ϵ*/
	QEarthMath::GEOCoordFrameToWorldCoordFrame(dLon, dLat,
		dAlt, 0.0, 0.0, 0.0, dAzimuth, dPitch, dRoll,
		geoToWorldMat, *m_mapNode->getMapSRS());
	/**2.��������ϵ�µ�ƫ��*/
	geoToWorldMat.preMultTranslate(osg::Vec3d(0, 0, 0));

	matrixNode->setMatrix(geoToWorldMat);
}


void QEarthManager::worldPointToMapPoint(const double x, const double y, const double z, double& lon, double& lat, double& alt)
{
	osg::Vec3d input(x, y, z);
	osg::Vec3d output(0.0f, 0.0f, 0.0f);
	m_mapNode->getMapSRS()->transformFromWorld(input, output);
	lon = output.x();
	lat = output.y();
	alt = output.z();
}

void QEarthManager::mapPointToWorldPoint(const double lon, const double lat, const double alt, double& x, double& y, double& z)
{
	osg::Vec3d input(lon, lat, alt);
	osg::Vec3d output(0.0f, 0.0f, 0.0f);
	m_mapNode->getMapSRS()->transformToWorld(input, output);
	x = output.x();
	y = output.y();
	z = output.z();
}

double QEarthManager::getElevation(const double dLon, const double dLat)
{
	//  �����߳̾��ȡ���λ����
	double query_resolution = 0.00001f;
	// �õ��ĸ߳�ֵ
	double out_elevation = 0.0f;
	// ʵ�ʵĸ߳̾��ȡ���λ����
	double out_resolution = 0.0f;
	osgEarth::ElevationQuery query(m_mapNode->getMap());
	// convert to map coords:
	GeoPoint mapPoint(m_mapNode->getMapSRS(), dLon, dLat);
	bool bOk = query.getElevation(mapPoint,
		out_elevation,
		query_resolution,
		&out_resolution);

	return bOk ? out_elevation : -10000.0f;
}

bool QEarthManager::initEarthView(const QString& earthFile, osgViewer::Viewer* pViewer)
{
	if (!pViewer) return false;
	// �����ǽڵ�
	osg::Node* mapNode = osgDB::readNodeFile(earthFile.toStdString());
	//����MapNode
	m_mapNode = osgEarth::MapNode::findMapNode(mapNode);
	if (!m_mapNode)
	{
		OE_WARN << "Unable to load earth file." << std::endl;
		return false;
	}
	//����һ�����ڵ�
	m_pRoot = new osg::Group();
	//��MapNode��ӵ���ڵ���ȥ
	m_pRoot->addChild(m_mapNode);

	//ʵ�ֿ����
	osg::Multisample *pms = new osg::Multisample;
	pms->setSampleCoverage(GL_MULTISAMPLE_ARB, true);
	m_pRoot->getOrCreateStateSet()->setAttributeAndModes(pms, osg::StateAttribute::ON);

	//����ڵ�����Ϊ�����ڵ�
	pViewer->setSceneData(m_pRoot);

	addEarthManipulator(pViewer);

	addSkyNode(pViewer);

	//����Զ���߳���ʾ
	pViewer->addEventHandler(new QueryElevationHandler(m_mapNode, m_pRoot));
	return true;
}


void QEarthManager::addEarthManipulator(osgViewer::Viewer* pViewer)
{
	//����earth������
	m_manipulator = new osgEarth::Util::EarthManipulator();
	m_manipulator->getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_IN, osgGA::GUIEventAdapter::SCROLL_UP);
	m_manipulator->getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_OUT, osgGA::GUIEventAdapter::SCROLL_DOWN);
	m_manipulator->getSettings()->setArcViewpointTransitions(true);
	m_manipulator->getSettings()->setTetherMode(osgEarth::Util::EarthManipulator::TETHER_CENTER);
	pViewer->setCameraManipulator(m_manipulator.get());
	m_manipulator->setViewpoint(osgEarth::Viewpoint(QString("Ĭ��λ��").toLocal8Bit().data(),
		115.971, 30.85, 444.02, -1.84, -70.01, 8028182.75), 0.0);
}

void QEarthManager::addSkyNode(osgViewer::Viewer* pViewer)
{
	/**��ʼ�����*/
	m_skyNode = SkyNode::create(m_mapNode);
	if (m_skyNode)
	{
		DateTime dTime;//�Զ�ʱ��
		m_skyNode->setDateTime(dTime);
		m_skyNode->attach(pViewer);
		m_skyNode->setSunVisible(true);
		m_skyNode->setMoonVisible(true);
		m_skyNode->setStarsVisible(true);
		m_skyNode->setLighting(osg::StateAttribute::OFF);

		osgEarth::insertGroup(m_skyNode, m_mapNode->getParent(0));
	}
}


osg::ref_ptr<osg::Group> QEarthManager::createLight(osg::ref_ptr<osg::Node> node)
{
	// �򳡾�����ӹ�Դ
	osg::ref_ptr<osg::Group> lightRoot = new osg::Group();
	lightRoot->addChild(node);

	//��������
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset = lightRoot->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	stateset->setMode(GL_LIGHT0, osg::StateAttribute::ON);

	//�����Χ��
	osg::BoundingSphere bs;
	node->computeBound();
	bs = node->getBound();

	//����һ��Light����
	osg::ref_ptr<osg::Light> light = new osg::Light();
	light->setLightNum(0);
	//���÷���
	light->setDirection(osg::Vec3(0.0f, 0.0f, -1.0f));
	//����λ��	
	light->setPosition(osg::Vec4(bs.center().x(), bs.center().y(), bs.center().z() + bs.radius(), 1.0f));
	//���û��������ɫ
	light->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//����ɢ������ɫ
	light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//���ú�˥��ָ��
	light->setConstantAttenuation(1.0f);
	//��������˥��ָ��
	light->setLinearAttenuation(0.0f);
	//���ö��η�˥��ָ��
	light->setQuadraticAttenuation(0.0f);

	//������Դ
	osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource();
	lightSource->setLight(light.get());

	lightRoot->addChild(lightSource.get());

	return lightRoot.get();
}

