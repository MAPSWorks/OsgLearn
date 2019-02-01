#ifndef KUMAPMATH_3D_H
#define KUMAPMATH_3D_H


#include <osg/Quat>
#include <osg/Matrixd>
#include <osgEarth/SpatialReference>

class QEarthMath
{
public:
/**
     * ������������.�����������������,��������������.
     * ���������������ת���Ӳ��ֵ�3x3�Ӿ����������������,������ת���ɵľ���.
     * ������ֻ�������ת���ֵ���.����ƽ�Ʋ���.
     */
static osg::Matrixd  OrthMatrixInvert_3x3( const osg::Matrixd& mat );
/**
     * �ú����������ת���ֵ���֮�⣬�������ƽ�Ʋ��ֵ���
    */
static osg::Matrixd  OrthMatrixInvert_4x3(const osg::Matrixd& mat);
/**
     * ��ת��Ԫ��ת����ŷ����
     * Լ��������Z����ת�ĽǶ�Ϊazimuth_deg����Y����תΪpitch_deg����x����תΪroll_deg
     * ��Ԫ��ת����ŷ����ʱ����ת·����Ψһ.Լ����ת·��Ϊz��-y��-x��
     * quat:��ת��Ԫ��
     * azimuth_rad-pitch_rad-roll_rad:��λ��-������-�����
     */
static void  QuatToEulerAngle(const osg::Quat& quat, double& azimuth_deg, double& pitch_deg, double& roll_deg);
static void  GetLonLatHByLonLatAziPitch(double lon0, double lat0, double h0, double azi,
                                double pitch,double dis,double& outLon,double& outLat,double& outH,osgEarth::SpatialReference* srs = NULL);
/**
     * �����ڵ�������ϵ�µ�λ����̬ת�������������µ�λ����̬����
     * Լ��������Z����ת�ĽǶ�Ϊazimuth_deg����Y����תΪpitch_deg����x����תΪroll_deg
     * lon_deg-lat_deg-alt.����-γ��-����.��λ:�Ƕ�-��
     * x_offset,y_offset,z_offset.����ھ�γ���ƫ��.��λ:��
     * azimuth_rad-pitch_rad-roll_rad:��������ϵ�µķ�λ��-������-�����
     * worldCoordFrame:��������������ϵ�µ�λ����̬����
     * SpatialReference:�ռ�����ϵͳ
     */
static void  GEOCoordFrameToWorldCoordFrame(const double lon_deg, const double lat_deg, const double alt,
                                     const double x_offset, const double y_offset, const double z_offset,
                                     const double azimuth_deg, const double pitch_deg, const double roll_deg,
                                     osg::Matrixd& worldCoordFrame, const osgEarth::SpatialReference& mapSRS );

/**
     * �����ڵ�������ϵ�µ�λ����̬ת�������������µ�λ����̬����
     * Լ��������Z����ת�ĽǶ�Ϊazimuth_deg����Y����תΪpitch_deg����x����תΪroll_deg
     * lon_deg-lat_deg-alt.����-γ��-����.��λ:�Ƕ�-��
     * x_offset,y_offset,z_offset.����ھ�γ���ƫ��.��λ:��
     * x_self_offset,y_self_offset,z_self_offset.�������������ϵ��ƫ��(��������ʱ���)
     * azimuth_rad-pitch_rad-roll_rad:��������ϵ�µķ�λ��-������-�����
     * worldCoordFrame:��������������ϵ�µ�λ����̬����
     * SpatialReference:�ռ�����ϵͳ
     */
static void  GEOCoordFrameToWorldCoordFrame(const double lon_deg, const double lat_deg, const double alt,
                                    const double x_offset,const double y_offest,const double z_offset,
                                    const double x_self_offset,const double y_self_offset,const double z_self_offset,
                                    const double azimuth_deg,const double pitch_deg,const double roll_deg,
                                    osg::Matrixd& worldCoordFrame,const osgEarth::SpatialReference& mapSRS );

static osg::Vec3d  GEOCoordFrameToWorldCoorFrame(const double lon_deg, const double lat_deg, const double alt,
                                         const double x_offset,const double y_offest,const double z_offset,
                                         const double x_self_offset,const double y_self_offset,const double z_self_offset,
                                         const double azimuth_deg,const double pitch_deg,const double roll_deg,
                                         osgEarth::SpatialReference* mapSRS = NULL);
/**
    * ���������������µ�λ����̬ת���������ڵ�������ϵ�µ�λ����̬
    * Լ��������Z����ת�ĽǶ�Ϊazimuth_deg����Y����תΪpitch_deg����x����תΪroll_deg
    * lon_deg-lat_deg-alt.����-γ��-����.��λ:�Ƕ�-��
    * azimuth_rad-pitch_rad-roll_rad:��λ��-������-�����
    * azimuth_rad-pitch_rad-roll_rad:��������ϵ�µķ�λ��-������-�����
    * worldCoordFrame:��������������ϵ�µ�λ����̬����
    * SpatialReference:�ռ�����ϵͳ
    */
static void  WorldCoordFrameToGEOCoordFrame(double& lon_deg, double& lat_deg, double& alt,
                                     double& azimuth_deg, double& pitch_deg, double& roll_deg,
                                     const osg::Matrixd& worldCoordFrame, const osgEarth::SpatialReference& mapSRS );

static void  GetRelativePositon(double StartLon, double StartLat, double StartH,
                        double TargetLon,double TargetLat,double TargetH,
                        double &OutX,double &OutY,double &OutZ,
                        osgEarth::SpatialReference* srs = NULL);

};
#endif // KUMAPMATH_3D_H
