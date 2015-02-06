

#ifndef WMLSelectElement_h
#define WMLSelectElement_h

#if ENABLE(WML)
#include "WMLFormControlElement.h"
#include "SelectElement.h"

namespace WebCore {

class WMLSelectElement : public WMLFormControlElement, public SelectElement {
public:
    WMLSelectElement(const QualifiedName&, Document*);
    virtual ~WMLSelectElement();

    virtual const AtomicString& formControlName() const;
    virtual const AtomicString& formControlType() const;
 
    virtual bool isKeyboardFocusable(KeyboardEvent*) const;
    virtual bool isMouseFocusable() const;
    virtual bool canSelectAll() const { return !m_data.usesMenuList(); }
    virtual void selectAll();

    virtual void recalcStyle(StyleChange);

    virtual void dispatchFocusEvent();
    virtual void dispatchBlurEvent();

    virtual bool canStartSelection() const { return false; }

    virtual int selectedIndex() const;
    virtual void setSelectedIndex(int index, bool deselect = true);
    virtual void setSelectedIndexByUser(int index, bool deselect = true, bool fireOnChangeNow = false);

    virtual int size() const { return m_data.size(); }
    virtual bool multiple() const { return m_data.multiple(); }

    virtual bool saveFormControlState(String& value) const;
    virtual void restoreFormControlState(const String&);

    virtual void childrenChanged(bool changedByParser = false, Node* beforeChange = 0, Node* afterChange = 0, int childCountDelta = 0);

    virtual void parseMappedAttribute(MappedAttribute*);

    virtual RenderObject* createRenderer(RenderArena*, RenderStyle*);
    virtual bool appendFormData(FormDataList&, bool);
    virtual int optionToListIndex(int optionIndex) const;
    virtual int listToOptionIndex(int listIndex) const;

    virtual const Vector<Element*>& listItems() const { return m_data.listItems(this); }
    virtual void reset();

    virtual void defaultEventHandler(Event*);
    virtual void accessKeyAction(bool sendToAnyElement);
    virtual void setActiveSelectionAnchorIndex(int index);
    virtual void setActiveSelectionEndIndex(int index);
    virtual void updateListBoxSelection(bool deselectOtherOptions);
    virtual void listBoxOnChange();
    virtual void menuListOnChange();

    virtual int activeSelectionStartListIndex() const;
    virtual int activeSelectionEndListIndex() const;

    void accessKeySetSelectedIndex(int);
    void setRecalcListItems();
    void scrollToSelection();
    void selectInitialOptions();

    bool initialized() const { return m_initialized; }

private:
    virtual void insertedIntoTree(bool);

    void calculateDefaultOptionIndices();
    void selectDefaultOptions();
    void initializeVariables();
    void updateVariables();

    Vector<unsigned> parseIndexValueString(const String&) const;
    Vector<unsigned> valueStringToOptionIndices(const String&) const;
    String optionIndicesToValueString() const;
    String optionIndicesToString() const;

    String name() const;
    String value() const;
    String iname() const;
    String ivalue() const;

    SelectElementData m_data;
    bool m_initialized;
    Vector<unsigned> m_defaultOptionIndices;
};

}

#endif
#endif
