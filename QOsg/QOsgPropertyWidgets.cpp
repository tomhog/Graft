#include <QOsg/QOsgPropertyWidgets.h>

#include "PropertyUtils.h"

#include <QFileDialog>


QOsgPropertyWidget::QOsgPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent)
    : QWidget(parent),
    _object(anObject),
    _propertyName(aPropertyName)
{
    _ci.getPropertyType(_object, _propertyName, _type);
    _wrapper = _ci.getObjectWrapper(_object);

    _hLayout = new QHBoxLayout(this);
    _hLayout->setContentsMargins(0, 0, 0, 0);
}

QOsgPropertyWidget* QOsgPropertyWidget::Create(osg::Object* anObject, const std::string& aPropertyName, const osgDB::BaseSerializer::Type& aType)
{
    switch(aType)
    {
        case osgDB::BaseSerializer::RW_BOOL:
            return new QOsgBoolPropertyWidget(anObject, aPropertyName);
        case osgDB::BaseSerializer::RW_ENUM:
            return new QOsgEnumPropertyWidget(anObject, aPropertyName);
        case osgDB::BaseSerializer::RW_STRING:
        case osgDB::BaseSerializer::RW_INT:
        case osgDB::BaseSerializer::RW_UINT:
        case osgDB::BaseSerializer::RW_FLOAT:
        case osgDB::BaseSerializer::RW_DOUBLE:
        case osgDB::BaseSerializer::RW_VEC2F:
        case osgDB::BaseSerializer::RW_VEC2D:
        case osgDB::BaseSerializer::RW_VEC3F:
        case osgDB::BaseSerializer::RW_VEC3D:
        case osgDB::BaseSerializer::RW_VEC4F:
        case osgDB::BaseSerializer::RW_VEC4D:
        case osgDB::BaseSerializer::RW_MATRIXF:
        case osgDB::BaseSerializer::RW_MATRIXD:
            if(aPropertyName.find("Path") != std::string::npos || aPropertyName.find("Directory") != std::string::npos)
                return new QOsgPathPropertyWidget(anObject, aPropertyName);
            else
                return new QOsgTextPropertyWidget(anObject, aPropertyName);
        default: break;
    }
    return NULL;
}

//
//
//
QOsgBoolPropertyWidget::QOsgBoolPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent)
    : QOsgPropertyWidget(anObject, aPropertyName, parent)
{
    _checkbox = new QCheckBox();
    bool value;
    _ci.getProperty<bool>(_object, _propertyName, value);
    _checkbox->setChecked(value);
    _hLayout->addWidget(_checkbox, Qt::AlignTop | Qt::AlignLeft);
    QObject::connect(_checkbox, SIGNAL(stateChanged(int)),
                        this, SLOT(onStateChanged(int)));
}

void QOsgBoolPropertyWidget::onStateChanged(int value)
{
    _ci.setProperty<bool>(_object, _propertyName, _checkbox->isChecked());
}

//
//
//
QOsgEnumPropertyWidget::QOsgEnumPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent)
    : QOsgPropertyWidget(anObject, aPropertyName, parent)
{
    _combobox = new QComboBox();
    _hLayout->addWidget(_combobox, Qt::AlignTop | Qt::AlignLeft);

    osgDB::BaseSerializer* serializer = _wrapper->getSerializer(aPropertyName);
    osgDB::IntLookup::StringToValue stringToValue = serializer->getIntLookup()->getStringToValue();
    for(osgDB::IntLookup::StringToValue::iterator itr=stringToValue.begin();
        itr!=stringToValue.end();
        ++itr)
    {
        _combobox->addItem(QString((*itr).first.c_str()));
    }

    int value;
    _ci.getProperty<int>(_object, _propertyName, value);

    osgDB::IntLookup::ValueToString valueToString = serializer->getIntLookup()->getValueToString();
    _combobox->setCurrentText(QString(valueToString[value].c_str()));

    QObject::connect(_combobox, SIGNAL(currentTextChanged(const QString&)),
                        this, SLOT(onCurrentTextChanged(const QString&)));
}

void QOsgEnumPropertyWidget::onCurrentTextChanged(const QString& value)
{
    osgDB::BaseSerializer* serializer = _wrapper->getSerializer(_propertyName);
    osgDB::IntLookup::StringToValue stringToValue = serializer->getIntLookup()->getStringToValue();
    _ci.setProperty<int>(_object, _propertyName, stringToValue[value.toStdString()]);
}

//
//
//
QOsgTextPropertyWidget::QOsgTextPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent)
    : QOsgPropertyWidget(anObject, aPropertyName, parent)
{
    QValidator* validator = NULL;
    switch(_type)
    {
        case osgDB::BaseSerializer::RW_INT:
            validator = new QIntValidator(-INT_MAX, INT_MAX, this);
            break;
        case osgDB::BaseSerializer::RW_FLOAT:
        case osgDB::BaseSerializer::RW_DOUBLE:
        case osgDB::BaseSerializer::RW_VEC2F:
        case osgDB::BaseSerializer::RW_VEC2D:
        case osgDB::BaseSerializer::RW_VEC3F:
        case osgDB::BaseSerializer::RW_VEC3D:
        case osgDB::BaseSerializer::RW_VEC4F:
        case osgDB::BaseSerializer::RW_VEC4D:
        case osgDB::BaseSerializer::RW_MATRIXF:
        case osgDB::BaseSerializer::RW_MATRIXD:
        {
            int precision = DECIMAL_PLACES; //graft::Config::instance()->get()->getPrecision();
            validator = new QDoubleValidator(-FLT_MAX, FLT_MAX, precision, this);
            break;
        }
        default: break;
    }

    std::vector<std::string> valueStrings;
    osgDB::getPropertyAsStringVector(_object, _propertyName, valueStrings);

    int numElements = osgDB::getNumElementsForType(_type);

    for(int i=0; i<numElements; i++) {
        QLineEdit* lineEdit = new QLineEdit();
        lineEdit->setValidator(validator);
        lineEdit->setText(QString(valueStrings[i].c_str())); //set text after validator is installed
        _hLayout->addWidget(lineEdit, Qt::AlignLeft);
        QObject::connect(lineEdit, SIGNAL(editingFinished()),
                            this, SLOT(onEditingFinished()));
        _lineEdits.push_back(lineEdit);
    }
}

void QOsgTextPropertyWidget::onEditingFinished()
{
    int numElements = osgDB::getNumElementsForType(_type);

    std::vector<std::string> valueStrings;

    for(int i=0; i<numElements; i++)
        valueStrings.push_back(_lineEdits[i]->text().toStdString());

    osgDB::setPropertyFromStringVector(_object, _propertyName, valueStrings);
}

//
//
//
QOsgPathPropertyWidget::QOsgPathPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent)
    : QOsgPropertyWidget(anObject, aPropertyName, parent)
{
    std::string valueString;
    _ci.getProperty<std::string>(anObject, aPropertyName, valueString);

    _lineEdit = new QLineEdit();
    _lineEdit->setText(QString(valueString.c_str())); //set text after validator is installed
    _hLayout->addWidget(_lineEdit, Qt::AlignLeft);
    QObject::connect(_lineEdit, SIGNAL(editingFinished()),
                        this, SLOT(onEditingFinished()));

    _button = new QToolButton();
    _button->setText("...");
    _hLayout->addWidget(_button, Qt::AlignLeft);
    QObject::connect(_button, SIGNAL(clicked(bool)),
                     this, SLOT(onSelectFileButtonClicked(bool)));
}

void QOsgPathPropertyWidget::onEditingFinished()
{
    std::string valueString;
    valueString = _lineEdit->text().toStdString();
    _ci.setProperty<std::string>(_object, _propertyName, valueString);
}

void QOsgPathPropertyWidget::onSelectFileButtonClicked(bool checked)
{
    QFileDialog* fd = new QFileDialog;
    fd->setFileMode(QFileDialog::ExistingFile);
    if(_propertyName.find("Directory") != std::string::npos)
    {
        fd->setOption(QFileDialog::ShowDirsOnly);
        fd->setFileMode(QFileDialog::Directory);
    } else {
        std::string nodeFilterString = "Any file (*.*);;";
        fd->setNameFilter(QString(nodeFilterString.c_str()));
    }
    fd->setViewMode(QFileDialog::Detail);


    int result = fd->exec();

    if(result)
    {
        for(int i=0; i<fd->selectedFiles().size(); i++)
        {
            QString filePath = fd->selectedFiles()[i];
            _lineEdit->setText(filePath);
            onEditingFinished();
        }
    }
}

