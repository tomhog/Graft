#ifndef QOSGTREEWIDGET
#define QOSGTREEWIDGET

#include <QOsg/Export.h>
#include <QTreeWidget>
#include <osg/NodeVisitor>

class PopulateTreeWidgetVisitor : public osg::NodeVisitor
{
public:
    PopulateTreeWidgetVisitor(QTreeWidget* tree):
        osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
        _tree(tree),
        _parent(NULL)
    {}

    virtual void apply(osg::Node& node)
    {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem((_parent == NULL ? _tree : NULL));

        // QTreeWidgetItem::setText(int column, const QString & text)
        treeItem->setText(0, QString(node.getName().c_str()) + " (" + QString(node.className()) + ")");

        if(_parent != NULL)
            _parent->addChild(treeItem);

        QTreeWidgetItem* oldparent = _parent;
        _parent = treeItem;
        traverse(node);
        _parent = oldparent;
    }
    QTreeWidget* _tree;
    QTreeWidgetItem *_parent;
};

#endif // QOSGTREEWIDGET
