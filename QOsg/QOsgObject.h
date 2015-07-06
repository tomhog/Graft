#ifndef QOSGOBJECT
#define QOSGOBJECT

#include <QOsg/Export.h>
#include <QObject>
#include <QDataStream>
#include <osgDB/Registry>
#include <osgDB/ReaderWriter>

class QOSG_EXPORT QOsgObject : public QObject
{
    Q_OBJECT
public:
    QOsgObject(osg::Object* anObject=NULL)
        : _object(anObject)
    {}
    virtual ~QOsgObject() {}
    QOsgObject(const QOsgObject& obj)
        : _object(obj._object) {
    }

    void setObject(osg::Object* anObject) {
        _object = anObject;
    }
    osg::Object* getObject() {
        return _object.get();
    }

    void write(QDataStream &out) const {

        osgDB::ReaderWriter* writer = osgDB::Registry::instance()->getReaderWriterForExtension("osgt");
        if(writer != NULL)
        {
            std::stringstream strStream;
            osg::ref_ptr<osgDB::Options> options = new osgDB::Options();
            options->setPluginStringData("fileType", "Ascii");
            writer->writeObject(*_object.get(), strStream, options);
            out << QString(strStream.str().c_str());
        }
    }

    void read(QDataStream &in) {
        osgDB::ReaderWriter* reader = osgDB::Registry::instance()->getReaderWriterForExtension("osgt");
        if(reader != NULL)
        {
            QString string;
            in >> string;
            std::stringstream strStream;
            strStream << string.toStdString();

            osg::ref_ptr<osgDB::Options> options = new osgDB::Options();
            options->setPluginStringData("fileType", "Ascii");
            osgDB::ReaderWriter::ReadResult result =  reader->readObject(strStream, options);
            if(result.success())
                _object = result.getObject();
        }
    }

protected:
    osg::ref_ptr<osg::Object> _object;
};
Q_DECLARE_METATYPE(QOsgObject)

#endif // QOSGOBJECT

