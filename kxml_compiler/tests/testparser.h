#ifndef TESTPARSER_H
#define TESTPARSER_H

#include <QtCore/QObject>

class TestParser : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testParseAccount();
};

#endif
