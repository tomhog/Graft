#include "PropertyWidgets.h"

#include <hbx/Config.h>
#include <hbx/PropertyEdit.h>
#include <hbx/Action.h>

#include <QFileDialog>

namespace hbx {

class DummyAction : public hbx::Action
{
public:
    DummyAction()
        : hbx::Action(),
        _bool(false),
        _string("test"),
        _int(1),
        _float(2.0f),
        _double(3.0),
        _vec2(osg::Vec2(0.0f,1.0f)),
        _vec3(osg::Vec3(2.0f,3.0f,4.0f)),
        _vec4(osg::Vec4(5.0f,6.0f,7.0f,8.0f)),
        _mat(osg::Matrix::identity())
    {
    }

    DummyAction(const DummyAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : hbx::Action(op, copyop)
    {
    }

    META_Object(hbx,DummyAction)

    virtual std::string friendlyName() {return "Dummy";}
    virtual std::string description() {return "For testing different property types";}
    virtual std::string category(){ return "Model"; }

    //
    // serializable properties
    //
    inline void setBool(bool aBool) { _bool = aBool; }
    inline bool getBool() const { return _bool; }

    inline void setString(const std::string& aString) { _string = aString; }
    inline const std::string& getString() const { return _string; }

    inline void setInt(int anInt) { _int = anInt; }
    inline int getInt() const { return _int; }

    inline void setFloat(float aFloat) { _float = aFloat; }
    inline float getFloat() const { return _float; }

    inline void setDouble(double aDouble) { _double = aDouble; }
    inline double getDouble() const { return _double; }

    inline void setVec2(const osg::Vec2& aVec2) { _vec2 = aVec2; }
    inline const osg::Vec2& getVec2() const { return _vec2; }

    inline void setVec3(const osg::Vec3& aVec3) { _vec3 = aVec3; }
    inline const osg::Vec3& getVec3() const { return _vec3; }

    inline void setVec4(const osg::Vec4& aVec4) { _vec4 = aVec4; }
    inline const osg::Vec4& getVec4() const { return _vec4; }

    inline void setMat(const osg::Matrix& aMat) { _mat = aMat; }
    inline const osg::Matrix& getMat() const { return _mat; }

    //
    // apply implementations
    //
    virtual void process(ActionData* aData){}

protected:
    bool _bool;
    std::string _string;
    int _int;
    float _double;
    float _float;
    osg::Vec2 _vec2;
    osg::Vec3 _vec3;
    osg::Vec4 _vec4;
    osg::Matrix _mat;

};

} // end hbx


REGISTER_OBJECT_WRAPPER( hbx_DummyAction,
                         new hbx::DummyAction,
                         hbx::DummyAction,
                         "osg::Object hbx::Action hbx::DummyAction" )
{
    ADD_STRING_SERIALIZER( String, std::string() );  // _string
    ADD_BOOL_SERIALIZER( Bool, false );
    ADD_INT_SERIALIZER( Int, 0 );  // _int
    ADD_FLOAT_SERIALIZER( Float, 0.0f );  // _float
    ADD_DOUBLE_SERIALIZER( Double, 0.0 );  // _double
    ADD_VEC2_SERIALIZER( Vec2, osg::Vec2() );  // _vec2
    ADD_VEC3_SERIALIZER( Vec3, osg::Vec3() );  // _vec3
    ADD_VEC4_SERIALIZER( Vec4, osg::Vec4() );  // _vec4
    ADD_MATRIX_SERIALIZER( Mat, osg::Matrix() );  // _mat
}

PropertyWidget::PropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent)
    : QWidget(parent),
    _object(anObject),
    _propertyName(aPropertyName)
{
    _ci.getPropertyType(_object, _propertyName, _type);
    _wrapper = _ci.getObjectWrapper(_object);

    _hLayout = new QHBoxLayout(this);
    _hLayout->setContentsMargins(0, 0, 0, 0);
}

PropertyWidget* PropertyWidget::Create(osg::Object* anObject, const std::string& aPropertyName, const osgDB::BaseSerializer::Type& aType)
{
    switch(aType)
    {
        case osgDB::BaseSerializer::RW_BOOL:
            return new BoolPropertyWidget(anObject, aPropertyName);
        case osgDB::BaseSerializer::RW_ENUM:
            return new EnumPropertyWidget(anObject, aPropertyName);
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
                return new PathPropertyWidget(anObject, aPropertyName);
            else
                return new TextPropertyWidget(anObject, aPropertyName);
        default: break;
    }
    return NULL;
}

//
//
//
BoolPropertyWidget::BoolPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent)
    : PropertyWidget(anObject, aPropertyName, parent)
{
    _checkbox = new QCheckBox();
    bool value;
    _ci.getProperty<bool>(_object, _propertyName, value);
    _checkbox->setChecked(value);
    _hLayout->addWidget(_checkbox, Qt::AlignTop | Qt::AlignLeft);
    QObject::connect(_checkbox, SIGNAL(stateChanged(int)),
                        this, SLOT(onStateChanged(int)));
}

void BoolPropertyWidget::onStateChanged(int value)
{
    _ci.setProperty<bool>(_object, _propertyName, _checkbox->isChecked());
}

//
//
//
EnumPropertyWidget::EnumPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent)
    : PropertyWidget(anObject, aPropertyName, parent)
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

void EnumPropertyWidget::onCurrentTextChanged(const QString& value)
{
    osgDB::BaseSerializer* serializer = _wrapper->getSerializer(_propertyName);
    osgDB::IntLookup::StringToValue stringToValue = serializer->getIntLookup()->getStringToValue();
    _ci.setProperty<int>(_object, _propertyName, stringToValue[value.toStdString()]);
}

//
//
//
TextPropertyWidget::TextPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent)
    : PropertyWidget(anObject, aPropertyName, parent)
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
            int precision = hbx::Config::instance()->get()->getPrecision();
            validator = new QDoubleValidator(-FLT_MAX, FLT_MAX, precision, this);
            break;
        }
        default: break;
    }

    std::vector<std::string> valueStrings;
    hbx::getPropertyAsStringVector(_object, _propertyName, valueStrings);

    int numElements = hbx::getNumElementsForType(_type);

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

void TextPropertyWidget::onEditingFinished()
{
    int numElements = hbx::getNumElementsForType(_type);

    std::vector<std::string> valueStrings;

    for(int i=0; i<numElements; i++)
        valueStrings.push_back(_lineEdits[i]->text().toStdString());

    hbx::setPropertyFromStringVector(_object, _propertyName, valueStrings);
}

//
//
//
PathPropertyWidget::PathPropertyWidget(osg::Object* anObject, const std::string& aPropertyName, QWidget *parent)
    : PropertyWidget(anObject, aPropertyName, parent)
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

void PathPropertyWidget::onEditingFinished()
{
    std::string valueString;
    valueString = _lineEdit->text().toStdString();
    _ci.setProperty<std::string>(_object, _propertyName, valueString);
}

void PathPropertyWidget::onSelectFileButtonClicked(bool checked)
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

