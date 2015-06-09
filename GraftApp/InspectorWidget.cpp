#include "InspectorWidget.h"

#include "PropertyWidgets.h"

InspectorWidget::InspectorWidget(QWidget *parent) : QScrollArea(parent)
{
    _scrollAreaContents = new QWidget(this);
    _layout = new QFormLayout();
    _layout->setLabelAlignment(Qt::AlignLeft);
    _layout->setFormAlignment( Qt::AlignLeft | Qt::AlignTop );
    //_layout->set
}

void InspectorWidget::setTargetObject(osg::Object* anObject)
{
    this->setWidget(NULL);
    _scrollAreaContents->setLayout(NULL);

    clearLayout(_layout, true);

    if(anObject == NULL) return;

    osgDB::ClassInterface ci;
    osgDB::ObjectWrapper* ow = ci.getObjectWrapper(anObject);

    osgDB::ClassInterface::PropertyMap blackList;
    blackList["Name"] = osgDB::BaseSerializer::RW_STRING;
    blackList["DataVariance"] = osgDB::BaseSerializer::RW_ENUM;
    blackList["UserData"] = osgDB::BaseSerializer::RW_USER;
    blackList["UserDataContainer"] = osgDB::BaseSerializer::RW_OBJECT;

    osgDB::ClassInterface::PropertyMap propMap;
    ci.getSupportedProperties(anObject, propMap, true);

    const osgDB::StringList& associates = ow->getAssociates();
    for(osgDB::StringList::const_iterator aitr = associates.begin();
        aitr != associates.end();
        ++aitr)
    {
        osgDB::ObjectWrapper* associate_wrapper = osgDB::Registry::instance()->getObjectWrapperManager()->findWrapper(*aitr);
        if (associate_wrapper)
        {
            const osgDB::ObjectWrapper::SerializerList& associate_serializers = associate_wrapper->getSerializerList();
            unsigned int i=0;
            for(osgDB::ObjectWrapper::SerializerList::const_iterator sitr = associate_serializers.begin();
                sitr != associate_serializers.end();
                ++sitr, ++i)
            {
                const std::string& propertyName = (*sitr)->getName();
                bool notBlackListed = (blackList.count(propertyName)==0);
                if (notBlackListed) {
                   // properties[propertyName] = associate_wrapper->getTypeList()[i];
                    //OSG_ALWAYS << "Property: " << (*propItr).first << " : " << ci.getTypeName((*propItr).second) << std::endl;
                    //create readable label
                    QString qpropname = QString(propertyName.c_str());
                    QStringList sl = qpropname.split(QRegExp("(?=[A-Z])"), QString::SkipEmptyParts);
                    std::string label = sl.join(" ").toStdString();

                    PropertyWidget* propWidget = PropertyWidget::Create(anObject, propertyName, associate_wrapper->getTypeList()[i]);
                    _layout->addRow(QString(label.c_str()), propWidget);
                }
            }
        }
    }

    _scrollAreaContents->setLayout(_layout);
    this->setWidget(_scrollAreaContents);
}

void InspectorWidget::clearLayout(QLayout* layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                delete widget;
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}
