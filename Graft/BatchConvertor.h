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

#ifndef GRAFT_BATCHCONVERTOR_H
#define GRAFT_BATCHCONVERTOR_H 1

#include <Graft/ActionQueue.h>
#include <Graft/FileOutputAction.h>
#include <Graft/FileInputAction.h>

namespace graft {

class BatchConvertor;

class GRAFT_EXPORT ProcessingInputCallback : public virtual osg::Referenced
{
public:
    // inform listener that we're about to begin processing input at anInputIndex
    // aProcessIndex is the index in the range of 0-aProcessCount
    // aProcessCount is the total being processed this time
    virtual void processing(BatchConvertor* aConvertor, unsigned int anInputIndex, unsigned int aProcessIndex = 0, unsigned int aProcessCount = 1) = 0;

    // inform listener that an input as finished being processed by all actions
    // aProcessIndex is the index in the range of 0-aProcessCount
    // aProcessCount is the total being processed this time
    virtual void processingComplete(BatchConvertor* aConvertor, unsigned int anIputIndex, unsigned int aProcessIndex = 0, unsigned int aProcessCount = 1) = 0;

    virtual bool shouldContinue() { return true; }

    virtual void cancel() = 0;

protected:
    virtual ~ProcessingInputCallback() {}
};

class GRAFT_EXPORT BatchConvertor : public osg::Referenced
{
public:
    BatchConvertor(osg::GraphicsContext* context)
        : osg::Referenced(),
        _inputActions(new ActionQueue()),
        _operations(new ActionQueue()),
        _context(context)
    {}

    ActionQueue* getInputs(){ return _inputActions; }
    ActionQueue* getActions(){ return _operations; }
    void setActions(ActionQueue* aQueue){ _operations = aQueue; }

    //FileOutputAction* getOutput(){ return _outputAction; }
    std::vector< osg::ref_ptr<ActionData> >& getOutputDatas(){ return _outputDatas; }

    FileInputAction* addInput(const std::string& aFilePath, const std::string& aCommonPath="")
    {
        graft::FileInputAction* inputOp = new graft::FileInputAction();
        inputOp->setFilePath(aFilePath);

        getInputs()->addAction(inputOp);
        getOutputDatas().push_back(NULL);

        return inputOp;
    }

    bool removeInput(FileInputAction* anInput)
    {
        unsigned int index = getInputs()->getActionIndex(anInput);
        if(index >= getInputs()->getNumActions())
            return false;
        return removeInput(index);
    }
    bool removeInput(unsigned int anIndex)
    {
        getInputs()->removeAction(anIndex);

        unsigned int endOfRemoveRange = anIndex+1;
        if (endOfRemoveRange>getOutputDatas().size())
            endOfRemoveRange=getOutputDatas().size();

        getOutputDatas().erase(getOutputDatas().begin()+anIndex,getOutputDatas().begin()+endOfRemoveRange);
        return true;
    }

    void setCallback(ProcessingInputCallback* aCallback)
    {
        _processingCallback = aCallback;
    }

    void process(const unsigned int aStartIndex, const unsigned int aProcessCount = 1)
    {
        unsigned int endIndex = aStartIndex + aProcessCount;
        if(endIndex >= _inputActions->getNumActions())
            endIndex = _inputActions->getNumActions();

        // create an operation data and pass to input operation, then send through actions
        for(unsigned int i=aStartIndex; i<endIndex; i++)
        {
            if(_outputDatas[i] == NULL)
            {
                _outputDatas[i] = new ActionData();
                _outputDatas[i]->_context = _context;
            }

            Action* input = _inputActions->getAction(i);

            if(_processingCallback.valid())
                _processingCallback->processing(this, i, i-aStartIndex, aProcessCount);

            input->process(_outputDatas[i]);

            for(unsigned int t=0; t<_operations->getNumActions(); t++)
            {
                Action* action = _operations->getAction(t);
                OSG_ALWAYS << "Action " << action->friendlyName() << " is " << (action->getEnabled() ? "enabled" : "disabled") << std::endl;
                if(action->getEnabled()) action->process(_outputDatas[i]);
                //
                if(_processingCallback.valid() && !_processingCallback->shouldContinue()){
                    _processingCallback->cancel();
                    break;
                }
            }

            if(_processingCallback.valid() && !_processingCallback->shouldContinue()){
                _processingCallback->cancel();
                break;
            }

            if(_processingCallback.valid())
                _processingCallback->processingComplete(this, i, i-aStartIndex, aProcessCount);
        }
    }

protected:
    virtual ~BatchConvertor(void){}

    osg::ref_ptr<ActionQueue> _inputActions;
    osg::ref_ptr<ActionQueue> _operations;

    std::vector< osg::ref_ptr<ActionData> > _outputDatas;

    osg::ref_ptr<ProcessingInputCallback> _processingCallback;

    osg::ref_ptr<osg::GraphicsContext> _context;
};

} // end graft nampespace

#endif // GRAFT_BATCHCONVERTOR_H
