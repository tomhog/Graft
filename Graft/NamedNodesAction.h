#ifndef GRAFT_NAMEDNODESACTION_H
#define GRAFT_NAMEDNODESACTION_H

#include <Graft/Export.h>
#include <Graft/Action.h>

namespace graft
{


//
// Find all nodes in the graph that match the name passed
//
class GRAFT_EXPORT FindNodesByName : public osg::NodeVisitor
{
public:

    FindNodesByName(std::string searchName, bool subString=false)
        : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
        _searchName(searchName),
        _searchSubString(subString),
        _count(0)
    {
    }

    virtual void apply(osg::Node& node)
    {
        if(_searchSubString)
        {
            size_t found=node.getName().rfind(_searchName);
            if (found!=std::string::npos)
            {
                _foundList.push_back(&node);
                _count++;
            }
        }else{
            if(node.getName() == _searchName){
                _foundList.push_back(&node);
                _count++;
            }
        }
        traverse(node);
    }

    std::string _searchName;
    //search for name as substring of name
    bool _searchSubString;

    std::vector< osg::observer_ptr<osg::Node> > _foundList;
    int _count;
};


class GRAFT_EXPORT NamedNodesAction : public Action
{
public:
    NamedNodesAction()
        : Action(),
          _searchString(""),
          _wholeWord(false)
    {
    }

    NamedNodesAction(const NamedNodesAction& op, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
     : Action(op, copyop),
       _searchString(op._searchString),
        _wholeWord(op._wholeWord)
    {
    }

    virtual osg::Object* cloneType() const = 0;
    virtual osg::Object* clone(const osg::CopyOp&) const = 0;
    virtual bool isSameKindAs(const osg::Object* obj) const { return dynamic_cast<const VisitorAction*>(obj)!=NULL; }
    virtual const char* libraryName() const { return "graft"; }
    virtual const char* className() const { return "NamedNodesAction"; }

    // process the individal nodes found with the _searchString, could be null
    virtual void processNamedNode(ActionData* aData, osg::Node* aNode) = 0;

    virtual void process(ActionData* aData) {
        if(aData->asNode() == NULL) return;
        osg::ref_ptr<FindNodesByName> find = new FindNodesByName(_searchString, !_wholeWord);
        aData->asNode()->accept(*find);

        //if(find->_foundList.size() > 0)
        for(std::vector< osg::observer_ptr<osg::Node> >::iterator itr = find->_foundList.begin();
            itr != find->_foundList.end();
            ++itr)
        {
            processNamedNode(aData, (*itr).get());
        }
    }

    //
    // Serializable
    //

    inline void setSearchString(const std::string& aString) { _searchString = aString; }
    inline const std::string& getSearchString() const { return _searchString; }

    bool getWholeWord() const { return _wholeWord; }
    void setWholeWord(bool isEnabled){ _wholeWord = isEnabled; }

protected:
    std::string _searchString;
    bool _wholeWord;
};

} // end graft namespace

#endif // GRAFT_NAMEDNODESACTION_H
