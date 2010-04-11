#pragma once
#include <QList>
#include <QPoint>
#include <QString>

//todo:: � ���� ����� ���� �������� ��� �� ������, �� �������� ��� ���� ������� � ����� (������ �� 2 ������)

class Adopter
{
public:
    Adopter();
    static QString pathToString(QList<QPoint> const & path);
    static QList<QPoint> stringToPath(QString const & str);

private:
    static bool isDigit(QChar const & symbol);
    static bool isPath(QString const & str);
};
