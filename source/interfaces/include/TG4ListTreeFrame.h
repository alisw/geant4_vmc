// $Id: TG4ListTreeFrame.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup interfaces
//
/// \class TG4ListTreeFrame
///
/// Frame for the ListTree container
///
/// Author: D. Adamova

#ifndef TG4_LISTTREE_FRAME_H
#define TG4_LISTTREE_FRAME_H

#include <TGFrame.h>
#include <TObject.h>

class TGListTreeItem;
class TGListTree;
class TGCanvas;
class TGTab;
class TGPicture;

class TG4ListTreeFrame : public TObject {
public:   

    TG4ListTreeFrame( TGTab* tab, TGMainFrame* actionFrame);
    virtual ~TG4ListTreeFrame();

    Bool_t ProcessSubMessage(Long_t msg, Long_t parm1);  
    void DrawSelectedVolume(TGListTreeItem* item);
    TGListTreeItem*
        AddItem(TObject* obj, TGListTreeItem* parent,const char* name,
                const TGPicture* open, const TGPicture* closed);    
    void SendCloseMessage();                
//---------------------------------------------------------------------------

protected:

    TG4ListTreeFrame& operator=(const TG4ListTreeFrame& ltf);
    TG4ListTreeFrame(const TG4ListTreeFrame& ltf); 		    
//----------------------------------------------------

private:

    TGCanvas*   fCanvasWindow;  // Canvas window for the list tree
    TGListTree* fVolumesListTree;  // volumes list tree 
			
    ClassDef(TG4ListTreeFrame,0)  // the frame for the volumes list tree 
  };
  
//

#endif
