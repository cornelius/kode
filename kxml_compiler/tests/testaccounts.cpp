#include "account.h"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFileInfo>

#include <QDebug>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("testaccounts");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser cmdLine;
    cmdLine.setApplicationDescription("Test program");
    cmdLine.addHelpOption();
    cmdLine.addVersionOption();

    QCommandLineOption dirOption("xmlfile",
                                 QCoreApplication::translate("main", "Name of XML file"));
    cmdLine.addOption(dirOption);

    if (!cmdLine.parse(QCoreApplication::arguments())) {
        qDebug() << cmdLine.errorText();
        return -1;
    }

    if (cmdLine.positionalArguments().count() < 1 || cmdLine.positionalArguments().count() > 2) {
        qDebug() << "Wrong number of arguments";
        cmdLine.showHelp(-1);
    }

    QString filename = cmdLine.positionalArguments().at(0);
    QString referenceFile;
    if (cmdLine.positionalArguments().count() == 2) {
        referenceFile = cmdLine.positionalArguments().at(1);
    } else {
        referenceFile = filename;
    }

    QString outputFile = "out_" + QFileInfo(filename).fileName();

    qDebug() << "Comparing" << referenceFile << "and" << outputFile;

    bool ok;
    Account account = Account::parseFile(filename, &ok);

    if (!ok) {
        qDebug() << "Parse error";
        return 1;
    } else {
        qDebug() << "DISPLAY NAME" << account.displayname();

        QDate date = account.creationDate().value();
        qDebug() << date;
        QDateTime dateTime = account.email().updatedAt();
        qDebug() << dateTime;
        if (account.resources().resourceList().size() != 2)
            exit(1);
        if (account.resources3().resource3List().size() != 3)
            exit(1);
        account.resources().resourceList().first().isValid();

        account.writeFile(outputFile);
        int exitCode = system(QString("diff %1 %2").arg(referenceFile).arg(outputFile).toUtf8());
        if (exitCode != 0)
            exit(1);
    }

    return 0;
}
