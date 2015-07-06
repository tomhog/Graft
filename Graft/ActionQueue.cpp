
#include <Graft/ActionQueue.h>

#include <osg/ValueObject>
#include <osgDB/ObjectWrapper>
#include <osgDB/InputStream>
#include <osgDB/OutputStream>

#include <stdio.h>
#include <math.h>

#include <algorithm>

using namespace graft;

ActionQueue::ActionQueue()
{
}

ActionQueue::ActionQueue(const ActionQueue& group,const osg::CopyOp& copyop):
    osg::Object(group,copyop)
{
    for(ActionList::const_iterator itr=group._children.begin();
        itr!=group._children.end();
        ++itr)
    {
        Action* child = dynamic_cast<Action*>(copyop(itr->get()));
        if (child) addAction(child);
    }
}

ActionQueue::~ActionQueue()
{
    // remove reference to this from children's parent lists.
    for(ActionList::iterator itr=_children.begin();
        itr!=_children.end();
        ++itr)
    {
        //(*itr)->removeParent(this);
    }

}

bool ActionQueue::process(ActionData* aData)
{
    for(ActionList::iterator itr=_children.begin();
        itr!=_children.end();
        ++itr)
    {
        (*itr)->process(aData);
    }
    return true;
}

bool ActionQueue::addAction( Action *anAction )
{
    return ActionQueue::insertAction( _children.size(), anAction );
}

bool ActionQueue::insertAction(unsigned int index, Action *anAction)
{
    if (!anAction) return false;

    if (containsAction(anAction))
    {
        OSG_WARN<<"Adding non unique child to osg::Group, ignoring call"<<std::endl;
        return false;
    }

    if (anAction)
    {
        // note ref_ptr<> automatically handles incrementing child's reference count.
        if (index >= _children.size())
        {
            index = _children.size();      // set correct index value to be passed to the "childInserted" method
            _children.push_back(anAction);
        }
        else
        {
            _children.insert(_children.begin()+index, anAction);
        }

        // register as parent of child.
        //child->addParent(this);

        // tell any subclasses that a child has been inserted so that they can update themselves.
        operationInserted(index);

        //dirtyBound();

        return true;
    }
    else return false;
}

unsigned int ActionQueue::getNumActions() const
{
    return static_cast<unsigned int>(_children.size());
}


bool ActionQueue::removeAction(Action *anAction)
{
    unsigned int pos = getActionIndex(anAction);
    if (pos<_children.size()) return removeAction(pos,1);
    else return false;
}


bool ActionQueue::removeActions(unsigned int pos,unsigned int numChildrenToRemove)
{
    if (pos<_children.size() && numChildrenToRemove>0)
    {
        unsigned int endOfRemoveRange = pos+numChildrenToRemove;
        if (endOfRemoveRange>_children.size())
        {
            OSG_DEBUG<<"Warning: Group::removeChild(i,numChildrenToRemove) has been passed an excessive number"<<std::endl;
            OSG_DEBUG<<"         of chilren to remove, trimming just to end of child list."<<std::endl;
            endOfRemoveRange=_children.size();
        }

        operationRemoved(pos,endOfRemoveRange-pos);

        _children.erase(_children.begin()+pos,_children.begin()+endOfRemoveRange);

        //dirtyBound();

        return true;
    }
    else return false;
}


bool ActionQueue::replaceAction(Action *origAction, Action *newAction)
{
    if (newAction==NULL || origAction==newAction) return false;

    unsigned int pos = getActionIndex(origAction);
    if (pos<_children.size())
    {
        return setAction(pos,newAction);
    }
    return false;
}


bool ActionQueue::setAction(unsigned int i, Action* newAction)
{
    if (i<_children.size() && newAction)
    {

        osg::ref_ptr<Action> origAction = _children[i];

        // first remove for origNode's parent list.
        //origNode->removeParent(this);

        // note ref_ptr<> automatically handles decrementing origNode's reference count,
        // and inccrementing newNode's reference count.
        _children[i] = newAction;

        // register as parent of child.
        //newNode->addParent(this);

        //dirtyBound();

        return true;
    }
    else return false;
}

//
// Serialize
//

static bool checkChildren( const graft::ActionQueue& aQueue )
{
    return aQueue.getNumActions()>0;
}

static bool readChildren( osgDB::InputStream& is, graft::ActionQueue& aQueue )
{
    unsigned int size = 0; is >> size >> is.BEGIN_BRACKET;
    for ( unsigned int i=0; i<size; ++i )
    {
        osg::ref_ptr<osg::Object> obj = is.readObject();
        graft::Action* child = dynamic_cast<graft::Action*>( obj.get() );
        if ( child ) aQueue.addAction( child );
    }
    is >> is.END_BRACKET;
    return true;
}

static bool writeChildren( osgDB::OutputStream& os, const graft::ActionQueue& aQueue )
{
    unsigned int size = aQueue.getNumActions();
    os << size << os.BEGIN_BRACKET << std::endl;
    for ( unsigned int i=0; i<size; ++i )
    {
        os << aQueue.getAction(i);
    }
    os << os.END_BRACKET << std::endl;
    return true;
}


REGISTER_OBJECT_WRAPPER( graft_ActionQueue,
                         new graft::ActionQueue,
                         graft::ActionQueue,
                         "osg::Object graft::ActionQueue" )
{
    ADD_USER_SERIALIZER( Children );  // _children
}
