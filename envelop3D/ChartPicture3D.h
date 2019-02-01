#pragma  once

#include <osg/Vec4d>
#include <osg/ref_ptr>
#include <osg/Group>
#include <QVector3D>
#include <osg/Geometry>
#include <osg/MatrixTransform>

namespace ChatDirection
{
	typedef struct{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	}RGB, *PRGB;
}
struct PosPose
{//Ŀ��λ����̬
	double      dLon;
	double      dLat;
	double      dAlt;
	double      dAzimuth;// ��λ
	double      dPitch;	// ����
	double      dRoll;	// ��ת��
};

class ChatPicture3D
{
public:
	ChatPicture3D();
	~ChatPicture3D();
	//root node
	osg::ref_ptr<osg::MatrixTransform> getNodeMatrix(){ return m_nodeMatrix; }

	//��ͼ ���ݸ�ʽ [��λ...]-����-[����...] 
	void  DrawDirectionOfChart(const int envelopID, const PosPose& objPosPose,
		const std::vector<QVector3D>& points3D);

	//����λ����̬
	void setChartPosPose(const PosPose& objPosPose);

	int GetEquipID() { return m_chartID; }

	//�����ʾ����
	void  ClearChart();

	//���ýڵ��Ƿ���ʾ
	void SetVisible(const bool show);

	//��ȡ��ʾ�����Ƿ���ʾ
	bool  GetShow() { return m_Show; }

	//�Ƿ�Ŵ���ʾ
	void setAmplificationDisplay(bool bAmplification);

	//ͬ��2D�ǶȽ���
	void setCorrect(bool bCorrect);
private:

	void initGeometry();

	void initEnvelopMatrix();

	osg::Vec3d computerVHPosition(double vdegree, double hdegree, double r);

	ChatDirection::RGB GetGridColor_gradient(double dcurrent_value, 
		double dthreshold_value, double dmax_value, unsigned int alpha);

	void ColorRing(unsigned int count, ChatDirection::PRGB rgb, unsigned int color_intensity);

	void Coe2Color(double coe, ChatDirection::PRGB rgb, 
		int color_intensity, int color_start, int color_end);

	osg::ref_ptr<osg::MatrixTransform> m_nodeMatrix;
	osg::ref_ptr<osg::Group>  m_ChartGroup;
	osg::ref_ptr<osg::MatrixTransform> m_chartMatrix;
	osg::ref_ptr<osg::Geometry> m_geometry;
	bool m_Show;
	int m_chartID;
	bool m_bAmplification;
	bool m_bCorrect;
	float  m_scaleSize;
	float m_aziCorrect;
	float m_pitchCorrect;
};