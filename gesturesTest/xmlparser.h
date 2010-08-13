#pragma once
#include <QPair>
#include <QPoint>

//todo:: ��-�������� ������� � ��������� ����
typedef QPair<QString, QList<QPoint> > GestureObject;

class XmlParser
{
public:
	static QList<GestureObject> parseXml(const QString & filePath);

private:
	static QList<QPoint> stringToPath(QString const & str);
	static QPoint stringToPoint(QString const & str);
};

