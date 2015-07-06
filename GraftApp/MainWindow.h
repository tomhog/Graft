/*  -*-c++-*-
 *  Copyright (C) 2015 Thomas Hogarth <tom@hogbox.com>
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QProgressDialog>
#include <QDragEnterEvent>
#include <QTimer>

#include <osgViewer/Viewer>
#include <osgQt/GraphicsWindowQt>

#include <Graft/Action.h>
#include <Graft/BatchConvertor.h>
#include <Graft/Scene.h>

namespace Ui {
class MainWindow;
}

class GraftProcessingInputCallback;

//
//
//
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void paintEvent( QPaintEvent* event );
    virtual void showEvent( QShowEvent *event );

protected:

    void createGLView();

    void addInput(const QString& anInputFile, const QString& aCommonDirectory="");
    void selectInput(const unsigned int& anIndex);

    void addActionToUI(graft::Action* anAction);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:

    void on_addInputFilesButton_clicked(bool checked);

    void on_addActionButton_clicked(bool checked);

    void on_addActionComboBox_currentIndexChanged(int index);

    void on_runConvertorButton_clicked(bool checked);

    void on_removeInputFilesButton_clicked(bool checked);

    void on_removeActionButton_clicked(bool checked);

    void on_operationsListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_inputListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_exportButton_clicked(bool checked);

    void on_project_saveActionQueue_triggered(bool state);
    void on_project_loadActionQueue_triggered(bool state);

    void on_backgroundColorButton_clicked(bool checked);

    void on_operationsListWidget_rowsMoved(QModelIndex,int,int,QModelIndex,int);

private:
    Ui::MainWindow *ui;

    osg::ref_ptr<osgQt::GraphicsWindowQt> _osgQtWindow;

    osg::ref_ptr<osgViewer::Viewer> _viewer;
    osg::ref_ptr<graft::Scene> _scene;

    osg::ref_ptr<graft::BatchConvertor> _convertor;
    osg::ref_ptr<GraftProcessingInputCallback> _processCallback;

    QTimer _timer;
};


//
//
//
class GraftProcessingInputCallback : public graft::ProcessingInputCallback
{
public:
    GraftProcessingInputCallback(QWidget* aParentWidget)
        : graft::ProcessingInputCallback(),
          _parentWidget(aParentWidget),
          _progress(NULL)
    {
    }

    // inform listener that we're about to begin processing input at anInputIndex
    // aProcessIndex is the index in the range of 0-aProcessCount
    // aProcessCount is the total being processed this time
    virtual void processing(graft::BatchConvertor* aConvertor, unsigned int anInputIndex, unsigned int aProcessIndex = 0, unsigned int aProcessCount = 1)
    {
        if(aProcessIndex == 0 || _progress == NULL)
        {
            _progress = new QProgressDialog("Processing....................", "Cancel", 0, aProcessCount, _parentWidget);
            _progress->setWindowModality(Qt::WindowModal);
            _progress->setMinimumDuration(0);
            _progress->show();
        }

        graft::FileInputAction* input = (graft::FileInputAction*)aConvertor->getInputs()->getAction(anInputIndex);
        std::string message = "Processing " + osgDB::getSimpleFileName(input->getFilePath());
        _progress->setLabelText(QString(message.c_str()));
        _progress->setValue(aProcessIndex);
    }

    // inform listener that an input as finished being processed by all actions
    // aProcessIndex is the index in the range of 0-aProcessCount
    // aProcessCount is the total being processed this time
    virtual void processingComplete(graft::BatchConvertor* aConvertor, unsigned int anIputIndex, unsigned int aProcessIndex = 0, unsigned int aProcessCount = 1)
    {
        if(_progress == NULL)
            return;

        _progress->setValue(aProcessIndex);

        if(aProcessIndex == aProcessCount-1)
        {
            delete _progress;
            _progress = NULL;
        }
    }

    virtual bool shouldContinue()
    {
        return _progress != NULL ? !_progress->wasCanceled() : true;
    }

    virtual void cancel()
    {
        if(_progress != NULL) {
            delete _progress;
            _progress = NULL;
        }
    }

protected:
    virtual ~GraftProcessingInputCallback() {}

    QProgressDialog* _progress;
    QWidget* _parentWidget;
};

#endif // MAINWINDOW_H
