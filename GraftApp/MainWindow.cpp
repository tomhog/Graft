#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QComboBox>
#include <QMimeData>
#include <QMessageBox>
#include <QColorDialog>
#include <QTreeWidget>

#include <hbx/Formats.h>
#include <hbx/PluginRegistry.h>
#include <hbx/Config.h>
#include "GraphTreeWidget.h"
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
#include <osgUtil/Statistics>


QDataStream &operator<<(QDataStream &out, const ActionWrap *myObj)
{
        myObj->write(out);
        return out;
}

QDataStream &operator>>(QDataStream &in, ActionWrap *myObj)
{
        myObj->read(in);
        return in;
}



class ResizeHandler: public osgGA::GUIEventHandler
{
public:
    ResizeHandler(osgViewer::Viewer* viewer, GraftScene* scene)
        : osgGA::GUIEventHandler(),
          _viewer(viewer),
          _scene(scene)
    {
    }

    virtual bool handle(const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*);
protected:
    osgViewer::Viewer* _viewer;
    GraftScene* _scene;
};

bool ResizeHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*)
{
    if (ea.getEventType() == osgGA::GUIEventAdapter::RESIZE)
    {
        _viewer->getCamera()->setViewport(0,0, ea.getWindowWidth(), ea.getWindowHeight());
        _scene->resize(osg::Vec2(ea.getWindowWidth(), ea.getWindowHeight()));
        return true;
    }
   return false;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->show();
    //this->setStyleSheet("background-color: black;");
    qRegisterMetaTypeStreamOperators<ActionWrap*>("ActionWrap");

    osgQt::initQtWindowingSystem();
    osg::setNotifyHandler(new QTNotifyHandler());

    // init singletons
    hbx::Formats::instance();
    hbx::Config::instance();

    osg::setNotifyLevel(osg::FATAL);

    //build osg viewer
    createGLView();

    //add actions to combobox
    std::map<std::string, hbx::ActionList> actions = hbx::PluginRegistry::instance()->getActionsByCategory();
    for(std::map<std::string, hbx::ActionList>::iterator catItr = actions.begin();
        catItr != actions.end();
        ++catItr)
    {
        for(hbx::ActionList::iterator itr=(*catItr).second.begin();
            itr != (*catItr).second.end();
            ++itr)
        {
            hbx::Action* operation = (*itr).get();
            QVariant var;
            var.setValue(new ActionWrap(operation));
            ui->addActionComboBox->addItem(QString(operation->friendlyName().c_str()), var);
        }
        ui->addActionComboBox->insertSeparator(ui->addActionComboBox->count());
    }

    // hook up signal for action list internal move
    connect(ui->operationsListWidget->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(on_operationsListWidget_rowsMoved(QModelIndex,int,int,QModelIndex,int)));

    //read clear color from config and set color button icon
    _viewer->getCamera()->setClearColor(hbx::Config::instance()->get()->getClearColor());
    osg::Vec4 currentColor = _viewer->getCamera()->getClearColor();
    QColor c(currentColor.r()*255,currentColor.g()*255,currentColor.b()*255, currentColor.a()*255);
    QPixmap px(20, 20);
    px.fill(c);
    ui->backgroundColorButton->setIcon(px);

    //setup convertor
    _convertor = new hbx::BatchConvertor(_viewer->getCamera()->getGraphicsContext());

    _processCallback = new GraftProcessingInputCallback(this);
    _convertor->setCallback(_processCallback);

    //start update timer
    connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
    _timer.start( 30 );
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

bool isInitialized = false;
void MainWindow::showEvent( QShowEvent *event )
{
    // call whatever your base class is!
    QMainWindow::showEvent( event );
    if( event->spontaneous() )
        return;

    if(isInitialized)
        return;

    // get the final size of window once laid out
    osg::Vec2 gcSize = osg::Vec2(_osgQtWindow->getGLWidget()->width(), _osgQtWindow->getGLWidget()->height());
    float pixelRatio = devicePixelRatio();

    // set viewport accounting for pixel ratio
    _viewer->getCamera()->setViewport(0,0,gcSize.x()*pixelRatio, gcSize.y()*pixelRatio);

    //set default projection matrix
    _viewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, gcSize.x() / gcSize.y(), 0.1f, 10000.0f);

    _scene->resize(osg::Vec2(gcSize.x()*pixelRatio, gcSize.y()*pixelRatio));
    isInitialized = true;
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

void MainWindow::createGLView()
{
    //set traits
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName = "";
    traits->windowDecoration = false;
    traits->x = 0;
    traits->y = 0;
    traits->width = 800;
    traits->height = 600;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    //create osgqt window
    _osgQtWindow = new osgQt::GraphicsWindowQt(traits.get());
    // add osgqt widget to layout
    ui->inputRenderLayout->addWidget(_osgQtWindow->getGLWidget());

    osg::Vec2 gcSize = osg::Vec2(_osgQtWindow->getGLWidget()->width(), _osgQtWindow->getGLWidget()->height());
    float pixelRatio = devicePixelRatio();

    //create a viewer an use osgQT window as graphic context
    _viewer = new osgViewer::Viewer();
    _viewer->getCamera()->setGraphicsContext(_osgQtWindow);

    // set viewport accounting for pixel ratio
    _viewer->getCamera()->setViewport(0,0,gcSize.x()*pixelRatio, gcSize.y()*pixelRatio);

    //set default projection matrix
    _viewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, gcSize.x() / gcSize.y(), 0.1f, 10000.0f);

    _viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
    //_viewer->setRunFrameScheme(_viewer->ON_DEMAND);

    // create our scene and set as scene data
    _scene = new GraftScene();
    _viewer->setSceneData(_scene);

    //attach viewer event handlers
    _viewer->addEventHandler(new osgViewer::StatsHandler());
    _viewer->addEventHandler(new ResizeHandler(_viewer.get(), _scene.get()));

    _scene->resize(osg::Vec2(gcSize.x()*pixelRatio, gcSize.y()*pixelRatio));
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
        //clear preview scene
        _scene->clear();
        ui->fileInspector->setTargetObject(NULL);
        //clear stats
        std::stringstream statsStr;
        statsStr << "<html><head/><body><p>Stats</p></body></html>";
        ui->selectedInputStatsLabel->setText(QString(statsStr.str().c_str()));
        return;
    }

    //update ui selections
    ui->inputListWidget->setCurrentRow(anIndex);

    // display selected input in file inspector
    ui->fileInspector->setTargetObject(_convertor->getInputs()->getAction(anIndex));

    //if no data, process the selected input
    if(_convertor->getOutputDatas()[anIndex] == NULL)
        _convertor->process(anIndex);

    //if still no node something went wrong so don't render
    if(_convertor->getOutputDatas()[anIndex]->_object.get() == NULL)
        return;

    hbx::ActionData* selectedData = _convertor->getOutputDatas()[anIndex];

    //
    ui->inputNodeTree->clear();

    // preview selected data
    if(selectedData->asNode() != NULL)
    {
        // it's a model
        _scene->setModel(selectedData->asNode());

        // collect and display stats
        osgUtil::StatsVisitor stats;
        selectedData->asNode()->accept(stats);

        unsigned int instanced_primitives = 0;
        osgUtil::Statistics::PrimitiveCountMap::iterator pcmitr;
        for(pcmitr = stats._instancedStats.GetPrimitivesBegin();
            pcmitr != stats._instancedStats.GetPrimitivesEnd();
            ++pcmitr)
        {
            instanced_primitives += pcmitr->second;
        }

        //
        std::stringstream statsStr;
        statsStr << "<html><head/><body><p>Stats - Vert: " << stats._instancedStats._vertexCount << " Prim: " << instanced_primitives << "</p></body></html>";

        ui->selectedInputStatsLabel->setText(QString(statsStr.str().c_str()));

        // populate tree view
        PopulateTreeWidgetVisitor poptree(ui->inputNodeTree);
        selectedData->asNode()->accept(poptree);

    } else if(selectedData->asImage() != NULL) {

        // it's an image
        _scene->setImage(selectedData->asImage());

        // display stats
        osg::Image* img = selectedData->asImage();

        std::stringstream statsStr;
        statsStr << "<html><head/><body><p>Stats - w: " << img->s() << " h: " << img->t() << "</p></body></html>";

        ui->selectedInputStatsLabel->setText(QString(statsStr.str().c_str()));
    }

    // if now camera mainpulator add one
    if(_viewer->getCameraManipulator() == NULL)
    {
        _viewer->setCameraManipulator(new osgGA::OrbitManipulator());
        _viewer->getCameraManipulator()->home(0.0);
    }
}

void MainWindow::populateInputTreeview()
{
    QTreeWidget* treewidget = ui->inputNodeTree;
    //QTreeView* treeview = treewidget->view
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

    // auto select new action
    int count = ui->operationsListWidget->count();
    ui->operationsListWidget->setCurrentRow(count - 1);
    ui->actionInspector->setTargetObject(anAction);


    // if first then select as default
    /*if(ui->operationsListWidget->count() == 1)
    {
        ui->operationsListWidget->setCurrentRow(0);
        ui->actionInspector->setTargetObject(anAction);
    }*/
}

//
// Slots
//

//
// Add an input file
//
void MainWindow::on_addInputFilesButton_clicked(bool checked)
{
    // create file select dialog
    QFileDialog* fd = new QFileDialog;
    fd->setFileMode(QFileDialog::ExistingFiles);
    //fd->setOption(QFileDialog::ShowDirsOnly);
    fd->setViewMode(QFileDialog::Detail);
    std::string nodeFilterString = "3d data (" + hbx::Formats::instance()->getReadNodeExtensionsString() + ");;";
    nodeFilterString += "image (" + hbx::Formats::instance()->getReadImageExtensionsString() + ");;";
    fd->setNameFilter(QString(nodeFilterString.c_str()));

    //use last import directory if one was saved
    std::string lastImportDirectory = hbx::Config::instance()->get()->getLastImportDirectory();
    if(!lastImportDirectory.empty()){
        if(osgDB::fileType(lastImportDirectory) == osgDB::DIRECTORY)
            fd->setDirectory(QString(lastImportDirectory.c_str()));
    }

    int result = fd->exec();

    if(result)
    {
        // add selected files as inputs
        for(int i=0; i<fd->selectedFiles().size(); i++)
        {
            QString filePath = fd->selectedFiles()[i];
            addInput(filePath);
        }

        //save as last import directory
        hbx::Config::instance()->get()->setLastImportDirectory(osgDB::getFilePath(fd->selectedFiles()[0].toStdString()));
        hbx::Config::instance()->write();
    }

    //
    //_convertor->process(0, _convertor->getInputs()->getNumActions());
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

    // delete from input list
    int selectedIndex = ui->inputListWidget->currentRow();
    _convertor->removeInput(selectedIndex);

    // delete item from list widget
    qDeleteAll(ui->inputListWidget->selectedItems());

    // if no inputs left then clear
    if(ui->inputListWidget->count() == 0)
    {
        //pass size of inputs list to indicate a clear/no selection
        selectInput(_convertor->getInputs()->getNumActions());
        return;
    }

    // select next input
    selectedIndex = osg::clampTo<int>(--selectedIndex, 0, ui->inputListWidget->count()-1);

    // select new input
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

    // open save file dialog
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
        // write file using selected file name
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
    //c = QColorDialog::getColor(c, 0, "Select clear color", QColorDialog::ShowAlphaChannel);

    QColorDialog dlg(this);
    dlg.setWindowTitle("Select clear color");
    //dlg.setOptions(options);
    dlg.setCurrentColor(c);

    int result = dlg.exec();

    if(result)
    {
        c = dlg.selectedColor();
        osg::Vec4 newColor = osg::Vec4(c.redF(), c.greenF(), c.blueF(), c.alphaF());
        _viewer->getCamera()->setClearColor(newColor);
        QPixmap px(20, 20);
        px.fill(c);
        ui->backgroundColorButton->setIcon(px);
        hbx::Config::instance()->get()->setClearColor(newColor);
        hbx::Config::instance()->write();
    }
}

void MainWindow::on_operationsListWidget_rowsMoved(QModelIndex mi1,int i1,int i2,QModelIndex mi2,int i3)
{
    OSG_ALWAYS << "REORDERED: " << mi1.column() << " dest: " << mi2.column() << std::endl;
    OSG_ALWAYS << i1 << " : " << i2 << " : " << i3 << std::endl;

   // the actions list was reordered with drag and drop so resync the convertors list of actions to match
    osg::ref_ptr<hbx::Action> moved = _convertor->getActions()->getAction(i1);
    _convertor->getActions()->removeAction(moved.get());

    if(i3 > i1)
        i3 = osg::maximum<int>(0, i3-1);

    _convertor->getActions()->insertAction(i3, moved.get());
}
