#pragma  once

#include <vector>
#include <QVector3D>
#include "ChartPicture3D.h"
#include <osg\Group>

class ChartPicture3DManager
{
public:
	ChartPicture3DManager();
	~ChartPicture3DManager();

	//���� ��ά����ͼ 
	//���ݸ�ʽ [��λ...]-����-[����...] 
	void createEnvelop3D(const int envelopID, const PosPose& objPosPose,
		const  std::vector<QVector3D>& Points, 
		bool bAmplification = false, bool bCorrect = true);

	//����
	bool setVisibleById(const int& envelopID, bool bShow);
	//��ȡָ��ID����״̬ -1: δ�ҵ�; 1����ʾ; 0:����
	int getStatus(const int& equipID);
	//��ʾ����
	void setAllVisible(bool bShow);
	//�����ʾ����
	void DelChartDisplayByID(const int& envelopID);
	//�ӽڵ���ɾ������
	void DelAllChartPicture();
	//����λ�ýǶ�
	bool updateChartPosPose(const int& envelopID, const PosPose& objPosPose);
	//���ļ��ж�ȡ [��λ...]-����-[����...] 
	static bool readEnvelopeDataFromFile(const char* filepathname,
		std::vector<QVector3D>& points);
private:
	
	//��ʾ��ʼ��
	bool mountToScrene(osg::Group* screnceRoot);

	virtual void removefromScrence(osg::Group * screnceRoot);

	//���
	void AddChart3D(const int envelopID, const PosPose& objPosPose,
		const  std::vector<QVector3D>& Points, bool bAmplification, bool bCorrect);

	//����
	bool UpDateChart3D(const int envelopID, const PosPose& objPosPose,
		const  std::vector<QVector3D>& Points, bool bAmplification, bool bCorrect);

	void removeChatPicture(ChatPicture3D* chartptr);
	//�洢
	osg::ref_ptr<osg::Group> m_envelopsNode;
	std::vector<ChatPicture3D*> 	m_ChartPictures;
	bool m_bEnable;
};
