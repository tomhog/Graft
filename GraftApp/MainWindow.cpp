#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QComboBox>
#include <QMimeData>
#include <QMessageBox>
#include <QColorDialog>
#include <osgQt/GraphicsWindowQt>

#include <hbx/Formats.h>
#include <hbx/PluginRegistry.h>
#include <hbx/Config.h>
#include "QTNotifyHandler.h"

#include <osg/Notify>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/ClassInterface>

#include <osgViewer/ViewerEventHandlers>
#include <osgGA/OrbitManipulator>
#include <osgGA/StateSetManipulator>

#include "Ommatidium.h"


class ResizeHandler: public osgGA::GUIEventHandler
{
public:
    ResizeHandler(osgViewer::Viewer* viewer)
        : osgGA::GUIEventHandler(),
          _viewer(viewer)
    {
    }

    virtual bool handle(const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*);
protected:
    osgViewer::Viewer* _viewer;
};

bool ResizeHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*)
{
    if (ea.getEventType() == osgGA::GUIEventAdapter::RESIZE)
    {
        std::cerr << "Resize event, time = " << ea.getTime() << "\n";
        _viewer->getCamera()->setViewport(0,0, ea.getWindowWidth(), ea.getWindowHeight());
        return true;
    }
   return false;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Ommatidium o;

    ui->setupUi(this);
    //this->setStyleSheet("background-color: black;");

    osgQt::initQtWindowingSystem();
    osg::setNotifyHandler(new QTNotifyHandler());

    hbx::Config::instance();
    hbx::Formats::instance();

    osg::setNotifyLevel(osg::FATAL);

    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName = "";
    traits->windowDecoration = false;
    traits->x = 0;
    traits->y = 0;
    traits->width = 800;//this->ui->inputRenderLayout->geometry().width();
    traits->height = 600;//this->ui->inputRenderLayout->geometry().height();
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    osgQt::GraphicsWindowQt* gc = new osgQt::GraphicsWindowQt(traits.get());
    ui->inputRenderLayout->addWidget(gc->getGLWidget());
    QSize gcSize = gc->getGLWidget()->frameSize();
    OSG_ALWAYS << "GLWidget Dimensions: " << gcSize.width() << ", " << gcSize.height() << std::endl;

    int pixelRatio = devicePixelRatio();

    _viewer = new osgViewer::Viewer();
    _viewer->getCamera()->setGraphicsContext(gc);

    _viewer->getCamera()->setViewport(0,0,gcSize.width()*pixelRatio, gcSize.height()*pixelRatio);
    _viewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, _viewer->getCamera()->getViewport()->width() / _viewer->getCamera()->getViewport()->height(), 0.1f, 10000.0f);

    _viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
   // _viewer->setRunFrameScheme(_viewer->ON_DEMAND);
   // _viewer->setCameraManipulator(new osgGA::OrbitManipulator());

    _viewer->addEventHandler(new osgViewer::StatsHandler());
    _viewer->addEventHandler(new ResizeHandler(_viewer.get()));
    //_viewer->addEventHandler(new osgGA::StateSetManipulator());
    //_viewer->addEventHandler(new osgViewer::ThreadingHandler());

    _root = new osg::Group();
    _viewer->setSceneData(_root);

    connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
    _timer.start( 30 );

    for(hbx::ActionList::const_iterator opItr = hbx::PluginRegistry::instance()->getActionsList().begin();
        opItr != hbx::PluginRegistry::instance()->getActionsList().end();
        ++opItr)
    {
        hbx::Action* operation = (*opItr).get();
        QVariant var;
        var.setValue(new ActionWrap(operation));
        ui->addActionComboBox->addItem(QString(operation->friendlyName().c_str()), var);
    }

    _convertor = new hbx::BatchConvertor(gc);

    _processCallback = new GraftProcessingInputCallback(this);
    _convertor->setCallback(_processCallback);
}

MainWindow::~MainWindow()
{
    delete ui;
    _timer.stop();
}

void MainWindow::paintEvent( QPaintEvent* event )
{
    _viewer->frame();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> droppedUrls = event->mimeData()->urls();
    int droppedUrlCnt = droppedUrls.size();
    for(int i = 0; i < droppedUrlCnt; i++) {
        QString localPath = droppedUrls[i].toLocalFile();
        QFileInfo fileInfo(localPath);
        if(fileInfo.isFile()) {
            std::string filestr = fileInfo.absoluteFilePath().toStdString();
            if(hbx::Formats::instance()->supported(osgDB::getFileExtension(filestr)))
                addInput(fileInfo.absoluteFilePath(), QString());
        }
        else if(fileInfo.isDir()) {
            osgDB::FilePathList files;
            hbx::getAllSupportedFilesInFolder(fileInfo.absoluteFilePath().toStdString(), files);
            for(osgDB::FilePathList::iterator itr=files.begin(); itr!=files.end(); ++itr)
                addInput(QString((*itr).c_str()), fileInfo.absoluteFilePath());

        }
        else {
            QMessageBox::information(this, tr("Dropped, but unknown"), tr("Unknown: %1").arg(fileInfo.absoluteFilePath()));
        }
    }

    event->acceptProposedAction();
}

void MainWindow::addInput(const QString& anInputFile, const QString& aCommonDirectory)
{
    hbx::FileInputAction* inputOp = _convertor->addInput(anInputFile.toStdString(), aCommonDirectory.toStdString());

    _convertor->getInputs()->addAction(inputOp);

    QVariant opData;
    opData.setValue(new ActionWrap(inputOp));

    QListWidgetItem* newItem = new QListWidgetItem;
    newItem->setText( QString(osgDB::getSimpleFileName(anInputFile.toStdString()).c_str()) );
    newItem->setData(Qt::UserRole, opData);
    newItem->setToolTip(anInputFile);
    ui->inputListWidget->addItem(newItem);

    if(ui->inputListWidget->count() == 1)
        selectInput(0);
}

void MainWindow::selectInput(const unsigned int& anIndex)
{
    if(anIndex >= _convertor->getInputs()->getNumActions()) {
        _root->removeChildren(0, _root->getNumChildren());
        ui->fileInspector->setTargetObject(NULL);
        return;
    }

    //update ui selections
    ui->inputListWidget->setCurrentRow(anIndex);
    ui->fileInspector->setTargetObject(_convertor->getInputs()->getAction(anIndex));

    //if no data process the the index
    if(_convertor->getOutputDatas()[anIndex] == NULL)
        _convertor->process(anIndex);

    //if still no node something went wrong so don't render
    if(_convertor->getOutputDatas()[anIndex]->_object.get() == NULL)
        return;

    //set as scene data to render
    _root->removeChildren(0, _root->getNumChildren());
    if(_convertor->getOutputDatas()[anIndex]->asNode() != NULL)
    _root->addChild(_convertor->getOutputDatas()[anIndex]->asNode());

    if(_viewer->getCameraManipulator() == NULL)
    {
        _viewer->setCameraManipulator(new osgGA::OrbitManipulator());
        _viewer->getCameraManipulator()->home(0.0);
    }
}

void MainWindow::addActionToUI(hbx::Action* anAction)
{
    // store new operation in ActionWrap variant
    QVariant opData;
    opData.setValue(new ActionWrap(anAction));

    // create operation list item widget with ActionWrap variant as data
    QListWidgetItem* newItem = new QListWidgetItem;
    newItem->setText(QString(anAction->friendlyName().c_str()));
    newItem->setData(Qt::UserRole, opData);

    // add the item to the operations list widget
    ui->operationsListWidget->addItem(newItem);

    //if first then select as default
    if(ui->operationsListWidget->count() == 1)
    {
        ui->operationsListWidget->setCurrentRow(0);
        ui->actionInspector->setTargetObject(anAction);
    }
}

//
// Slots
//

//
// Add an input file
//
void MainWindow::on_addInputFilesButton_clicked(bool checked)
{
    QFileDialog* fd = new QFileDialog;
    fd->setFileMode(QFileDialog::ExistingFiles);
    //fd->setOption(QFileDialog::ShowDirsOnly);
    fd->setViewMode(QFileDialog::Detail);
    std::string nodeFilterString = "3d data (" + hbx::Formats::instance()->getReadNodeExtensionsString() + ");;";
    nodeFilterString += "image (" + hbx::Formats::instance()->getReadImageExtensionsString() + ");;";
    fd->setNameFilter(QString(nodeFilterString.c_str()));

    //use last directory if one was saved
    std::string lastImportDirectory = hbx::Config::instance()->get()->getLastImportDirectory();
    if(!lastImportDirectory.empty()){
        if(osgDB::fileType(lastImportDirectory) == osgDB::DIRECTORY)
            fd->setDirectory(QString(lastImportDirectory.c_str()));
    }

    int result = fd->exec();

    if(result)
    {
        for(int i=0; i<fd->selectedFiles().size(); i++)
        {
            QString filePath = fd->selectedFiles()[i];
            addInput(filePath);
        }

        //save the directory
        hbx::Config::instance()->get()->setLastImportDirectory(osgDB::getFilePath(fd->selectedFiles()[0].toStdString()));
        hbx::Config::instance()->write();
    }
    _convertor->process(0, _convertor->getInputs()->getNumActions());
}

void MainWindow::on_addActionButton_clicked(bool checked)
{
    //get selected operation type
    QVariant selectedData = ui->addActionComboBox->currentData();
    ActionWrap* wrap = selectedData.value<ActionWrap*>();

    // allocate an operation
    hbx::Action* op = dynamic_cast<hbx::Action*>(wrap->getAction()->cloneType());

    _convertor->getActions()->addAction(op);

    addActionToUI(op);
}

void MainWindow::on_addActionComboBox_currentIndexChanged(int index)
{

}

void MainWindow::on_operationsListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    hbx::Action* operation = NULL;
    if(current != NULL)
    {
        QVariant selectedData = current->data(Qt::UserRole);
        ActionWrap* wrap = selectedData.value<ActionWrap*>();
        operation = wrap->getAction();
    }

    ui->actionInspector->setTargetObject(operation);
}

void MainWindow::on_runConvertorButton_clicked(bool checked)
{
    if(_convertor->getInputs()->getNumActions() == 0)
        return;

    int selectedRow = ui->inputListWidget->currentRow();
    if(selectedRow == -1) selectedRow = 0;

    OSG_ALWAYS << "RUN: " << selectedRow << std::endl;

    if(!ui->processSelectedOnlyCheckbox->isChecked())
        _convertor->process(0, _convertor->getInputs()->getNumActions());
    else
        _convertor->process(selectedRow);

    selectInput(selectedRow);
}

void MainWindow::on_inputListWidget_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    selectInput(ui->inputListWidget->currentRow());
}

void MainWindow::on_removeInputFilesButton_clicked(bool checked)
{
    if(ui->inputListWidget->count() == 0|| ui->inputListWidget->selectedItems().count() == 0)
        return;

    int selectedIndex = ui->inputListWidget->currentRow();
    _convertor->removeInput(selectedIndex);

    qDeleteAll(ui->inputListWidget->selectedItems());

    if(ui->inputListWidget->count() == 0)
    {
        //pass size of inputs list to indicate a clear/no selection
        selectInput(_convertor->getInputs()->getNumActions());
        return;
    }

    selectedIndex = osg::clampTo<int>(--selectedIndex, 0, ui->inputListWidget->count()-1);

    selectInput(selectedIndex);
}

void MainWindow::on_removeActionButton_clicked(bool checked)
{
    if(ui->operationsListWidget->count() == 0 || ui->operationsListWidget->selectedItems().count() == 0)
        return;

    int selectedIndex = ui->operationsListWidget->currentRow();
    _convertor->getActions()->removeAction(selectedIndex);

    qDeleteAll(ui->operationsListWidget->selectedItems());

    if(ui->operationsListWidget->count() == 0)
    {
        ui->actionInspector->setTargetObject(NULL);
        return;
    }

    selectedIndex = osg::clampTo<int>(--selectedIndex, 0, ui->operationsListWidget->count()-1);

    ui->actionInspector->setTargetObject(_convertor->getActions()->getAction(selectedIndex));
}


void MainWindow::on_exportButton_clicked(bool checked)
{
    if(ui->inputListWidget->currentRow() == -1)
        return;

    QFileDialog* fd = new QFileDialog;
    fd->setWindowTitle("Export file");
    fd->setAcceptMode(QFileDialog::AcceptSave);
    fd->setFileMode(QFileDialog::AnyFile);
    fd->setOption(QFileDialog::ShowDirsOnly);
    fd->setViewMode(QFileDialog::Detail);
    std::string nodeFilterString = "3d data (" + hbx::Formats::instance()->getReadNodeExtensionsString() + ");;";
    fd->setNameFilter(QString(nodeFilterString.c_str()));

    int result = fd->exec();

    if(result)
    {
        QString filePath = fd->selectedFiles()[0];
        hbx::ActionData* selected = _convertor->getOutputDatas()[ui->inputListWidget->currentRow()];
        if(selected->asNode() != NULL)
            osgDB::writeNodeFile(*selected->asNode(), filePath.toStdString());
        else if(selected->asImage() != NULL)
            osgDB::writeImageFile(*selected->asImage(), filePath.toStdString());
    }
}

void MainWindow::on_project_saveActionQueue_triggered(bool state)
{
    if(_convertor->getActions()->getNumActions() == 0)
    {
        QMessageBox::information(this, tr("No Actions in Queue to save"), tr("Please add some operations before saving."));
        return;
    }

    QFileDialog* fd = new QFileDialog;
    fd->setWindowTitle("Save Action Queue");
    fd->setAcceptMode(QFileDialog::AcceptSave);
    fd->setFileMode(QFileDialog::AnyFile);
    fd->setOption(QFileDialog::ShowDirsOnly);
    fd->setViewMode(QFileDialog::Detail);
    std::string nodeFilterString = "Action Queue (*.osgt);;";
    fd->setNameFilter(QString(nodeFilterString.c_str()));

    int result = fd->exec();

    if(result)
    {
        QString filePath = fd->selectedFiles()[0];
        osgDB::writeObjectFile(*_convertor->getActions(), filePath.toStdString());
    }
}

void MainWindow::on_project_loadActionQueue_triggered(bool state)
{
    QFileDialog* fd = new QFileDialog;
    fd->setFileMode(QFileDialog::ExistingFile);
    fd->setOption(QFileDialog::ShowDirsOnly);
    fd->setViewMode(QFileDialog::Detail);
    std::string nodeFilterString = "Action Queue (*.osgt);;";
    fd->setNameFilter(QString(nodeFilterString.c_str()));

    int result = fd->exec();

    if(result)
    {
        QString filePath = fd->selectedFiles()[0];
        osg::ref_ptr<hbx::ActionQueue> loadedQueue = dynamic_cast<hbx::ActionQueue*>(osgDB::readObjectFile(filePath.toStdString()));

        if(!loadedQueue.valid())
        {
            QMessageBox::information(this, tr("Failed to load an Action Queue from file"), tr("File: %1").arg(filePath));
            return;
        }

        if(loadedQueue->getNumActions() == 0)
        {
            QMessageBox::information(this, tr("The loaded Action Queue is empty"), tr("File: %1").arg(filePath));
            return;
        }

        //destory current operationqueue ui
        ui->operationsListWidget->clear();

        _convertor->setActions(loadedQueue);
        //iterate and add all the loaded operation to the ui
        for(unsigned int i=0; i<_convertor->getActions()->getNumActions(); i++)
        {
            addActionToUI(_convertor->getActions()->getAction(i));
        }

    }
}

void MainWindow::on_backgroundColorButton_clicked(bool checked)
{
    osg::Vec4 currentColor = _viewer->getCamera()->getClearColor();
    QColor c(currentColor.r()*255,currentColor.g()*255,currentColor.b()*255, currentColor.a()*255);
    c = QColorDialog::getColor(c, 0, "Select background color", QColorDialog::ShowAlphaChannel);
    _viewer->getCamera()->setClearColor(osg::Vec4(c.redF(), c.greenF(), c.blueF(), c.alphaF()));
}
