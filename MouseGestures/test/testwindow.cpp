#include "testwindow.h"
#include "ui_testwindow.h"
#include "multistrokeRecognizers/multistrokeGesturesManagers.h"
#include "multistrokeRecognizers/rectanglegesturesmanager.h"
#include "multistrokeRecognizers/sumMultistrokeGesturesManager.h"
#include "multistrokeRecognizers/nearestposgridgesturesmanager.h"
#include "xmlparser.h"
#include "adopter.h"
#include "NeuralNetwork/neuralNetwork.h"
#include "testThread.h"
#include <QFileDialog>
#include <QApplication>

//const QString pathToTestFile = "usersGestures.xml";
const QString pathToTrainingFile = "NeuralNetwork/learnGestures/learn_gestures1.xml";
const QString levXYSortAlgorithm = "levenshtein distance square and sorting by XY coordinates";
const QString levDistHullAlgorithm = "levenshtein distance and hull sorting";
const QString sortXYAlgorithm = "one size keys and sorting by XY";
const QString hullSortAlgorithm = "one size keys and hull sorting";
const QString squaresCurvesAlgorithm = "distance between nearest squares";
const QString trainingGesturesManagerAlgorithm = "training algorithm";
const QString sumGesturesRecognizeAlgorithm = "sum positions algorithm";
const QString rectangleGesturesAlgorithm = "rectangle gestures algorithm";
const QString nearestPosGridAlgorithm = "distance grid algorithm";

TestWindow::TestWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    ui->cbAlgorithm->addItem(squaresCurvesAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(trainingGesturesManagerAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(levXYSortAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(levDistHullAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(sortXYAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(hullSortAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(sumGesturesRecognizeAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(rectangleGesturesAlgorithm, QVariant());
    ui->cbAlgorithm->addItem(nearestPosGridAlgorithm, QVariant());
    connect(ui->bTest, SIGNAL(clicked()), this, SLOT(test()));
    ui->pbTested->setValue(0);
}

void TestWindow::test()
{
    ui->pbTested->setValue(1);
    GesturesManager * gesturesManager = getGesturesManager();
    TestThread * thread = new TestThread(gesturesManager, this);
    connect(thread, SIGNAL(tested(int, int, int)), this, SLOT(printData(int,int,int)));
    ui->pbTested->setValue(5);
    thread->run();
}

void TestWindow::printData(int percent, int all, int recognized)
{
    ui->pbTested->setValue(percent);
    ui->teAll->setText(QString::number(all));
    ui->teRecognized->setText(QString::number(recognized));
    ui->teNotRecognized->setText(QString::number(all - recognized));
}

GesturesManager * TestWindow::getGesturesManager()
{
    QString name = ui->cbAlgorithm->currentText();
    if (name ==  levXYSortAlgorithm)
        return new LevenshteinXYSortGesturesManager();
    else if (name == levDistHullAlgorithm)
        return new LevenshteinHullGesturesManager();
    else if (name == sortXYAlgorithm)
        return new OneSizeXYSortGesturesManager();
    else if (name == squaresCurvesAlgorithm)
        return new SquaresCurveGesturesManager();
    else if (name == trainingGesturesManagerAlgorithm)
    {
        TrainingGesturesManager<GridClassifier> * trainingGesturesManager
                = new TrainingGesturesManager<GridClassifier>();
        QString trainingFile = QFileDialog::getOpenFileName(this, tr("Open Xml"), "",
                                                            tr("Xml files (*.xml)"));
        QMap<QString, UsersGestures> gestures = XmlParser::parseXml(trainingFile);
        foreach (QString object, gestures.keys())
        {
            UsersGestures paths = gestures[object];
            trainingGesturesManager->learn(object, paths.first);
            foreach (QString path, paths.second)
            {
                QApplication::processEvents();
                qDebug() << "new gesture";
                PathVector gesture = Parser::stringToPath(path);
                if (!trainingGesturesManager->belong(object, gesture))
                    trainingGesturesManager->learn(object, gesture);
            }
        }
        qDebug() << "learnt";
        return trainingGesturesManager;
    }
    else if (name == sumGesturesRecognizeAlgorithm)
    {
        qDebug() << "try to initialize";
        return new SumGesturesManager();
    }
    else if (name == rectangleGesturesAlgorithm)
        return new RectangleGesturesManager();
    else if (name == nearestPosGridAlgorithm)
        return new NearestPosGridGesturesManager();
    else return new OneSizeHullGesturesManager();
}


TestWindow::~TestWindow()
{
    delete ui;
}